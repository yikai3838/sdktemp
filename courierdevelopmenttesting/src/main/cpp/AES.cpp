#include <fstream>
#include <iostream>
#include "AES.h"
#include "com_aimazing_aimazinglib.h"

using namespace std;

void AES::readTable(bool flag){

    FILE *fp;
    std::string file_path;
    // Encrypt mode
    if (flag == true){
        file_path = path + "tables_encrypt.bin";
    }
        // Decrypt mode
    else{
        file_path = path + "tables_decrypt.bin";
    }

    fp=fopen(file_path.c_str(),"rb");

    if (!fp){
        printf("table_*.bin not existed!\n");
        return;
    }

    memset(Tyboxes, 0x0, 9*16*0x100*sizeof(unsigned int));
    //memset(Tyboxes_decrypt, 0x0, 9*16*0x100*sizeof(unsigned int));
    memset(Tbox, 0x0, 16*0x100*sizeof(unsigned char));
    //memset(Tbox_decrypt, 0x0, 16*0x100*sizeof(unsigned char));
    memset(Txor, 0x0, 0x10*0x10*sizeof(unsigned char));
    memset(MixingBijection, 0x0, 9*16*0x100*sizeof(unsigned int));
    //memset(MixingBijection_decrypt, 0x0, 9*16*0x100*sizeof(unsigned int));

    /*
    file.read(Tyboxes, 9*16*256*sizeof(unsigned int));
    file.read(Tyboxes_decrypt, 9*16*256*sizeof(unsigned int));
    file.read(Tbox, 16*256*sizeof(unsigned char));
    file.read(Tbox_decrypt, 16*256*sizeof(unsigned char));
    file.read(Txor, 16*16*sizeof(unsigned char));
    */

    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 16; ++j){
            for (int k = 0; k < 0x100; ++k){
                fread(&Tyboxes[i][j][k], sizeof(int), 1, fp);
            }
        }
    }
    for (int j = 0; j < 16; ++j){
        for (int k = 0; k < 0x100; ++k){
            fread(&Tbox[j][k], sizeof(char), 1, fp);
        }
    }
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 16; ++j){
            for (int k = 0; k < 0x100; ++k){
                fread(&MixingBijection[i][j][k], sizeof(int), 1, fp);
            }
        }
    }
    /*
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 16; ++j){
            for (int k = 0; k < 0x100; ++k){
                fread(&Tyboxes_decrypt[i][j][k], sizeof(int), 1, fpDec);
            }
        }
    }
    for (int j = 0; j < 16; ++j){
        for (int k = 0; k < 0x100; ++k){
            fread(&Tbox_decrypt[j][k], sizeof(char), 1, fpDec);
        }
    }
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 16; ++j){
            for (int k = 0; k < 0x100; ++k){
                fread(&MixingBijection_decrypt[i][j][k], sizeof(int), 1, fpDec);
            }
        }
    }
    */
    for (int i = 0; i < 0x10; ++i){
        for (int j = 0; j < 0x10; ++j){
            fread(&Txor[i][j], sizeof(char), 1, fp);
            //fread(&Txor[i][j], sizeof(char), 1, fpDec);
        }
    }

    fclose(fp);

}

void AES::ShiftRows(unsigned char cipher[16]){
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

void AES::InvShiftRows(unsigned char plain[16]){
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

std::string AES::encrypt(std::string data)
{
    readTable(true);
    // =================
    size_t length = data.length();
    if (length % 16 != 0 ){
        length = ((length/16) + 1) * 16;
    }
    unsigned char *cstr = new unsigned char[length+1];
    memset(cstr, '\0', length+1);
    memcpy(cstr, data.c_str(), length+1);

    LOGD("CPP--ENCRYPT CSTR_S", "%s", cstr);
    LOGD("CPP--ENCRYPT CSTR_X: ", "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",cstr[0],cstr[1],cstr[2],cstr[3],cstr[4],cstr[5],cstr[6],cstr[7],cstr[8],cstr[9],cstr[10],cstr[11],cstr[12],cstr[13],cstr[14],cstr[15],cstr[16],cstr[17],cstr[18],cstr[19],cstr[20],cstr[21],cstr[22],cstr[23],cstr[24],cstr[25],cstr[26],cstr[27],cstr[28],cstr[29],cstr[30],cstr[31]);
    LOGD("CPP--ENCRYPT CSTR_C: ", "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",cstr[0],cstr[1],cstr[2],cstr[3],cstr[4],cstr[5],cstr[6],cstr[7],cstr[8],cstr[9],cstr[10],cstr[11],cstr[12],cstr[13],cstr[14],cstr[15],cstr[16],cstr[17],cstr[18],cstr[19],cstr[20],cstr[21],cstr[22],cstr[23],cstr[24],cstr[25],cstr[26],cstr[27],cstr[28],cstr[29],cstr[30],cstr[31]);

    unsigned char cipher[16];
    /// Let's start the encryption process now
    for (int counter = 0; counter < length; counter += 16){
        memset(cipher, '\0', 16);
        if (counter + 16 > length)
            memcpy(cipher, &cstr[counter], length%16);
        else
            memcpy(cipher, &cstr[counter], 16);

        /*
        LOGD("CPP--ENCRYPT ING_S: ", "%d-%d %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",counter,counter+15,cipher[0],cipher[1],cipher[2],cipher[3],cipher[4],cipher[5],cipher[6],cipher[7],cipher[8],cipher[9],cipher[10],cipher[11],cipher[12],cipher[13],cipher[14],cipher[15]);
        LOGD("CPP--ENCRYPT ING_X: ", "%d-%d %x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",counter,counter+15,cipher[0],cipher[1],cipher[2],cipher[3],cipher[4],cipher[5],cipher[6],cipher[7],cipher[8],cipher[9],cipher[10],cipher[11],cipher[12],cipher[13],cipher[14],cipher[15]);
        LOGD("CPP--", "00 round: ");
        for (int z = 0; z < 16; ++z)
            LOGD("CPP--", "%02x",cipher[z]);
        */

        for (size_t i = 0; i < 9; ++i) {
            ShiftRows(cipher);
            for (size_t j = 0; j < 4; ++j) {

                unsigned int aa = Tyboxes[i][j * 4 + 0][cipher[j * 4 + 0]];
                unsigned int bb = Tyboxes[i][j * 4 + 1][cipher[j * 4 + 1]];
                unsigned int cc = Tyboxes[i][j * 4 + 2][cipher[j * 4 + 2]];
                unsigned int dd = Tyboxes[i][j * 4 + 3][cipher[j * 4 + 3]];

                cipher[j * 4 + 0] = (Txor[Txor[(aa >>  0) & 0xf][(bb >>  0) & 0xf]][Txor[(cc >>  0) & 0xf][(dd >>  0) & 0xf]]) | ((Txor[Txor[(aa >>  4) & 0xf][(bb >>  4) & 0xf]][Txor[(cc >>  4) & 0xf][(dd >>  4) & 0xf]]) << 4);
                cipher[j * 4 + 1] = (Txor[Txor[(aa >>  8) & 0xf][(bb >>  8) & 0xf]][Txor[(cc >>  8) & 0xf][(dd >>  8) & 0xf]]) | ((Txor[Txor[(aa >> 12) & 0xf][(bb >> 12) & 0xf]][Txor[(cc >> 12) & 0xf][(dd >> 12) & 0xf]]) << 4);
                cipher[j * 4 + 2] = (Txor[Txor[(aa >> 16) & 0xf][(bb >> 16) & 0xf]][Txor[(cc >> 16) & 0xf][(dd >> 16) & 0xf]]) | ((Txor[Txor[(aa >> 20) & 0xf][(bb >> 20) & 0xf]][Txor[(cc >> 20) & 0xf][(dd >> 20) & 0xf]]) << 4);
                cipher[j * 4 + 3] = (Txor[Txor[(aa >> 24) & 0xf][(bb >> 24) & 0xf]][Txor[(cc >> 24) & 0xf][(dd >> 24) & 0xf]]) | ((Txor[Txor[(aa >> 28) & 0xf][(bb >> 28) & 0xf]][Txor[(cc >> 28) & 0xf][(dd >> 28) & 0xf]]) << 4);

                aa = MixingBijection[i][j * 4 + 0][cipher[j * 4 + 0]];
                bb = MixingBijection[i][j * 4 + 1][cipher[j * 4 + 1]];
                cc = MixingBijection[i][j * 4 + 2][cipher[j * 4 + 2]];
                dd = MixingBijection[i][j * 4 + 3][cipher[j * 4 + 3]];

                cipher[j * 4 + 0] = (Txor[Txor[(aa >>  0) & 0xf][(bb >>  0) & 0xf]][Txor[(cc >>  0) & 0xf][(dd >>  0) & 0xf]]) | ((Txor[Txor[(aa >>  4) & 0xf][(bb >>  4) & 0xf]][Txor[(cc >>  4) & 0xf][(dd >>  4) & 0xf]]) << 4);
                cipher[j * 4 + 1] = (Txor[Txor[(aa >>  8) & 0xf][(bb >>  8) & 0xf]][Txor[(cc >>  8) & 0xf][(dd >>  8) & 0xf]]) | ((Txor[Txor[(aa >> 12) & 0xf][(bb >> 12) & 0xf]][Txor[(cc >> 12) & 0xf][(dd >> 12) & 0xf]]) << 4);
                cipher[j * 4 + 2] = (Txor[Txor[(aa >> 16) & 0xf][(bb >> 16) & 0xf]][Txor[(cc >> 16) & 0xf][(dd >> 16) & 0xf]]) | ((Txor[Txor[(aa >> 20) & 0xf][(bb >> 20) & 0xf]][Txor[(cc >> 20) & 0xf][(dd >> 20) & 0xf]]) << 4);
                cipher[j * 4 + 3] = (Txor[Txor[(aa >> 24) & 0xf][(bb >> 24) & 0xf]][Txor[(cc >> 24) & 0xf][(dd >> 24) & 0xf]]) | ((Txor[Txor[(aa >> 28) & 0xf][(bb >> 28) & 0xf]][Txor[(cc >> 28) & 0xf][(dd >> 28) & 0xf]]) << 4);

            }

            /*
            LOGD("CPP--", "%02zu round: ", i+1);
            for (int z = 0; z < 16; ++z)
                LOGD("CPP--", "%02x ", cipher[z]);
            */
        }

        /// Last round which is a bit different
        ShiftRows(cipher);
        for (size_t j = 0; j < 16; ++j) {
            unsigned char x = Tbox[j][cipher[j]];
            cipher[j] = x;
        }
        /*
        LOGD("CPP--", "10 round: ");
        for (int z = 0; z < 16; ++z)
            LOGD("CPP--", "%02x ", cipher[z]);
        */
        /*
        if (counter + 16 > length)
            memcpy(&cstr[counter], cipher, length%16);
        else
        */
        memcpy(&cstr[counter], cipher, 16);


        LOGD("CPP--ENCRYPT END: ", "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",cipher[0],cipher[1],cipher[2],cipher[3],cipher[4],cipher[5],cipher[6],cipher[7],cipher[8],cipher[9],cipher[10],cipher[11],cipher[12],cipher[13],cipher[14],cipher[15]);
        LOGD("CPP--ENCRYPT END: ", "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",cstr[0],cstr[1],cstr[2],cstr[3],cstr[4],cstr[5],cstr[6],cstr[7],cstr[8],cstr[9],cstr[10],cstr[11],cstr[12],cstr[13],cstr[14],cstr[15],cstr[16],cstr[17],cstr[18],cstr[19],cstr[20],cstr[21],cstr[22],cstr[23],cstr[24],cstr[25],cstr[26],cstr[27],cstr[28],cstr[29],cstr[30],cstr[31]);
    }

    std::stringstream ss;
    string str;

    /*
    for(int i=0; i<length; ++i)
        ss << setfill('0') << std::setw(2) << std::hex << (int)cstr[i];
    */
    for (int i=0; i<length; ++i){
        ss << setfill('0') << std::setw(2) << std::hex << (int)cstr[i];
    }
    str = ss.str();

    LOGD("CPP--ENCRYPT END: ", "%s",str.c_str());


    delete cstr;
    return str;
}


std::string AES::decrypt(std::string data)
{
    readTable(false);
    // =================
    size_t length = data.length();
    LOGD("CPP--DECRYPT start: ", "%d", length);

    std::stringstream ss;
    char result;
    for (int i = 0; i < length; i+= 2){
        result = (data[i] <= '9' ? data[i] - '0' : toupper(data[i]) - 'A' + 10) << 4;
        result |= data[i+1] <= '9' ? data[i+1] - '0' : toupper(data[i+1]) - 'A' + 10;
        ss << result;
    }
    length = length / 2;
    string final_data = ss.str();
    unsigned char *cstr = new unsigned char[length+1];
    memcpy(cstr, final_data.c_str(), length);

    LOGD("CPP--DECRYPT start: ", "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",cstr[0],cstr[1],cstr[2],cstr[3],cstr[4],cstr[5],cstr[6],cstr[7],cstr[8],cstr[9],cstr[10],cstr[11],cstr[12],cstr[13],cstr[14],cstr[15],cstr[16],cstr[17],cstr[18],cstr[19],cstr[20],cstr[21],cstr[22],cstr[23],cstr[24],cstr[25],cstr[26],cstr[27],cstr[28],cstr[29],cstr[30],cstr[31]);
    unsigned char plain[16];

    for (int counter = 0; counter < length; counter += 16){
        memset(plain, '\0', 16);
        if (counter + 16 > length)
            memcpy(plain, &cstr[counter], length%16);
        else
            memcpy(plain, &cstr[counter], 16);

        for (size_t i = 0; i < 9; ++i) {
            InvShiftRows(plain);
            for (size_t j = 0; j < 4; ++j) {

                unsigned int aa = Tyboxes[i][j * 4 + 0][plain[j * 4 + 0]];
                unsigned int bb = Tyboxes[i][j * 4 + 1][plain[j * 4 + 1]];
                unsigned int cc = Tyboxes[i][j * 4 + 2][plain[j * 4 + 2]];
                unsigned int dd = Tyboxes[i][j * 4 + 3][plain[j * 4 + 3]];

                plain[j * 4 + 0] = (Txor[Txor[(aa >>  0) & 0xf][(bb >>  0) & 0xf]][Txor[(cc >>  0) & 0xf][(dd >>  0) & 0xf]]) | ((Txor[Txor[(aa >>  4) & 0xf][(bb >>  4) & 0xf]][Txor[(cc >>  4) & 0xf][(dd >>  4) & 0xf]]) << 4);
                plain[j * 4 + 1] = (Txor[Txor[(aa >>  8) & 0xf][(bb >>  8) & 0xf]][Txor[(cc >>  8) & 0xf][(dd >>  8) & 0xf]]) | ((Txor[Txor[(aa >> 12) & 0xf][(bb >> 12) & 0xf]][Txor[(cc >> 12) & 0xf][(dd >> 12) & 0xf]]) << 4);
                plain[j * 4 + 2] = (Txor[Txor[(aa >> 16) & 0xf][(bb >> 16) & 0xf]][Txor[(cc >> 16) & 0xf][(dd >> 16) & 0xf]]) | ((Txor[Txor[(aa >> 20) & 0xf][(bb >> 20) & 0xf]][Txor[(cc >> 20) & 0xf][(dd >> 20) & 0xf]]) << 4);
                plain[j * 4 + 3] = (Txor[Txor[(aa >> 24) & 0xf][(bb >> 24) & 0xf]][Txor[(cc >> 24) & 0xf][(dd >> 24) & 0xf]]) | ((Txor[Txor[(aa >> 28) & 0xf][(bb >> 28) & 0xf]][Txor[(cc >> 28) & 0xf][(dd >> 28) & 0xf]]) << 4);

                aa = MixingBijection[i][j * 4 + 0][plain[j * 4 + 0]];
                bb = MixingBijection[i][j * 4 + 1][plain[j * 4 + 1]];
                cc = MixingBijection[i][j * 4 + 2][plain[j * 4 + 2]];
                dd = MixingBijection[i][j * 4 + 3][plain[j * 4 + 3]];

                plain[j * 4 + 0] = (Txor[Txor[(aa >>  0) & 0xf][(bb >>  0) & 0xf]][Txor[(cc >>  0) & 0xf][(dd >>  0) & 0xf]]) | ((Txor[Txor[(aa >>  4) & 0xf][(bb >>  4) & 0xf]][Txor[(cc >>  4) & 0xf][(dd >>  4) & 0xf]]) << 4);
                plain[j * 4 + 1] = (Txor[Txor[(aa >>  8) & 0xf][(bb >>  8) & 0xf]][Txor[(cc >>  8) & 0xf][(dd >>  8) & 0xf]]) | ((Txor[Txor[(aa >> 12) & 0xf][(bb >> 12) & 0xf]][Txor[(cc >> 12) & 0xf][(dd >> 12) & 0xf]]) << 4);
                plain[j * 4 + 2] = (Txor[Txor[(aa >> 16) & 0xf][(bb >> 16) & 0xf]][Txor[(cc >> 16) & 0xf][(dd >> 16) & 0xf]]) | ((Txor[Txor[(aa >> 20) & 0xf][(bb >> 20) & 0xf]][Txor[(cc >> 20) & 0xf][(dd >> 20) & 0xf]]) << 4);
                plain[j * 4 + 3] = (Txor[Txor[(aa >> 24) & 0xf][(bb >> 24) & 0xf]][Txor[(cc >> 24) & 0xf][(dd >> 24) & 0xf]]) | ((Txor[Txor[(aa >> 28) & 0xf][(bb >> 28) & 0xf]][Txor[(cc >> 28) & 0xf][(dd >> 28) & 0xf]]) << 4);

            }

        }
        /// First round which is a bit different
        InvShiftRows(plain);
        for (size_t j = 0; j < 16; ++j) {
            unsigned char x = Tbox[j][plain[j]];
            plain[j] = x;
        }

        if (counter + 16 > length)
            memcpy(&cstr[counter], plain, length%16);
        else
            memcpy(&cstr[counter], plain, 16);


        LOGD("CPP--DECRYPT END: ", "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",plain[0],plain[1],plain[2],plain[3],plain[4],plain[5],plain[6],plain[7],plain[8],plain[9],plain[10],plain[11],plain[12],plain[13],plain[14],plain[15]);
        LOGD("CPP--DECRYPT END: ", "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",cstr[0],cstr[1],cstr[2],cstr[3],cstr[4],cstr[5],cstr[6],cstr[7],cstr[8],cstr[9],cstr[10],cstr[11],cstr[12],cstr[13],cstr[14],cstr[15],cstr[16],cstr[17],cstr[18],cstr[19],cstr[20],cstr[21],cstr[22],cstr[23],cstr[24],cstr[25],cstr[26],cstr[27],cstr[28],cstr[29],cstr[30],cstr[31]);
    }

    std:stringstream ss2;
    for (int i = 0; i < length; i++){
        ss2 << cstr[i];
    }
    string str = ss2.str();




    delete cstr;
    return str;

}

AES::AES(std::string _path){
    path = _path;
}

AES::~AES(){}