//Thư viện sử dụng
#include "Floor.h"
#include "Room.h"
#include "Utils.h"
#include "Menu.h"
#include "Resident.h"
#include "Delete.h"
#include <stdio.h>      // printf, sprintf, fprintf, fopen, fclose, fgets, sscanf, rename, remove
#include <string.h>     // strlen, strcmp, strcpy, strncpy, strcspn
#include <ctype.h>      // isdigit
#include <dirent.h>     // DIR, struct dirent, opendir, readdir, closedir

//Kiểm tra chuỗi có phải toàn là số không
int isAllDigitsEdit(const char *s) {
    if (s[0] == '\0') return 0;
    for (int i = 0; s[i]; i++) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

// ============================================================
//  Kiểm tra CCCD có bị trùng không
//  - Nếu CCCD trùng với chính nó (khi sửa) thì bỏ qua
// ============================================================
int checkDuplicatesEdit(const char *CCCD, const char *skipCCCD) {
    FILE *f = fopen("Data/cccd.txt", "r");
    if (f == NULL) return 1; // Không mở được file thì cho qua

    char line[16];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';

        // Bo qua chinh CCCD dang duoc sua
        if (skipCCCD != NULL && strcmp(line, skipCCCD) == 0) continue;

        if (strcmp(line, CCCD) == 0) {
            fclose(f);
            return 0; // Bị trùng
        }
    }
    fclose(f);
    return 1; // Không trùng
}

//  Kiểm tra CCCD 
int checkCCCDEdit(Resident *r, const char *skipCCCD) {

    // Kiểm tra độ dài
    if (strlen(r->CCCD) != 12) return 0;

    // Kiểm tra tất cả là số
    if (!isAllDigitsEdit(r->CCCD)) return 0;

    // Kiểm tra trùng lặp
    if (!checkDuplicatesEdit(r->CCCD, skipCCCD)) return 0;

    // Lấy mã tỉnh (3 chữ số đầu)
    int provinceCode = (r->CCCD[0]-'0')*100 + (r->CCCD[1]-'0')*10 + (r->CCCD[2]-'0');
    if (!validProvince(provinceCode)) return 0;

    // Đọc tên tỉnh từ file
    char path[256];
    sprintf(path, "Data/Province/%03d.txt", provinceCode);
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        strcpy(r->province, "Không tìm thấy quê quán");
    } else {
        fgets(r->province, 32, f);
        r->province[strcspn(r->province, "\n")] = '\0';
        fclose(f);
    }

    // Lấy giới tính (chữ số thứ 4)
    int genderDigit = r->CCCD[3] - '0';
    if (genderDigit < 0 || genderDigit > 3) return 0;
    if (genderDigit % 2 == 0) strcpy(r->gender, "Nam");
    else strcpy(r->gender, "Nữ");

    // Lấy năm sinh (chữ số thứ 5-6)
    int yearVal = (r->CCCD[4]-'0')*10 + (r->CCCD[5]-'0');
    if (genderDigit == 0 || genderDigit == 1)
        sprintf(r->year, "19%02d", yearVal);
    else
        sprintf(r->year, "20%02d", yearVal);

    return 1;
}


//  Cập nhật CCCD trong Data/cccd.txt (đổi CCCD cũ thành mới)
void updateCCCDFile(const char *oldCCCD, const char *newCCCD) {
    FILE *f = fopen("Data/cccd.txt", "r");
    if (f == NULL) return;

    FILE *temp = fopen("Data/cccd_temp.txt", "w");
    if (temp == NULL) { fclose(f); return; }

    char line[16];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, oldCCCD) == 0)
            fprintf(temp, "%s\n", newCCCD);
        else
            fprintf(temp, "%s\n", line);
    }
    fclose(f);
    fclose(temp);

    remove("Data/cccd.txt");
    rename("Data/cccd_temp.txt", "Data/cccd.txt");
}

// ============================================================
//  Đọc thông tin cư dân từ file 
// ============================================================
int loadResident(Resident *r, const char *roomPath) {
    char filePath[512];
    sprintf(filePath, "%s/%s.txt", roomPath, r->CCCD);

    FILE *f = fopen(filePath, "r");
    if (f == NULL) return 0;

    fgets(r->name,     64,     f); r->name[strcspn(r->name, "\n")]         = '\0';
    fgets(r->year,     64,     f); r->year[strcspn(r->year, "\n")]         = '\0';
    fgets(r->province, 64,     f); r->province[strcspn(r->province, "\n")] = '\0';
    fgets(r->gender,   64,     f); r->gender[strcspn(r->gender, "\n")]     = '\0';
    
    fclose(f);
    return 1;
}

// ============================================================
//  Ghi thông tin cư dân vào file
// ============================================================
void saveResident(const Resident *r, const char *roomPath) {
    char filePath[512];
    sprintf(filePath, "%s/%s.txt", roomPath, r->CCCD);

    FILE *f = fopen(filePath, "w");
    if (f == NULL) {
        setColor(12);
        printf("[Lỗi]. ");
        printf("Không thể ghi file\n");
        return;
    }
    fprintf(f, "%s\n%s\n%s\n%s", r->name, r->year, r->province, r->gender);
    fclose(f);
}

// ============================================================
//  Chỉnh sửa thông tin cư dân
// ============================================================
void editResident(const char *roomPath, int floor, int room) {

    // Hiển thị danh sách cư dân trong phòng
    if (printResidentCCCDAndName(roomPath, floor, room) == 0) return;

    // Buffer nhập liệu chung
    char input[32];

    // Nhập CCCD của cư dân muốn sửa
    Resident old;
    printf("-> Nhập CCCD muốn sửa: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    strncpy(old.CCCD, input, 13);
    old.CCCD[12] = '\0';

    // Kiểm file cư dân có tồn tại không
    if (!loadResident(&old, roomPath)) {
        setColor(12);
        printf("[Lỗi]. ");
        setColor(7);
        printf("Không tìm thấy cư dân với CCCD là: %s\n", old.CCCD);
        return;
    }

    //Tạo path
    char namePath[64], cccdPath[64], yearPath[64], provincePath[64], genderPath[64];
    sprintf(namePath, "Họ tên   : %s", old.name);
    sprintf(cccdPath, "CCCD     : %s", old.CCCD);
    sprintf(yearPath, "Năm sinh : %s", old.year);
    sprintf(provincePath, "Quê quán : %s", old.province);
    sprintf(genderPath, "Giới tính: %s", old.gender);

    //Hiện thị thông tin hiện tại
    downLine();
    drawBorderTop();
    drawRow(namePath);
    drawRow(cccdPath);
    drawRow(yearPath);
    drawRow(provincePath);
    drawRow(genderPath);
    drawBorderBottom();

    Resident updated = old; // Bắt đầu với dữ liệu cũ

    // Nhập tên mới
    printf("-> Nhập tên mới (Enter để giữ nguyên): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) > 0) {
        strncpy(updated.name, input, 63);
        updated.name[63] = '\0';
        normalizeName(updated.name);
    }

    // Nhập cccd mới
    printf("-> Nhập CCCD mới (Enter để giữ nguyên): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) > 0) {
        strncpy(updated.CCCD, input, 12);
        updated.CCCD[12] = '\0';

        // Kiểm tra CCCD mới có hợp lệ không
        while (!checkCCCDEdit(&updated, old.CCCD)) {
            setColor(12);
            printf("[Lỗi]. ");
            printf("CCCD không hợp lệ hoặc đã tồn tại.\n-> Nhập lại: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            strncpy(updated.CCCD, input, 12);
            updated.CCCD[12] = '\0';
        }
    }

    //Lưu thay đổi

    // Nếu CCCD thay đổi: đổi tên file và cập nhật cccd.txt
    if (strcmp(old.CCCD, updated.CCCD) != 0) {
        char oldPath[512], newPath[512];
        sprintf(oldPath, "%s/%s.txt", roomPath, old.CCCD);
        sprintf(newPath, "%s/%s.txt", roomPath, updated.CCCD);

        if (rename(oldPath, newPath) != 0) {
            setColor(12);
            printf("[Lỗi]. ");
            setColor(7);
            printf("Không thể đổi tên file.\n");
            return;
        }
        updateCCCDFile(old.CCCD, updated.CCCD);
    }

    // Ghi thông tin mới vào file
    saveResident(&updated, roomPath);
    
    setColor(10);
    printf("\n[Done]. ");
    printf("Đã cập nhật thông tin cư dân.\n");
    setColor(7);
    //displayResidentList(roomPath, floor, room);
    printResidentCCCDAndName(roomPath, floor, room);
}


//  Xây dựng đường dẫn đến phòng
//  Ví dụ: tầng 1, phòng 1 -> FloorList/Tang_1/P101
void buildRoomPath(int floor, int room, char *path) {
    sprintf(path, "FloorList/Tang_%d/P%d%02d", floor, floor, room);
}

//  Luồng chính: nhập tầng -> nhập phòng-> chỉnh sửa cư dân
void handleEditRoom() {

    // Nhập số tầng
    int floor, floorCount = 0;
    char check[10];

    countFloors(&floorCount);
    openFloorList(floorCount);
    while (1) {
        printf("\n-> Nhập số tầng: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &floor)) break;
    }

    //Kiểm tra số tần có hợp lệ không
    if (floor <= 0 || floor > floorCount) {
        setColor(12);
        printf("[Lỗi]. ");
        setColor(7);
        printf("Tầng chọn không hợp lệ.");
        return;
    }

    // Hiển thị các phòng trên tầng đó
    displayRoom(floor);

    //Kiểm tra có phòng nào không, nếu không thì thoát
    if (checkHaveRoom(floor) == 0) {
        return;
    }

    // Nhập số phòng
    int room;
    while (1) {
        printf("\n-> Nhập số phòng: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &room)) break;
    }

    if (checkRoomOrder(room) == 0) {
        setColor(12);
        printf("[Lỗi]. ");
        setColor(7);
        printf("Số phòng không hợp lệ.");
        return;
    }

    // Xây dựng đường dẫn đến phòng
    char roomPath[256];
    buildRoomPath(floor, room, roomPath);

    // Chỉnh sửa cư dân trong phòng
    editResident(roomPath, floor, room);

    return;
}