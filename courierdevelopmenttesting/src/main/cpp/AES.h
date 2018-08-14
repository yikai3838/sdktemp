#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>

class AES{

public:

    std::string encrypt(std::string data);
    std::string decrypt(std::string data);
    AES(std::string path);
    ~AES();

private:
    unsigned int Tyboxes[9][16][0x100];
    //unsigned int Tyboxes_decrypt[9][16][0x100];
    unsigned char Tbox[16][0x100];
    //unsigned char Tbox_decrypt[16][0x100];
    unsigned char Txor[0x10][0x10];
    unsigned int MixingBijection[9][16][0x100];
    //unsigned int MixingBijection_decrypt[9][16][0x100];
    std::string path;

    void readTable(bool flag);
    void ShiftRows(unsigned char cipher[16]);
    void InvShiftRows(unsigned char plain[16]);

};