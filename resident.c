#include <stdio.h>
#include <string.h>
#include "resident.h"

//Kiểm tra có phải tất cả là số không
int isAllDigits(char *s) {
    for (int i = 0; s[i]; i++) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}

//Kiểm tra mã tỉnh
int validProvince(int CCCD) {

    //Mã tỉnh hợp lệ
    int provinces[] = {
        1, 2, 4, 6, 8, 10, 11, 12, 14, 15,
        17, 19, 20, 22, 24, 25, 26, 27, 30,
        31, 33, 34, 35, 36, 37, 38, 40, 42,
        44, 45, 46, 48, 49, 51, 52, 54, 56,
        58, 60, 62, 64, 66, 67, 68, 70, 72,
        74, 75, 77, 79, 80, 82, 83, 84, 86,
        87, 89, 91, 92, 93, 94, 95, 96
    };

    //Lấy số lượng mã tỉnh
    int n = sizeof (provinces)/sizeof(provinces[0]);

    //Kiểm tra mã có hợp lệ không
    for (int i = 0; i < n; i++) {
        if (CCCD == provinces[i]) return 1;
    }
    return 0;
}

//Kiểm tra căn cước công dân có hợp lệ không
int checkCCCD(char CCCD[], char province[], char year[], char gender[]) {

    //Kiểm tra có đủ 12 ký tự không
    if (strlen(CCCD) != 12) return 0;

    //Kiểm tra có phải tất cả là số không
    if (!isAllDigits(CCCD)) return 0;

    //Kiểm tra có trùng với số CCCD nào khác không

    //Kiểm tra mã tỉnh
    int Province = (CCCD[0] - '0') * 100 + (CCCD[1] - '0') * 10 + (CCCD[2] - '0');
    if (!validProvince(Province)) return 0;
    
    //Lưu quê quán
    char path[256];
    sprintf(path, "Province/%d", Province);
    FILE *f = fopen (path, "r");
    if (f == NULL) {
        strcpy(province, "Không tìm thấy quê quán");
    } else {
        fgets(province, 32, f);
        province[strcspn(province, "\n")] = '\0';
        fclose(f);
    }

    //Kiểm tra giới tính
    int Gender = CCCD[3] - '0';
    if (Gender < 0 || Gender > 3) return 0;
    if (Gender / 2 == 0) strcpy(gender, "Nam");
    else strcpy(gender, "Nữ");

    //Kiểm tra năm sinh
    int Year = (CCCD[4] - '0') * 10 + (CCCD[5] - '0');
    if (Gender == 0 || Gender == 1) sprintf(year, "19%d", Year);
    if (Gender == 2 || Gender == 3) sprintf(year, "20%d", Year);

    return 1;
}

//Nhập thông tin cư dân
void inputResidentInformation() {
    
    //Nhập số lượng thành viên và kiểm tra
    int n;
    char check[10];
    while (1) {
        printf("Nhập số lượng thành viên trong gia đình [1, 5]: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &n) == 1 && n >= 1 && n <= 5) break;
    }

    //Khai báo hàm cư dân
    Resident resident[n];

    //Nhập tên cư dân
    getchar();
    printf("Nhập tên người thứ %d: ", i + 1);
    fgets(resident[i].name, sizeof(resident[i].name), stdin);
    resident[i].name[strcspn(resident[i].name, "\n")] = '\0';

    //Nhập căn cước công dân và rút ra năm sinh, quê quán, giới tính
    while (1) {
        printf("Nhập CCCD (12 số): ");
        fgets(resident[i].CCCD, sizeof(resident[i].CCCD), stdin);
        resident[i].CCCD[strcspn(resident[i].CCCD, "\n")] = '\0';
        if (checkCCCD(resident[i].year, resident[i].province, resident[i].CCCD, resident[i].gender)) break;
    }
}

