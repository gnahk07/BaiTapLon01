#ifndef RESIDENT_H
#define RESIDENT_H

//Định nghĩa cấu trúc cư dân
typedef struct Resident {

    char name[64];
    char CCCD[13];
    char year[5];
    char gender[5];
    char province[32];

} Resident;

//Kiểm tra có phải tất cả là số không
int isAllDigits(char *s);

//Kiểm tra mã tỉnh
int validProvince(int CCCD);

//Kiểm tra cccd có trùng với cccd nào khác không
int checkDuplicates(char CCCD[]);

//Kiểm tra căn cước công dân có hợp lệ không
int checkCCCD(char year[], char province[], char CCCD[], char gender[]);

//Bình thường hóa tên
void normalizeName(char name[]);

//Nhập thông tin cư dân
void inputResidentInformation();

#endif