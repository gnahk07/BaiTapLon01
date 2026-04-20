#ifndef RESIDENT_H
#define RESIDENT_H

//Định nghĩa cấu trúc cư dân
typedef struct {
    char name[64];
    char CCCD[13];
} Resident;

//Kiểm tra có phải tất cả là số không
int isAllDigits(char *s);

//Kiểm tra mã tỉnh
int validProvince(int CCCD);

//Kiểm tra căn cước công dân có hợp lệ không
int checkCCCD(char CCCD[], char province[], char year[], char gender[]);

//Nhập thông tin cư dân
void inputResidentInformation();

#endif