#include "tableGenerator.h"
#include "com_aimazing_aimazinglib.h"

void tableGenerator::ShiftRows(unsigned char cipher[16]){
    // +----+----+----+----+
    // | 00 | 04 | 08 | 12 |
    // +----+----+----+----+
    // | 01 | 05 | 09 | 13 |
    // +----+----+----+----+
    // | 02 | 06 | 10 | 14 |
    // +----+----+----+----+
    // | 03 | 07 | 11 | 15 |
    // +----+----+----+----+
    unsigned char tmp1, tmp2;

    tmp1 = cipher[1];
    cipher[1] = cipher[5];
    cipher[5] = cipher[9];
    cipher[9] = cipher[13];
    cipher[13] = tmp1;

    tmp1 = cipher[2];
    tmp2 = cipher[6];
    cipher[2] = cipher[10];
    cipher[6] = cipher[14];
    cipher[10] = tmp1;
    cipher[14] = tmp2;

    tmp1 = cipher[3];
    cipher[3] = cipher[15];
    cipher[15] = cipher[11];
    cipher[11] = cipher[7];
    cipher[7] = tmp1;
}

void tableGenerator::InvShiftRows(unsigned char plain[16]){
    // +----+----+----+----+
    // | 00 | 04 | 08 | 12 |
    // +----+----+----+----+
    // | 01 | 05 | 09 | 13 |
    // +----+----+----+----+
    // | 02 | 06 | 10 | 14 |
    // +----+----+----+----+
    // | 03 | 07 | 11 | 15 |
    // +----+----+----+----+
    unsigned char tmp1, tmp2;

    tmp1 = plain[13];
    plain[13] = plain[9];
    plain[9] = plain[5];
    plain[5] = plain[1];
    plain[1] = tmp1;

    tmp2 = plain[14];
    tmp1 = plain[10];
    plain[14] = plain[6];
    plain[10] = plain[2];
    plain[6] = tmp2;
    plain[2] = tmp1;

    tmp1 = plain[7];
    plain[7] = plain[11];
    plain[11] = plain[15];
    plain[15] = plain[3];
    plain[3] = tmp1;
}

void tableGenerator::ROT(unsigned int *d){
    unsigned int rotated_bit = *d & 0xFF;
    *d = (rotated_bit << 24) | (*d >> 8);
}

void tableGenerator::generate_key(){
    srand(time(NULL));
    rand() / (RAND_MAX / (16));

    unsigned char list[16] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };
    int index,index2;

    memset(key, 0x0, 16);
    for (int i = 0; i < 16; ++i){
        index = rand() / (RAND_MAX / (16));
        index2 = rand() / (RAND_MAX / (16));

        key[i] = (list[index] < 4) | list[index2];
    }
    return;
}

unsigned int tableGenerator::myrand(){
    return (rand());
}

unsigned int tableGenerator::myRandHex(){
    return (rand() / (RAND_MAX + 1.0) * 16);
}

// Mixing Bijection
void tableGenerator::AddToCol(mat_GF2& x, long j, const vec_GF2& a)
// add a to column j of x
// ALIAS RESTRICTION: a should not alias any row of x
{
    long n = x.NumRows();
    long m = x.NumCols();

    if (a.length() != n || j < 0 || j >= m)
        Error("AddToCol: bad args");

    long wj = j/NTL_BITS_PER_LONG;
    long bj = j - wj*NTL_BITS_PER_LONG;
    _ntl_ulong j_mask = 1UL << bj;

    const _ntl_ulong *ap = a.rep.elts();
    _ntl_ulong a_mask = 1;

    long i;
    for (i = 0; i < n; i++) {
        if (*ap & a_mask)
            x[i].rep.elts()[wj] ^= j_mask;

        a_mask <<= 1;
        if (!a_mask) {
            a_mask = 1;
            ap++;
        }
    }
}

long tableGenerator::invP(ref_GF2 d, mat_GF2& X, mat_GF2& Q, const mat_GF2& A)
{
    long n = A.NumRows();
    if (A.NumCols() != n)
        Error("solve: nonsquare matrix");

    if (n == 0) {
        X.SetDims(0, 0);
        set(d);
    }

    long i, j, k, pos;
    long rank=n;

    //
    // Gauss Jordan Elimination. Matrix M is extended version
    // with 2*N columns. Copy of A is in the left half, unity
    // matrix is in the right half.
    //
    mat_GF2 M;
    M.SetDims(n, 2*n);

    vec_GF2 aa;
    aa.SetLength(2*n);

    // Initializing Q matrix as unit matrix, will correspond to
    // column operations performed to obtain canonical form.
    // Since matrix is represented as array of vectors (rows),
    // we will work with transpose version of matrix.
    mat_GF2 I;
    ident(I, n);
    transpose(Q, I);
    for (i = 0; i < n; i++) {
        aa = A[i];
        aa.SetLength(2*n);
        aa.put(n+i, 1);
        M[i] = aa;
    }

    long wn = ((2*n) + NTL_BITS_PER_LONG - 1)/NTL_BITS_PER_LONG;
    for (k = 0; k < n; k++) {
        long wk = k/NTL_BITS_PER_LONG;
        long bk = k - wk*NTL_BITS_PER_LONG;
        _ntl_ulong k_mask = 1UL << bk;

#ifdef DEBUGOUT
        cout << "Intermediate result in step=" << k <<  "; Matrix" << endl << M << endl;
#endif
        // Find leading one in rows on k-th position in row.
        // Search in interval [k,n] (thus from current row down).
        pos = -1;
        for (i = k; i < n; i++) {
            if (M[i].rep.elts()[wk] & k_mask) {
                pos = i;
                break;
            }
        }
#ifdef DEBUGOUT
        cout << "Line pos: [" << pos << "] has leading 1 on [" << k << "]. position" << endl;
#endif
        if (pos == -1) {
            // If here it means there is no row in matrix that has leading
            // 1 on k-th position.
            //
            // Thus now look in rows [k,n] and find some row that has
            // 1 element on position > k. Then we will perform column swap
            // to obtain 1 element on desired position = k. This change has to be
            // reflected to Q matrix.
            //
            // Finding unit element on position k+1 in all rows [k,n].
            // If fails, look for unit element on position k+2 in all rows [k,n]...
            int kk, ii, colpos=-1;
            for (kk = k+1; kk < n; kk++) {
                long kwk = kk / NTL_BITS_PER_LONG;
                long kbk = kk - kwk * NTL_BITS_PER_LONG;
                _ntl_ulong kk_mask = 1UL << kbk;
                colpos=kk;

                // Find leading one in rows on kk-th position in row.
                // Search in interval [k,n] (thus from current row down).
#ifdef DEBUGOUT
                cout << "Looking for leading 1 element in column: " << kk << "; mask: " << kk_mask << endl;
#endif
                pos = -1;
                for (ii = k; ii < n; ii++) {
                    if (M[ii].rep.elts()[kwk] & kk_mask) {
                        pos = ii;
                        break;
                    }
                }
                if (pos!=-1) break;
            }


            if (pos==-1){
                // No such column exists, thus just simply null rest of columns in Q matrix
                // to obtain canonical form of product PAQ.
                rank = k;
#ifdef DEBUGOUT
                cout << "No such column exists, we are already in canonical form;"\
						"nulling all columns from: " << k << "; Rank: " << rank << endl;
#endif
                for(kk=k; kk<n; kk++){
                    for(ii=0; ii<n; ii++){
                        Q.put(kk, ii, 0);
                    }
                }

                break;
            }

#ifdef DEBUGOUT
            cout << "Swaping column [" << k << "] with column [" << colpos << "]. Matrix: " <<endl;
#endif
            // Do column swap to obtain 1 on desired k-th position.
            for(ii=0; ii<n; ii++){
                GF2 tmp = M.get(ii, k);
                M.put(ii, k, M.get(ii, colpos));
                M.put(ii, colpos, tmp);
            }

            // reflect this swap to Q matrix, swap rows (transpose form)
            swap(Q[colpos], Q[k]);
#ifdef DEBUGOUT
            cout << M << endl << "Qmatrix: " << endl << Q << endl << endl;
#endif
        }

        if (pos != -1) {
            // row number <pos> has leading one on k-th position
            if (k != pos) {
#ifdef DEBUGOUT
                cout << "Swap line " << pos << " with line " << k << endl;
#endif
                swap(M[pos], M[k]);
            }

            // obtain bit representation of vector in i-th row
            _ntl_ulong *y = M[k].rep.elts();

            for (i = k+1; i < n; i++) {
                // M[i] = M[i] + M[k]*M[i,k]

                // By another words, we are re-seting other 1s
                // in rows > k (down).
                if (M[i].rep.elts()[wk] & k_mask) {
                    _ntl_ulong *x = M[i].rep.elts();

                    // simple element-by-element addition
                    for (j = wk; j < wn; j++)
                        x[j] ^= y[j];
                }
            }
        }
    }

    vec_GF2 XX;
    XX.SetLength(2*n);

    X.SetDims(n, n);
    clear(X);

    for (j = 0; j < n; j++) {
        XX.SetLength(n+j+1);
        clear(XX);
        XX.put(n+j, to_GF2(1));

        for (i = n-1; i >= 0; i--) {
            XX.put(i, XX*M[i]);
        }

        XX.SetLength(n);
        AddToCol(X, j, XX);
    }

    // transpose Q matrix finally
    Q = transpose(Q);

    // determinant=0 <=> rank == n
    if (rank==n) set(d);
    else clear(d);

    return rank;
}

void tableGenerator::generateARankMatrix(mat_GF2& A, int rank, int n){
    long i=0, offset=0;

    A.SetDims(n,n);
    clear(A);
    if (rank==1){
        // On rank = 1 matrix has special form [1 1; 1 0] and then I
        A.put(0,0,1);
        A.put(0,1,1);
        A.put(1,0,1);
        for(i=2; i<n; i++){
            A.put(i,i,1);
        }
        return;
    }

    if ((rank % 2) == 1){
        // First block of matrix is 3x3 in special form [1 1 1; 1 1 0; 1 0 0]
        A.put(0,0,1);
        A.put(0,1,1);
        A.put(0,2,1);
        A.put(1,0,1);
        A.put(1,1,1);
        A.put(2,0,1);
        offset=3;
    }

    //
    // Merged case - r is odd or even and >= 3
    //

    // For even rank it is easy to construct
    // On diagonals is <rank> copies of matrix [0 1; 1 1]
    // filled with I_2 on rest of blocks
    for(i=0; i<rank/2; i++){
        A.put(2*i   + offset, 2*i+1 + offset, 1);
        A.put(2*i+1 + offset, 2*i   + offset, 1);
        A.put(2*i+1 + offset, 2*i+1 + offset, 1);
    }
    // the rest fill with 1 on diagonals (I_{n-r} matrix)
    for(i=rank+offset-1; i<n; i++){
        if (i<0) continue;
        A.put(i,i,1);
    }
    return;
}

long tableGenerator::generateInvertiblePM(mat_GF2& M, int p){
    int rounds=0;
    long i, j;
    GF2 det;

    // Initialize M as square matrix pxp
    M.SetDims(p,p);

    // Iterate until we have some invertible matrix, or to some boundary.
    // Reaching this boundary is highly improbable for small p.
    for(rounds=0; rounds < 100; rounds++){
        // Fill matrix with random values and then compute determinant.
        for(i=0; i<p; i++){
            for(j=0; j<p; j++){
                M.put(i,j,myrand()%2);
            }
        }

        // test for determinant. If determinant != 0 then matrix is non-singular, invertible
        determinant(det, M);
        if (det!=0){
            return rounds;
        }
    }

    return -1;
}

int tableGenerator::generateMixingBijection(mat_GF2& RES, int t, int p){
    // validate parameters
    if (t<p || (t%p) != 0){
        return -1;
    }
    RES.SetDims(t,t);

    // 0. generate M matrix pxp that is invertible
    mat_GF2 M;
    long res = generateInvertiblePM(M, p);
    if (res < 0) {
        // matrix was not found in 100 steps, weeeeird. HIGHLY UNPOSSIBLE.
        return -1;
    }
#ifdef DEBUGOUT
    cout << "generated M0 invertible matrix: " << endl << M << endl << endl;
#endif
    // some matrices that we will need, naming according to the paper
    mat_GF2 X; 	mat_GF2 Y;
    mat_GF2 P;	mat_GF2 Pinv;
    mat_GF2 Q;	mat_GF2 Qinv;
    mat_GF2 A;
    mat_GF2 TMP;
    mat_GF2 Minv;
    mat_GF2 N;
    GF2 d;
    ref_GF2 dd(d);

    int i,j,k;
    int curT = p;			// current size of matrix M
    int tmp;				// current column/row
    for(; curT < t; curT+=p){
        int pBlocksInM=curT/p;	// number of pxp sub-matrices in M

        // 1. X matrix - p x t matrix, generated from M matrix using some row
        X.SetDims(p, curT);
        tmp = myrand() % pBlocksInM;		// current row
        for(i=p*tmp,k=0; k<p; i++, k++){
            for(j=0; j<curT; j++){
                X.put(k,j, M.get(i,j));
            }
        }

        // 2. Y matrix - t x p matrix, generated from M matrix using some column
        Y.SetDims(curT, p);
        tmp = myrand() % pBlocksInM;
        for(i=0; i<curT; i++){
            for(j=p*tmp,k=0; k<p; j++, k++){
                Y.put(i,k, M.get(i,j));
            }
        }

        // 3. computing invertible P,Q matrices
        inv(Minv, M);
        TMP = X * Minv * Y;
#ifdef DEBUGOUT
        cout << "X matrix:" << endl << X << endl << endl;
		cout << "Y matrix:" << endl << Y << endl << endl;
		cout << "Generated M inverse: " << endl << Minv << endl << endl;
		cout << "TMP: " << endl << TMP << endl << endl;
#endif
        int rank = invP(dd, P, Q, TMP);
#ifdef DEBUGOUT
        cout << "Rank of TMP: " << rank;
		cout << "; P=" << endl << P << endl << endl;
		cout << "; Q=" << endl << Q << endl << endl;
#endif
        // 4. A matrix
        generateARankMatrix(A, rank, p);
#ifdef DEBUGOUT
        cout << "; A=" << endl << A << endl << endl;
#endif
        // 5. resulting matrix
        mat_GF2 TMP2;
        N.SetDims(curT + p, curT + p);
        inv(Pinv, P);
        inv(Qinv, Q);
        TMP2 = TMP + Pinv*A*Qinv;

        // copy M matrix, M is curT x curT matrix
        for(i=0;i<curT;i++){
            for(j=0;j<curT;j++){
                N.put(i,j,M.get(i,j));
            }
        }
        // copy X matrix, p x curT
        for(i=0;i<p;i++){
            for(j=0;j<curT;j++){
                N.put(curT+i,j,X.get(i,j));
            }
        }
        // copy Y matrix, curT x p
        for(i=0;i<curT;i++){
            for(j=0;j<p;j++){
                N.put(i,curT+j,Y.get(i,j));
            }
        }
        // copy TMP2 matrix, p x p
        for(i=0;i<p;i++){
            for(j=0;j<p;j++){
                N.put(curT+i,curT+j,TMP2.get(i,j));
            }
        }
#ifdef DEBUGOUT
        cout << "Intermediate result for curT=" << curT << "; Matrix = " << endl << N << endl << endl;
#endif
        M = N;
    }

    RES = M;
    return 0;
}

void tableGenerator::generateMixingBijections(MB08x08_TABLE L08x08[][16], MB32x32_TABLE MB32x32[][4]){
    srand(time(NULL));
    //	Generating L
    for(int r=0; r<9; r++){
        for(int i=0; i<16; i++){
            generateMixingBijection(L08x08[r][i].mb, 8, 4);
            L08x08[r][i].inv = inv(L08x08[r][i].mb);
        }
    }

    //	Generating MB
    for(int r=0; r<9; r++){
        for(int i=0; i<4; i++){
            generateMixingBijection(MB32x32[r][i].mb, 32, 4);
            MB32x32[r][i].inv = inv(MB32x32[r][i].mb);
        }
    }

    return;
}

// Internal Encoding
void tableGenerator::generateInternalEncoding(unsigned char encodingTable[0x10], unsigned char encodingTableInverse[0x10]){

    unsigned char list[16] = {0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF};
    memcpy(encodingTable, list, 16);
    unsigned char tmp;
    for (int i = 0; i < 0x10; ++i){
        int index = myRandHex();
        tmp = encodingTable[index];
        encodingTable[index] = encodingTable[i];
        encodingTable[i] = tmp;
    }
    for (int i = 0; i < 0x10; ++i){
        encodingTableInverse[encodingTable[i]] = i;
    }


    return;
}


// char byte to 8bit vector
void tableGenerator::CharToGF2(unsigned char c, vec_GF2& vec){
    clear(vec);
    GF2 c_gf2;
    for (int i = 0; i < 8; ++i){
        c_gf2 = (c>>7-i)&0x1;
        vec.put(i, c_gf2);
    }

    return;

}
// int to 32bit vector
void tableGenerator::IntToGF2(unsigned int i, vec_GF2& vec){
    clear(vec);
    GF2 i_gf2;
    for (int z = 0; z < 32; ++z){
        i_gf2 = (i>>31-z)&0x1;
        vec.put(z, i_gf2);
    }
}

// 8bit vector to char byte
void tableGenerator::GF2ToChar(vec_GF2 vec, unsigned char& c){
    c = 0x00;
    int tmp = 0x0;
    int output = 0x0;
    for (int i = 0; i < 8; ++i){
        if (vec.get(7-i) == (GF2) 0)
            tmp = 0x00;
        else
            tmp = 0x01;

        output = output | (tmp << i);
    }
    c = (unsigned char) output;
    return;
}
// 32bit vector to int
void tableGenerator::GF2ToInt(vec_GF2 vec, unsigned int& i){
    i = 0x000000;
    int tmp = 0x0;
    int output = 0x0;
    for (int z = 0; z < 32; ++z){
        if (vec.get(31-z) == (GF2) 0)
            tmp = 0x00;
        else
            tmp = 0x01;

        output = output | (tmp << z);
    }
    i = (unsigned int) output;
    return;
}

void tableGenerator::key_schedule(unsigned char round_keys[11][16]){

    memcpy(&round_keys[0][0], key, 16);
    unsigned int d = DW(&round_keys[0][12]);
    for (size_t i = 1; i < 11; ++i)
    {
        // Rotate `d` 8 bits to the right
        ROT(&d);

        // Takes every bytes of `d` & substitute them using `S_box`
        unsigned char a1, a2, a3, a4;
        // Do not forget to xor this byte with `rcon[i]`
        a1 = S_box[(d >> 0) & 0xff] ^ rcon[i]; // a1 is the LSB
        a2 = S_box[(d >> 8) & 0xff];
        a3 = S_box[(d >> 16) & 0xff];
        a4 = S_box[(d >> 24) & 0xff];

        d = (a1 << 0) | (a2 << 8) | (a3 << 16) | (a4 << 24);

        // Now we can generate the current roundkey using the previous one
        for (size_t j = 0; j < 4; j++)
        {
            d ^= DW(&(round_keys[i - 1][j * 4]));
            *(unsigned int*)(&(round_keys[i][j * 4])) = d;
        }
    }
}

void tableGenerator::writeTable(std::string path){

    LOGD("CPP==============", "writeTable() start");

    FILE *fpEnc;
    FILE *fpDec;

    std::string path_enc = path + "tables_encrypt.bin";
    std::string path_dec = path + "tables_decrypt.bin";

    fpEnc=fopen(path_enc.c_str(),"wb");
    fpDec=fopen(path_dec.c_str(),"wb");

    if (!fpEnc || !fpDec){
        LOGD("CPP==============", "Can't open file for table generation!\n");
        return;
    }

    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 16; ++j){
            for (int k = 0; k < 0x100; ++k){
                fwrite(&Tyboxes[i][j][k], sizeof(int), 1, fpEnc);
            }
        }
    }
    for (int j = 0; j < 16; ++j){
        for (int k = 0; k < 0x100; ++k){
            fwrite(&Tbox[j][k], sizeof(char), 1, fpEnc);
        }
    }
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 16; ++j){
            for (int k = 0; k < 0x100; ++k){
                fwrite(&MixingBijection[i][j][k], sizeof(int), 1, fpEnc);
            }
        }
    }

    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 16; ++j){
            for (int k = 0; k < 0x100; ++k){
                fwrite(&Tyboxes_decrypt[i][j][k], sizeof(int), 1, fpDec);
            }
        }
    }
    for (int j = 0; j < 16; ++j){
        for (int k = 0; k < 0x100; ++k){
            fwrite(&Tbox_decrypt[j][k], sizeof(char), 1, fpDec);
        }
    }
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 16; ++j){
            for (int k = 0; k < 0x100; ++k){
                fwrite(&MixingBijection_decrypt[i][j][k], sizeof(int), 1, fpDec);
            }
        }
    }
    for (int i = 0; i < 0x10; ++i){
        for (int j = 0; j < 0x10; ++j){
            fwrite(&Txor[i][j], sizeof(char), 1, fpEnc);
            fwrite(&Txor[i][j], sizeof(char), 1, fpDec);
        }
    }


    fclose(fpEnc);
    fclose(fpDec);
    LOGD("CPP==============", "writeTable END");
    return;

    // Tyboxes
    /*
    file.write(Tyboxes, 9*16*256*sizeof(unsigned int));
    file.write(Tyboxes_decrypt, 9*16*256*sizeof(unsigned int));
    file.write(Tbox, 16*256*sizeof(unsigned char));
    file.write(Tbox_decrypt, 16*256*sizeof(unsigned char));
    file.write(Txor, 16*16*sizeof(unsigned char));
    */

}


void tableGenerator::generate_table(std::string path)
{
    LOGD("CPP==============", "generateTable() start");

    // Inverse S-box
    unsigned char Inv_S_box[256];
    for (size_t i = 0; i < 256; ++i)
        Inv_S_box[S_box[i]] = i;

    unsigned char round_keys[11][16] = { 0 };

    const unsigned char matrix[16] = {
            /*
            2, 3, 1, 1,
            1, 2, 3, 1,
            1, 1, 2, 3,
            3, 1, 1, 2
            */
            1, 2, 0, 0,
            0, 1, 2, 0,
            0, 0, 1, 2,
            2, 0, 0, 1
    },
            gmul[3][0x100] = {
            { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF },
            { 0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4A, 0x4C, 0x4E, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5A, 0x5C, 0x5E, 0x60, 0x62, 0x64, 0x66, 0x68, 0x6A, 0x6C, 0x6E, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7A, 0x7C, 0x7E, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C, 0x8E, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9A, 0x9C, 0x9E, 0xA0, 0xA2, 0xA4, 0xA6, 0xA8, 0xAA, 0xAC, 0xAE, 0xB0, 0xB2, 0xB4, 0xB6, 0xB8, 0xBA, 0xBC, 0xBE, 0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE, 0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE, 0xF0, 0xF2, 0xF4, 0xF6, 0xF8, 0xFA, 0xFC, 0xFE, 0x1B, 0x19, 0x1F, 0x1D, 0x13, 0x11, 0x17, 0x15, 0x0B, 0x09, 0x0F, 0x0D, 0x03, 0x01, 0x07, 0x05, 0x3B, 0x39, 0x3F, 0x3D, 0x33, 0x31, 0x37, 0x35, 0x2B, 0x29, 0x2F, 0x2D, 0x23, 0x21, 0x27, 0x25, 0x5B, 0x59, 0x5F, 0x5D, 0x53, 0x51, 0x57, 0x55, 0x4B, 0x49, 0x4F, 0x4D, 0x43, 0x41, 0x47, 0x45, 0x7B, 0x79, 0x7F, 0x7D, 0x73, 0x71, 0x77, 0x75, 0x6B, 0x69, 0x6F, 0x6D, 0x63, 0x61, 0x67, 0x65, 0x9B, 0x99, 0x9F, 0x9D, 0x93, 0x91, 0x97, 0x95, 0x8B, 0x89, 0x8F, 0x8D, 0x83, 0x81, 0x87, 0x85, 0xBB, 0xB9, 0xBF, 0xBD, 0xB3, 0xB1, 0xB7, 0xB5, 0xAB, 0xA9, 0xAF, 0xAD, 0xA3, 0xA1, 0xA7, 0xA5, 0xDB, 0xD9, 0xDF, 0xDD, 0xD3, 0xD1, 0xD7, 0xD5, 0xCB, 0xC9, 0xCF, 0xCD, 0xC3, 0xC1, 0xC7, 0xC5, 0xFB, 0xF9, 0xFF, 0xFD, 0xF3, 0xF1, 0xF7, 0xF5, 0xEB, 0xE9, 0xEF, 0xED, 0xE3, 0xE1, 0xE7, 0xE5 },
            { 0x00, 0x03, 0x06, 0x05, 0x0C, 0x0F, 0x0A, 0x09, 0x18, 0x1B, 0x1E, 0x1D, 0x14, 0x17, 0x12, 0x11, 0x30, 0x33, 0x36, 0x35, 0x3C, 0x3F, 0x3A, 0x39, 0x28, 0x2B, 0x2E, 0x2D, 0x24, 0x27, 0x22, 0x21, 0x60, 0x63, 0x66, 0x65, 0x6C, 0x6F, 0x6A, 0x69, 0x78, 0x7B, 0x7E, 0x7D, 0x74, 0x77, 0x72, 0x71, 0x50, 0x53, 0x56, 0x55, 0x5C, 0x5F, 0x5A, 0x59, 0x48, 0x4B, 0x4E, 0x4D, 0x44, 0x47, 0x42, 0x41, 0xC0, 0xC3, 0xC6, 0xC5, 0xCC, 0xCF, 0xCA, 0xC9, 0xD8, 0xDB, 0xDE, 0xDD, 0xD4, 0xD7, 0xD2, 0xD1, 0xF0, 0xF3, 0xF6, 0xF5, 0xFC, 0xFF, 0xFA, 0xF9, 0xE8, 0xEB, 0xEE, 0xED, 0xE4, 0xE7, 0xE2, 0xE1, 0xA0, 0xA3, 0xA6, 0xA5, 0xAC, 0xAF, 0xAA, 0xA9, 0xB8, 0xBB, 0xBE, 0xBD, 0xB4, 0xB7, 0xB2, 0xB1, 0x90, 0x93, 0x96, 0x95, 0x9C, 0x9F, 0x9A, 0x99, 0x88, 0x8B, 0x8E, 0x8D, 0x84, 0x87, 0x82, 0x81, 0x9B, 0x98, 0x9D, 0x9E, 0x97, 0x94, 0x91, 0x92, 0x83, 0x80, 0x85, 0x86, 0x8F, 0x8C, 0x89, 0x8A, 0xAB, 0xA8, 0xAD, 0xAE, 0xA7, 0xA4, 0xA1, 0xA2, 0xB3, 0xB0, 0xB5, 0xB6, 0xBF, 0xBC, 0xB9, 0xBA, 0xFB, 0xF8, 0xFD, 0xFE, 0xF7, 0xF4, 0xF1, 0xF2, 0xE3, 0xE0, 0xE5, 0xE6, 0xEF, 0xEC, 0xE9, 0xEA, 0xCB, 0xC8, 0xCD, 0xCE, 0xC7, 0xC4, 0xC1, 0xC2, 0xD3, 0xD0, 0xD5, 0xD6, 0xDF, 0xDC, 0xD9, 0xDA, 0x5B, 0x58, 0x5D, 0x5E, 0x57, 0x54, 0x51, 0x52, 0x43, 0x40, 0x45, 0x46, 0x4F, 0x4C, 0x49, 0x4A, 0x6B, 0x68, 0x6D, 0x6E, 0x67, 0x64, 0x61, 0x62, 0x73, 0x70, 0x75, 0x76, 0x7F, 0x7C, 0x79, 0x7A, 0x3B, 0x38, 0x3D, 0x3E, 0x37, 0x34, 0x31, 0x32, 0x23, 0x20, 0x25, 0x26, 0x2F, 0x2C, 0x29, 0x2A, 0x0B, 0x08, 0x0D, 0x0E, 0x07, 0x04, 0x01, 0x02, 0x13, 0x10, 0x15, 0x16, 0x1F, 0x1C, 0x19, 0x1A }
    };

    const unsigned char matrix_decrypt[16] = {
            /*
            14, 11, 13,  9,
             9, 14, 11, 13,
            13,  9, 14, 11,
            11, 13,  9, 14
            */
            0, 1, 2, 3,
            3, 0, 1, 2,
            2, 3, 0, 1,
            1, 2, 3, 0
    },
            gmul_decrypt[4][0x100] = {
            { 0x00, 0x0e, 0x1c, 0x12, 0x38, 0x36, 0x24, 0x2a, 0x70, 0x7e, 0x6c, 0x62, 0x48, 0x46, 0x54, 0x5a, 0xe0, 0xee, 0xfc, 0xf2, 0xd8, 0xd6, 0xc4, 0xca, 0x90, 0x9e, 0x8c, 0x82, 0xa8, 0xa6, 0xb4, 0xba, 0xdb, 0xd5, 0xc7, 0xc9, 0xe3, 0xed, 0xff, 0xf1, 0xab, 0xa5, 0xb7, 0xb9, 0x93, 0x9d, 0x8f, 0x81, 0x3b, 0x35, 0x27, 0x29, 0x03, 0x0d, 0x1f, 0x11, 0x4b, 0x45, 0x57, 0x59, 0x73, 0x7d, 0x6f, 0x61, 0xad, 0xa3, 0xb1, 0xbf, 0x95, 0x9b, 0x89, 0x87, 0xdd, 0xd3, 0xc1, 0xcf, 0xe5, 0xeb, 0xf9, 0xf7, 0x4d, 0x43, 0x51, 0x5f, 0x75, 0x7b, 0x69, 0x67, 0x3d, 0x33, 0x21, 0x2f, 0x05, 0x0b, 0x19, 0x17, 0x76, 0x78, 0x6a, 0x64, 0x4e, 0x40, 0x52, 0x5c, 0x06, 0x08, 0x1a, 0x14, 0x3e, 0x30, 0x22, 0x2c, 0x96, 0x98, 0x8a, 0x84, 0xae, 0xa0, 0xb2, 0xbc, 0xe6, 0xe8, 0xfa, 0xf4, 0xde, 0xd0, 0xc2, 0xcc, 0x41, 0x4f, 0x5d, 0x53, 0x79, 0x77, 0x65, 0x6b, 0x31, 0x3f, 0x2d, 0x23, 0x09, 0x07, 0x15, 0x1b, 0xa1, 0xaf, 0xbd, 0xb3, 0x99, 0x97, 0x85, 0x8b, 0xd1, 0xdf, 0xcd, 0xc3, 0xe9, 0xe7, 0xf5, 0xfb, 0x9a, 0x94, 0x86, 0x88, 0xa2, 0xac, 0xbe, 0xb0, 0xea, 0xe4, 0xf6, 0xf8, 0xd2, 0xdc, 0xce, 0xc0, 0x7a, 0x74, 0x66, 0x68, 0x42, 0x4c, 0x5e, 0x50, 0x0a, 0x04, 0x16, 0x18, 0x32, 0x3c, 0x2e, 0x20, 0xec, 0xe2, 0xf0, 0xfe, 0xd4, 0xda, 0xc8, 0xc6, 0x9c, 0x92, 0x80, 0x8e, 0xa4, 0xaa, 0xb8, 0xb6, 0x0c, 0x02, 0x10, 0x1e, 0x34, 0x3a, 0x28, 0x26, 0x7c, 0x72, 0x60, 0x6e, 0x44, 0x4a, 0x58, 0x56, 0x37, 0x39, 0x2b, 0x25, 0x0f, 0x01, 0x13, 0x1d, 0x47, 0x49, 0x5b, 0x55, 0x7f, 0x71, 0x63, 0x6d, 0xd7, 0xd9, 0xcb, 0xc5, 0xef, 0xe1, 0xf3, 0xfd, 0xa7, 0xa9, 0xbb, 0xb5, 0x9f, 0x91, 0x83, 0x8d },
            { 0x00, 0x0b, 0x16, 0x1d, 0x2c, 0x27, 0x3a, 0x31, 0x58, 0x53, 0x4e, 0x45, 0x74, 0x7f, 0x62, 0x69, 0xb0, 0xbb, 0xa6, 0xad, 0x9c, 0x97, 0x8a, 0x81, 0xe8, 0xe3, 0xfe, 0xf5, 0xc4, 0xcf, 0xd2, 0xd9, 0x7b, 0x70, 0x6d, 0x66, 0x57, 0x5c, 0x41, 0x4a, 0x23, 0x28, 0x35, 0x3e, 0x0f, 0x04, 0x19, 0x12, 0xcb, 0xc0, 0xdd, 0xd6, 0xe7, 0xec, 0xf1, 0xfa, 0x93, 0x98, 0x85, 0x8e, 0xbf, 0xb4, 0xa9, 0xa2, 0xf6, 0xfd, 0xe0, 0xeb, 0xda, 0xd1, 0xcc, 0xc7, 0xae, 0xa5, 0xb8, 0xb3, 0x82, 0x89, 0x94, 0x9f, 0x46, 0x4d, 0x50, 0x5b, 0x6a, 0x61, 0x7c, 0x77, 0x1e, 0x15, 0x08, 0x03, 0x32, 0x39, 0x24, 0x2f, 0x8d, 0x86, 0x9b, 0x90, 0xa1, 0xaa, 0xb7, 0xbc, 0xd5, 0xde, 0xc3, 0xc8, 0xf9, 0xf2, 0xef, 0xe4, 0x3d, 0x36, 0x2b, 0x20, 0x11, 0x1a, 0x07, 0x0c, 0x65, 0x6e, 0x73, 0x78, 0x49, 0x42, 0x5f, 0x54, 0xf7, 0xfc, 0xe1, 0xea, 0xdb, 0xd0, 0xcd, 0xc6, 0xaf, 0xa4, 0xb9, 0xb2, 0x83, 0x88, 0x95, 0x9e, 0x47, 0x4c, 0x51, 0x5a, 0x6b, 0x60, 0x7d, 0x76, 0x1f, 0x14, 0x09, 0x02, 0x33, 0x38, 0x25, 0x2e, 0x8c, 0x87, 0x9a, 0x91, 0xa0, 0xab, 0xb6, 0xbd, 0xd4, 0xdf, 0xc2, 0xc9, 0xf8, 0xf3, 0xee, 0xe5, 0x3c, 0x37, 0x2a, 0x21, 0x10, 0x1b, 0x06, 0x0d, 0x64, 0x6f, 0x72, 0x79, 0x48, 0x43, 0x5e, 0x55, 0x01, 0x0a, 0x17, 0x1c, 0x2d, 0x26, 0x3b, 0x30, 0x59, 0x52, 0x4f, 0x44, 0x75, 0x7e, 0x63, 0x68, 0xb1, 0xba, 0xa7, 0xac, 0x9d, 0x96, 0x8b, 0x80, 0xe9, 0xe2, 0xff, 0xf4, 0xc5, 0xce, 0xd3, 0xd8, 0x7a, 0x71, 0x6c, 0x67, 0x56, 0x5d, 0x40, 0x4b, 0x22, 0x29, 0x34, 0x3f, 0x0e, 0x05, 0x18, 0x13, 0xca, 0xc1, 0xdc, 0xd7, 0xe6, 0xed, 0xf0, 0xfb, 0x92, 0x99, 0x84, 0x8f, 0xbe, 0xb5, 0xa8, 0xa3 },
            { 0x00, 0x0d, 0x1a, 0x17, 0x34, 0x39, 0x2e, 0x23, 0x68, 0x65, 0x72, 0x7f, 0x5c, 0x51, 0x46, 0x4b, 0xd0, 0xdd, 0xca, 0xc7, 0xe4, 0xe9, 0xfe, 0xf3, 0xb8, 0xb5, 0xa2, 0xaf, 0x8c, 0x81, 0x96, 0x9b, 0xbb, 0xb6, 0xa1, 0xac, 0x8f, 0x82, 0x95, 0x98, 0xd3, 0xde, 0xc9, 0xc4, 0xe7, 0xea, 0xfd, 0xf0, 0x6b, 0x66, 0x71, 0x7c, 0x5f, 0x52, 0x45, 0x48, 0x03, 0x0e, 0x19, 0x14, 0x37, 0x3a, 0x2d, 0x20, 0x6d, 0x60, 0x77, 0x7a, 0x59, 0x54, 0x43, 0x4e, 0x05, 0x08, 0x1f, 0x12, 0x31, 0x3c, 0x2b, 0x26, 0xbd, 0xb0, 0xa7, 0xaa, 0x89, 0x84, 0x93, 0x9e, 0xd5, 0xd8, 0xcf, 0xc2, 0xe1, 0xec, 0xfb, 0xf6, 0xd6, 0xdb, 0xcc, 0xc1, 0xe2, 0xef, 0xf8, 0xf5, 0xbe, 0xb3, 0xa4, 0xa9, 0x8a, 0x87, 0x90, 0x9d, 0x06, 0x0b, 0x1c, 0x11, 0x32, 0x3f, 0x28, 0x25, 0x6e, 0x63, 0x74, 0x79, 0x5a, 0x57, 0x40, 0x4d, 0xda, 0xd7, 0xc0, 0xcd, 0xee, 0xe3, 0xf4, 0xf9, 0xb2, 0xbf, 0xa8, 0xa5, 0x86, 0x8b, 0x9c, 0x91, 0x0a, 0x07, 0x10, 0x1d, 0x3e, 0x33, 0x24, 0x29, 0x62, 0x6f, 0x78, 0x75, 0x56, 0x5b, 0x4c, 0x41, 0x61, 0x6c, 0x7b, 0x76, 0x55, 0x58, 0x4f, 0x42, 0x09, 0x04, 0x13, 0x1e, 0x3d, 0x30, 0x27, 0x2a, 0xb1, 0xbc, 0xab, 0xa6, 0x85, 0x88, 0x9f, 0x92, 0xd9, 0xd4, 0xc3, 0xce, 0xed, 0xe0, 0xf7, 0xfa, 0xb7, 0xba, 0xad, 0xa0, 0x83, 0x8e, 0x99, 0x94, 0xdf, 0xd2, 0xc5, 0xc8, 0xeb, 0xe6, 0xf1, 0xfc, 0x67, 0x6a, 0x7d, 0x70, 0x53, 0x5e, 0x49, 0x44, 0x0f, 0x02, 0x15, 0x18, 0x3b, 0x36, 0x21, 0x2c, 0x0c, 0x01, 0x16, 0x1b, 0x38, 0x35, 0x22, 0x2f, 0x64, 0x69, 0x7e, 0x73, 0x50, 0x5d, 0x4a, 0x47, 0xdc, 0xd1, 0xc6, 0xcb, 0xe8, 0xe5, 0xf2, 0xff, 0xb4, 0xb9, 0xae, 0xa3, 0x80, 0x8d, 0x9a, 0x97 },
            { 0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f, 0x48, 0x41, 0x5a, 0x53, 0x6c, 0x65, 0x7e, 0x77, 0x90, 0x99, 0x82, 0x8b, 0xb4, 0xbd, 0xa6, 0xaf, 0xd8, 0xd1, 0xca, 0xc3, 0xfc, 0xf5, 0xee, 0xe7, 0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04, 0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c, 0xab, 0xa2, 0xb9, 0xb0, 0x8f, 0x86, 0x9d, 0x94, 0xe3, 0xea, 0xf1, 0xf8, 0xc7, 0xce, 0xd5, 0xdc, 0x76, 0x7f, 0x64, 0x6d, 0x52, 0x5b, 0x40, 0x49, 0x3e, 0x37, 0x2c, 0x25, 0x1a, 0x13, 0x08, 0x01, 0xe6, 0xef, 0xf4, 0xfd, 0xc2, 0xcb, 0xd0, 0xd9, 0xae, 0xa7, 0xbc, 0xb5, 0x8a, 0x83, 0x98, 0x91, 0x4d, 0x44, 0x5f, 0x56, 0x69, 0x60, 0x7b, 0x72, 0x05, 0x0c, 0x17, 0x1e, 0x21, 0x28, 0x33, 0x3a, 0xdd, 0xd4, 0xcf, 0xc6, 0xf9, 0xf0, 0xeb, 0xe2, 0x95, 0x9c, 0x87, 0x8e, 0xb1, 0xb8, 0xa3, 0xaa, 0xec, 0xe5, 0xfe, 0xf7, 0xc8, 0xc1, 0xda, 0xd3, 0xa4, 0xad, 0xb6, 0xbf, 0x80, 0x89, 0x92, 0x9b, 0x7c, 0x75, 0x6e, 0x67, 0x58, 0x51, 0x4a, 0x43, 0x34, 0x3d, 0x26, 0x2f, 0x10, 0x19, 0x02, 0x0b, 0xd7, 0xde, 0xc5, 0xcc, 0xf3, 0xfa, 0xe1, 0xe8, 0x9f, 0x96, 0x8d, 0x84, 0xbb, 0xb2, 0xa9, 0xa0, 0x47, 0x4e, 0x55, 0x5c, 0x63, 0x6a, 0x71, 0x78, 0x0f, 0x06, 0x1d, 0x14, 0x2b, 0x22, 0x39, 0x30, 0x9a, 0x93, 0x88, 0x81, 0xbe, 0xb7, 0xac, 0xa5, 0xd2, 0xdb, 0xc0, 0xc9, 0xf6, 0xff, 0xe4, 0xed, 0x0a, 0x03, 0x18, 0x11, 0x2e, 0x27, 0x3c, 0x35, 0x42, 0x4b, 0x50, 0x59, 0x66, 0x6f, 0x74, 0x7d, 0xa1, 0xa8, 0xb3, 0xba, 0x85, 0x8c, 0x97, 0x9e, 0xe9, 0xe0, 0xfb, 0xf2, 0xcd, 0xc4, 0xdf, 0xd6, 0x31, 0x38, 0x23, 0x2a, 0x15, 0x1c, 0x07, 0x0e, 0x79, 0x70, 0x6b, 0x62, 0x5d, 0x54, 0x4f, 0x46 }
    };

    key_schedule(round_keys);

    unsigned char Int_Enc[0x10];
    unsigned char Int_EncInverse[0x10];
    generateInternalEncoding(Int_Enc,Int_EncInverse);

    MB08x08_TABLE eMB_L08x08 [9][16];
    MB32x32_TABLE eMB_MB32x32[9][4];
    generateMixingBijections(eMB_L08x08, eMB_MB32x32);

    //Ty
    unsigned int Ty[4][0x100] = { 0 };
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 0x100; ++j) {
            magic_int mi;
            mi.b[0] = gmul[matrix[i + 0]][j];
            mi.b[1] = gmul[matrix[i + 4]][j];
            mi.b[2] = gmul[matrix[i + 8]][j];
            mi.b[3] = gmul[matrix[i + 12]][j];
            Ty[i][j] = mi.i;
        }
    }

    //Ty_decrypt
    unsigned int Ty_decrypt[4][0x100] = { 0 };
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 0x100; ++j) {
            magic_int mi;
            mi.b[0] = gmul_decrypt[matrix_decrypt[i + 0]][j];
            mi.b[1] = gmul_decrypt[matrix_decrypt[i + 4]][j];
            mi.b[2] = gmul_decrypt[matrix_decrypt[i + 8]][j];
            mi.b[3] = gmul_decrypt[matrix_decrypt[i + 12]][j];
            Ty_decrypt[i][j] = mi.i;
        }
    }

    //Txor
    for (size_t i = 0; i < 0x10; ++i) {
        for (size_t j = 0; j < 0x10; ++j) {
            Txor[i][j] = Int_EncInverse[i] ^ Int_EncInverse[j];
            Txor[i][j] = Int_Enc[Txor[i][j]];
        }
    }

    // Tyboxes
    for (size_t r = 0; r < 9; ++r) {
        ShiftRows(round_keys[r]);
        for (size_t i = 0; i < 16; ++i) {
            for (size_t x = 0; x < 0x100; ++x) {
                magic_int mi;
                unsigned int tmpInt;
                unsigned char tmpChar;
                unsigned char cur;
                NTL::vec_GF2 vec08(INIT_SIZE, 8);
                NTL::vec_GF2 vec32(INIT_SIZE, 32);


                cur = x;
                // L^(-1)
                if (r != 0){ // no inverse for round 0
                    cur = ((Int_EncInverse[(x>>4)&0xF]) << 4) | (Int_EncInverse[x & 0xF]);

                    tmpChar = cur;
                    CharToGF2(tmpChar, vec08);
                    vec08 = eMB_L08x08[r-1][i].inv * vec08;
                    GF2ToChar(vec08, cur);

                }

                // ============================
                // T
                unsigned char c = S_box[cur ^ round_keys[r][i]];

                // Ty
                Tyboxes[r][i][x] = Ty[i % 4][c];


                // MixingBijection
                tmpInt = Tyboxes[r][i][x];
                IntToGF2(tmpInt, vec32);
                vec32 = eMB_MB32x32[r][i/4].mb * vec32;
                GF2ToInt(vec32, Tyboxes[r][i][x]);

                // Internal Encoding after Tybox (Encode)
                mi.i = Tyboxes[r][i][x];
                for (int zz=0; zz< 4; ++zz){
                    mi.b[zz] = ((Int_Enc[(mi.b[zz]>>4)&0xF])<<4) | (Int_Enc[mi.b[zz]&0xF]);
                }

                Tyboxes[r][i][x] = mi.i;

                // ==================
                // MB（^-1)+L
                // Internal Encoding (Decode)
                cur = x;
                cur = ((Int_EncInverse[(x>>4)&0xF]) << 4) | (Int_EncInverse[x & 0xF]);

                // MB^(-1)
                tmpInt = (unsigned int) cur;
                tmpInt = tmpInt << 8*((i%4));
                IntToGF2(tmpInt, vec32);
                vec32 = eMB_MB32x32[r][i/4].inv * vec32;
                GF2ToInt(vec32, MixingBijection[r][i][x]);

                // L
                mi.i = MixingBijection[r][i][x];
                switch (i/4){
                    case 0:
                        tmpChar = mi.b[0];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][0].mb * vec08;
                        GF2ToChar(vec08, mi.b[0]);

                        tmpChar = mi.b[1];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][13].mb * vec08;
                        GF2ToChar(vec08, mi.b[1]);

                        tmpChar = mi.b[2];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][10].mb * vec08;
                        GF2ToChar(vec08, mi.b[2]);

                        tmpChar = mi.b[3];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][7].mb * vec08;
                        GF2ToChar(vec08, mi.b[3]);

                        for (int zz=0; zz< 4; ++zz){
                            mi.b[zz] = ((Int_Enc[(mi.b[zz]>>4)&0xF])<<4) | (Int_Enc[mi.b[zz]&0xF]);
                        }

                        MixingBijection[r][i][x] = mi.i;
                        break;
                    case 1:
                        tmpChar = mi.b[0];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][4].mb * vec08;
                        GF2ToChar(vec08, mi.b[0]);

                        tmpChar = mi.b[1];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][1].mb * vec08;
                        GF2ToChar(vec08, mi.b[1]);

                        tmpChar = mi.b[2];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][14].mb * vec08;
                        GF2ToChar(vec08, mi.b[2]);

                        tmpChar = mi.b[3];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][11].mb * vec08;
                        GF2ToChar(vec08, mi.b[3]);

                        for (int zz=0; zz< 4; ++zz){
                            mi.b[zz] = ((Int_Enc[(mi.b[zz]>>4)&0xF])<<4) | (Int_Enc[mi.b[zz]&0xF]);
                        }

                        MixingBijection[r][i][x] = mi.i;
                        break;
                    case 2:
                        tmpChar = mi.b[0];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][8].mb * vec08;
                        GF2ToChar(vec08, mi.b[0]);

                        tmpChar = mi.b[1];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][5].mb * vec08;
                        GF2ToChar(vec08, mi.b[1]);

                        tmpChar = mi.b[2];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][2].mb * vec08;
                        GF2ToChar(vec08, mi.b[2]);

                        tmpChar = mi.b[3];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][15].mb * vec08;
                        GF2ToChar(vec08, mi.b[3]);

                        for (int zz=0; zz< 4; ++zz){
                            mi.b[zz] = ((Int_Enc[(mi.b[zz]>>4)&0xF])<<4) | (Int_Enc[mi.b[zz]&0xF]);
                        }

                        MixingBijection[r][i][x] = mi.i;
                        break;
                    case 3:
                        tmpChar = mi.b[0];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][12].mb * vec08;
                        GF2ToChar(vec08, mi.b[0]);

                        tmpChar = mi.b[1];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][9].mb * vec08;
                        GF2ToChar(vec08, mi.b[1]);

                        tmpChar = mi.b[2];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][6].mb * vec08;
                        GF2ToChar(vec08, mi.b[2]);

                        tmpChar = mi.b[3];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][3].mb * vec08;
                        GF2ToChar(vec08, mi.b[3]);

                        for (int zz=0; zz< 4; ++zz){
                            mi.b[zz] = ((Int_Enc[(mi.b[zz]>>4)&0xF])<<4) | (Int_Enc[mi.b[zz]&0xF]);
                        }

                        MixingBijection[r][i][x] = mi.i;
                        break;
                }
            }   // End of x
        }
    }
    // Last round is a little bit different
    ShiftRows(round_keys[9]);
    for (size_t i = 0; i < 16; ++i) {
        for (size_t x = 0; x < 0x100; ++x) {
            unsigned char tmpChar;
            NTL::vec_GF2 vec08(INIT_SIZE, 8);
            unsigned char cur;

            cur = x;
            cur = Int_EncInverse[(x>>4)&0xF] << 4 | Int_EncInverse[x & 0xF];
            tmpChar = cur;
            CharToGF2(tmpChar, vec08);
            vec08 = eMB_L08x08[8][i].inv * vec08;
            GF2ToChar(vec08, cur);

            Tbox[i][x] = S_box[cur ^ round_keys[9][i]] ^ round_keys[10][i];
        }
    }


    // Tyboxes_decrypt
    InvShiftRows(round_keys[10]);
    for (size_t r = 0; r < 9; ++r) {
        InvShiftRows(round_keys[9-r]);
        for (size_t i = 0; i < 16; ++i) {
            for (size_t x = 0; x < 0x100; ++x) {

                unsigned int tmpInt;
                unsigned char tmpChar;
                NTL::vec_GF2 vec08(INIT_SIZE, 8);
                NTL::vec_GF2 vec32(INIT_SIZE, 32);
                magic_int mi;
                unsigned char cur;

                if (r == 0){
                    cur = x;

                    unsigned char c = Inv_S_box[cur ^ round_keys[10][i]] ^ round_keys[9][i];
                    Tyboxes_decrypt[r][i][x] = Ty_decrypt[i % 4][c];


                    // MixingBijection
                    tmpInt = Tyboxes_decrypt[r][i][x];
                    IntToGF2(tmpInt, vec32);
                    vec32 = eMB_MB32x32[r][i/4].mb * vec32;
                    GF2ToInt(vec32, Tyboxes_decrypt[r][i][x]);


                    // Encoding
                    mi.i = Tyboxes_decrypt[r][i][x];
                    for (int zz = 0; zz < 4; ++zz){
                        mi.b[zz] = ((Int_Enc[(mi.b[zz]>>4)&0xF])<<4) | (Int_Enc[mi.b[zz]&0xF]);
                    }
                    Tyboxes_decrypt[r][i][x] = mi.i;


                }
                else{
                    cur = x;

                    // L^(-1)
                    cur = ((Int_EncInverse[(x>>4)&0xF]) << 4) | (Int_EncInverse[x & 0xF]);

                    tmpChar = cur;
                    CharToGF2(tmpChar, vec08);
                    vec08 = eMB_L08x08[r-1][i].inv * vec08;
                    GF2ToChar(vec08, cur);


                    unsigned char c = Inv_S_box[cur] ^ round_keys[9-r][i];
                    Tyboxes_decrypt[r][i][x] = Ty_decrypt[i % 4][c];


                    // MixingBijection
                    tmpInt = Tyboxes_decrypt[r][i][x];
                    IntToGF2(tmpInt, vec32);
                    vec32 = eMB_MB32x32[r][i/4].mb * vec32;
                    GF2ToInt(vec32, Tyboxes_decrypt[r][i][x]);


                    // Encoding
                    mi.i = Tyboxes_decrypt[r][i][x];
                    for (int zz = 0; zz < 4; ++zz){
                        mi.b[zz] = ((Int_Enc[(mi.b[zz]>>4)&0xF])<<4) | (Int_Enc[mi.b[zz]&0xF]);
                    }
                    Tyboxes_decrypt[r][i][x] = mi.i;

                }


                // ==================
                // MB（^-1)+L
                // Internal Encoding (Decode)
                cur = x;
                cur = ((Int_EncInverse[(x>>4)&0xF]) << 4) | (Int_EncInverse[x & 0xF]);

                // MB^(-1)
                tmpInt = (unsigned int) cur;
                tmpInt = tmpInt << 8*((i%4));
                IntToGF2(tmpInt, vec32);
                vec32 = eMB_MB32x32[r][i/4].inv * vec32;
                GF2ToInt(vec32, MixingBijection_decrypt[r][i][x]);

                // L
                mi.i = MixingBijection_decrypt[r][i][x];
                switch (i/4){
                    case 0:
                        tmpChar = mi.b[0];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][0].mb * vec08;
                        GF2ToChar(vec08, mi.b[0]);

                        tmpChar = mi.b[1];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][5].mb * vec08;
                        GF2ToChar(vec08, mi.b[1]);

                        tmpChar = mi.b[2];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][10].mb * vec08;
                        GF2ToChar(vec08, mi.b[2]);

                        tmpChar = mi.b[3];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][15].mb * vec08;
                        GF2ToChar(vec08, mi.b[3]);

                        for (int zz=0; zz< 4; ++zz){
                            mi.b[zz] = ((Int_Enc[(mi.b[zz]>>4)&0xF])<<4) | (Int_Enc[mi.b[zz]&0xF]);
                        }

                        MixingBijection_decrypt[r][i][x] = mi.i;
                        break;
                    case 1:
                        tmpChar = mi.b[0];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][4].mb * vec08;
                        GF2ToChar(vec08, mi.b[0]);

                        tmpChar = mi.b[1];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][9].mb * vec08;
                        GF2ToChar(vec08, mi.b[1]);

                        tmpChar = mi.b[2];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][14].mb * vec08;
                        GF2ToChar(vec08, mi.b[2]);

                        tmpChar = mi.b[3];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][3].mb * vec08;
                        GF2ToChar(vec08, mi.b[3]);

                        for (int zz=0; zz< 4; ++zz){
                            mi.b[zz] = ((Int_Enc[(mi.b[zz]>>4)&0xF])<<4) | (Int_Enc[mi.b[zz]&0xF]);
                        }

                        MixingBijection_decrypt[r][i][x] = mi.i;
                        break;
                    case 2:
                        tmpChar = mi.b[0];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][8].mb * vec08;
                        GF2ToChar(vec08, mi.b[0]);

                        tmpChar = mi.b[1];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][13].mb * vec08;
                        GF2ToChar(vec08, mi.b[1]);

                        tmpChar = mi.b[2];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][2].mb * vec08;
                        GF2ToChar(vec08, mi.b[2]);

                        tmpChar = mi.b[3];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][7].mb * vec08;
                        GF2ToChar(vec08, mi.b[3]);

                        for (int zz=0; zz< 4; ++zz){
                            mi.b[zz] = ((Int_Enc[(mi.b[zz]>>4)&0xF])<<4) | (Int_Enc[mi.b[zz]&0xF]);
                        }

                        MixingBijection_decrypt[r][i][x] = mi.i;
                        break;
                    case 3:
                        tmpChar = mi.b[0];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][12].mb * vec08;
                        GF2ToChar(vec08, mi.b[0]);

                        tmpChar = mi.b[1];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][1].mb * vec08;
                        GF2ToChar(vec08, mi.b[1]);

                        tmpChar = mi.b[2];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][6].mb * vec08;
                        GF2ToChar(vec08, mi.b[2]);

                        tmpChar = mi.b[3];
                        CharToGF2(tmpChar, vec08);
                        vec08 = eMB_L08x08[r][11].mb * vec08;
                        GF2ToChar(vec08, mi.b[3]);

                        for (int zz=0; zz< 4; ++zz){
                            mi.b[zz] = ((Int_Enc[(mi.b[zz]>>4)&0xF])<<4) | (Int_Enc[mi.b[zz]&0xF]);
                        }

                        MixingBijection_decrypt[r][i][x] = mi.i;
                        break;
                }

            }
        }
    }
    // Last round is a little bit different
    InvShiftRows(round_keys[0]);
    for (size_t i = 0; i < 16; ++i) {
        for (size_t x = 0; x < 0x100; ++x) {
            unsigned char tmpChar;
            NTL::vec_GF2 vec08(INIT_SIZE, 8);
            unsigned char cur;

            cur = x;
            cur = Int_EncInverse[(x>>4)&0xF] << 4 | Int_EncInverse[x & 0xF];
            tmpChar = cur;
            CharToGF2(tmpChar, vec08);
            vec08 = eMB_L08x08[8][i].inv * vec08;
            GF2ToChar(vec08, cur);

            Tbox_decrypt[i][x] = Inv_S_box[cur] ^ round_keys[0][i];
        }
    }


    writeTable(path);
    LOGD("CPP==============", "generateTable() END");

}

tableGenerator::tableGenerator(){
    // Generate random key
    generate_key();

    memset(Tyboxes, 0x0, 9*16*0x100*sizeof(int));
    memset(Tyboxes_decrypt, 0x0, 9*16*0x100*sizeof(int));
    memset(Tbox, 0x0, 16*0x100*sizeof(char));
    memset(Tbox_decrypt, 0x0, 16*0x100*sizeof(char));
    memset(Txor, 0x0, 0x10*0x10*sizeof(char));

}
tableGenerator::~tableGenerator(){}