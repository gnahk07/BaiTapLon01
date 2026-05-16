//Thư viện sử dụng
#include "Resident.h"
#include "Room.h"
#include "Floor.h"
#include "Utils.h"
#include <stdio.h>      // printf, fgets, sscanf, fopen, fclose, fprintf
#include <string.h>     // strlen, strcmp, strcpy, strcspn
#include <ctype.h>      // isdigit, tolower, toupper
#include <stdlib.h>     // malloc, free
#include <dirent.h>     // DIR, opendir, closedir
#include <direct.h>     // _mkdir (Windows)
#include <windows.h>    // Sleep

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

//Kiểm tra cccd có trùng với cccd nào khác không
int checkDuplicates(char CCCD[]) {

    //Mở file chứa CCCD đã có trong trung cư
    FILE *f = fopen("Data/cccd.txt", "r");
    if (f == NULL) {
        printf("Khong mo duoc file!\n");
        return 0;
    }

    //Đọc từng dòng trong file
    char cccdInFile[13];
    while (fgets(cccdInFile, sizeof(cccdInFile), f) != NULL) {

        //Xóa ký tự xuống dòng
        cccdInFile[strcspn(cccdInFile, "\n")] = '\0';

        //So sánh có trùng không
        if (strcmp(CCCD, cccdInFile) == 0) {
            fclose(f);
            return 0;
        }
    }

    //Đóng file chứa CCCD đã có trong trung cư
    fclose(f);
    return 1;
}

//Kiểm tra căn cước công dân có hợp lệ không
int checkCCCD(char year[], char province[], char CCCD[], char gender[]) {

    //Kiểm tra có đủ 12 ký tự không
    if (strlen(CCCD) != 12) {
        setColor(12);
        printf("[Lỗi]. ");
        setColor(7);
        printf("-> Nhập lại CCCD có 12 số: ");
        return 0;
    }
    //Kiểm tra có phải tất cả là số không
    if (!isAllDigits(CCCD)) {
        setColor(12);
        printf("[Lỗi]. ");
        setColor(7);
        printf("-> Nhập lại CCCD chỉ toàn là số: ");
        return 0;
    }

    //Kiểm tra CCCD có trùng với ai không
    if (!checkDuplicates(CCCD)) {
        setColor(12);
        printf("[Lỗi]. ");
        setColor(7);
        printf("-> Nhập lại CCCD vì đã bị trùng: ");
        return 0;
    }

    //Kiểm tra mã tỉnh
    int Province = (CCCD[0] - '0') * 100 + (CCCD[1] - '0') * 10 + (CCCD[2] - '0');
    if (!validProvince(Province)) {
        setColor(12);
        printf("[Lỗi]. ");
        setColor(7);
        printf("-> Nhập lại CCCD: ");
        return 0;
    }
    //Lưu quê quán
    char path[256];
    sprintf(path, "Data/Province/%03d.txt", Province);
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
    if (Gender < 0 || Gender > 3) {
        printf("Nhập lại CCCD: ");
        return 0;
    }
    if (Gender % 2 == 0) strcpy(gender, "Nam");
    else strcpy(gender, "Nữ");

    //Kiểm tra năm sinh
    int Year = (CCCD[4] - '0') * 10 + (CCCD[5] - '0');
    if (Gender == 0 || Gender == 1) sprintf(year, "19%d", Year);
    if (Gender == 2 || Gender == 3) sprintf(year, "20%d", Year);

    return 1;
}

//Bình thường hóa tên
void normalizeName(char name[]) {
    //Chuyển toàn bộ thành chữ thường
    for (int i = 0; name[i]; i++)
        name[i] = tolower((unsigned char)name[i]);

    //In hoa chữ đầu tiên
    if (name[0] != '\0')
        name[0] = toupper((unsigned char)name[0]);

    //In hoa các chữ cái sau dấu cách
    for (int i = 1; name[i]; i++)
        if (name[i-1] == ' ' && name[i] != ' ')
            name[i] = toupper((unsigned char)name[i]);
}

//Nhập thông tin cư dân
void inputResidentInformation() {
    
    //Chọn tầng và phòng muốn thêm
    int selectFloor, selectRoom, n;
    int floorCount = 0;
    char check[10];
    countFloors(&floorCount);
    openFloorList(floorCount);
    printf("\n");
    while (1) {
        printf("-> Chọn tầng muốn thêm: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &selectFloor)) break;
    }
    displayRoom(selectFloor);
    while (1) {
        printf("\n-> Chọn phòng muốn thêm: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &selectRoom)) break;
    }

    //Mở folder chứa phòng
    char roomPath[256];
    sprintf(roomPath, "FloorList/Tang_%d/P%d%02d", selectFloor, selectFloor, selectRoom);
    DIR *dp = opendir(roomPath);

    //Kiểm tra phòng có tồn tại không nếu không thì thoát
    if (dp == NULL) {
        closedir(dp);
        printf("Phòng không tồn tại.");
        return;
    }

    //Nhập số lượng thành viên và kiểm tra
    while (1) {
        printf("-> Nhập số lượng muốn thêm: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &n)) break;
    }
    
    //Nếu n <= 0
    if (n <= 0) {
        setColor(12);
        printf("[Lỗi]. Số lượng không hợp lệ. ");
        setColor(7);
        return;
    }

    //Khai báo hàm cư dân
    Resident *resident = malloc(n * sizeof(Resident));

    //Tạo vòng lặp để nhập thông tin từng cư dân
    for (int i = 0; i < n; i++) {

        //Nhập tên cư dân
        printf("-> Nhập tên người thứ %d: ", i + 1);
        fgets(resident[i].name, sizeof(resident[i].name), stdin);
        resident[i].name[strcspn(resident[i].name, "\n")] = '\0';

        //Nhập căn cước công dân và rút ra năm sinh, quê quán, giới tính
        printf("-> Nhập CCCD: ");
        while (1) {
            fgets(resident[i].CCCD, sizeof(resident[i].CCCD), stdin);
            resident[i].CCCD[strcspn(resident[i].CCCD, "\n")] = '\0';
            if (checkCCCD(resident[i].year, resident[i].province, resident[i].CCCD, resident[i].gender)) break;
            while(getchar() != '\n');
        }

        //Chỉnh sửa tên. Chữ cái đầu viết thường, các chữ sau viết hoa
        normalizeName(resident[i].name);

        //Tạo đường dẫn đầy đủ để tạo file chứa thông tin cư dân
        char residentPath[256];
        sprintf(residentPath, "%s/%s.txt", roomPath, resident[i].CCCD);

        //Tạo file .txt để lưu thông tin của cư dân
        FILE *f1 = fopen (residentPath, "w");

        //Kiểm tra có tạo được file không
        if (f1 == NULL) {
            setColor(12);
            printf("[Lỗi]. Không tạo được File để chứa cư dân\n");
            setColor(7);
            return;
        }
        
        //Lưu thông tin cư dân
        fprintf(f1, "%s\n%s\n%s\n%s", resident[i].name, resident[i].year, resident[i].province, resident[i].gender);

        //Đóng file
        fclose (f1);

        //Lưu CCCD vào danh sách
        FILE *f2 = fopen ("Data/cccd.txt", "a");
        fprintf(f2, "%s\n", resident[i].CCCD);
        fclose (f2);

        //Xóa buffer
        while (getchar() != '\n');

        setColor(10);
        printf("Đã lưu thông tin cư dân thứ %d.\n", i + 1);
        setColor(7);
    }

    //Giải phóng bộ nhớ
    free(resident);
    setColor(10);
    printf("Done.");
    setColor(7);
}