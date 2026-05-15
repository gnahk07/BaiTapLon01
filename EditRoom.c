//Thu vien su dung
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <dirent.h>
#include <sys/stat.h>

// ============================================================
// Hàm cấu trúc cư dân
// ============================================================
typedef struct Resident {
    char name[64];
    char CCCD[13];
    char year[8];
    char gender[8];
    char province[32];
} Resident;

// ============================================================
// Hiển thị header
// ============================================================
void displayHeader() {
    printf("\n=====================================  PHAN MEM CHINH SUA PHONG TRUNG CU  =====================================\n\n");
}

// ============================================================
//  Kiểm tra chuỗi có phải toàn là số không
// ============================================================
int isAllDigits(const char *s) {
    for (int i = 0; s[i]; i++) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

// ============================================================
//  Kiểm tra mã tỉnh có hợp lệ không
// ============================================================
int validProvince(int code) {
    int provinces[] = {
        1,2,4,6,8,10,11,12,14,15,
        17,19,20,22,24,25,26,27,30,
        31,33,34,35,36,37,38,40,42,
        44,45,46,48,49,51,52,54,56,
        58,60,62,64,66,67,68,70,72,
        74,75,77,79,80,82,83,84,86,
        87,89,91,92,93,94,95,96
    };
    int n = sizeof(provinces) / sizeof(provinces[0]);
    for (int i = 0; i < n; i++)
        if (provinces[i] == code) return 1;
    return 0;
}

// ============================================================
//  Kiểm tra CCCD có bị trùng không
//  - Nếu CCCD trùng với chính nó (khi sửa) thì bỏ qua
// ============================================================
int checkDuplicates(const char *CCCD, const char *skipCCCD) {
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
int checkCCCD(Resident *r, const char *skipCCCD) {

    // Kiểm tra độ dài
    if (strlen(r->CCCD) != 12) return 0;

    // Kiểm tra tất cả là số
    if (!isAllDigits(r->CCCD)) return 0;

    // Kiểm tra trùng lặp
    if (!checkDuplicates(r->CCCD, skipCCCD)) return 0;

    // Lấy mã tỉnh (3 chữ số đầu)
    int provinceCode = (r->CCCD[0]-'0')*100 + (r->CCCD[1]-'0')*10 + (r->CCCD[2]-'0');
    if (!validProvince(provinceCode)) return 0;

    // Đọc tên tỉnh từ file
    char path[256];
    sprintf(path, "Data/Province/%03d.txt", provinceCode);
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        strcpy(r->province, "Khong tim thay que quan");
    } else {
        fgets(r->province, sizeof(r->province), f);
        r->province[strcspn(r->province, "\n")] = '\0';
        fclose(f);
    }

    // Lấy giới tính (chữ số thứ 4)
    int genderDigit = r->CCCD[3] - '0';
    if (genderDigit < 0 || genderDigit > 3) return 0;
    strcpy(r->gender, (genderDigit % 2 == 0) ? "Nam" : "Nu");

    // Lấy năm sinh (chữ số thứ 5-6)
    int yearVal = (r->CCCD[4]-'0')*10 + (r->CCCD[5]-'0');
    if (genderDigit == 0 || genderDigit == 1)
        sprintf(r->year, "19%02d", yearVal);
    else
        sprintf(r->year, "20%02d", yearVal);

    return 1;
}

//   Viết hoa chữ cái đầu của mỗi từ trong tên

void normalizeName(char name[]) {
    for (int i = 0; name[i]; i++)
        name[i] = tolower((unsigned char)name[i]);

    if (name[0] != '\0')
        name[0] = toupper((unsigned char)name[0]);

    for (int i = 1; name[i]; i++)
        if (name[i-1] == ' ' && name[i] != ' ')
            name[i] = toupper((unsigned char)name[i]);
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
//   Hiển thị danh sách cư dân trong một phòng
//  Trả về số lượng cư dân tìm thấy
// ============================================================
int displayResidentList(const char *roomPath) {
    DIR *d = opendir(roomPath);
    if (d == NULL) {
        printf("Khong tim thay phong: %s\n", roomPath);
        return 0;
    }

    printf("\nDanh sach cu dan trong phong:\n");
    printf("-------------------------------------\n");

    struct dirent *entry;
    int count = 0;
    while ((entry = readdir(d)) != NULL) {

        // Chi xu ly file .txt
        char *filename = entry->d_name;
        int len = strlen(filename);
        if (len <= 4 || strcmp(filename + len - 4, ".txt") != 0) continue;

        // Mở file để đọc tên cư dân
        char filePath[512];
        sprintf(filePath, "%s/%s", roomPath, filename);

        FILE *f = fopen(filePath, "r");
        char name[64] = "(Khong co ten)";
        if (f != NULL) {
            fgets(name, sizeof(name), f);
            name[strcspn(name, "\n")] = '\0';
            fclose(f);
        }

        // In CCCD (ten file bo duoi .txt)
        char cccd[13];
        strncpy(cccd, filename, len - 4);
        cccd[len - 4] = '\0';

        printf("%d. CCCD: %s | Ten: %s\n", ++count, cccd, name);
    }

    if (count == 0) printf("(Phong trong, chua co cu dan)\n");
    printf("-------------------------------------\n");
    closedir(d);
    return count;
}

// ============================================================
//  Đọc thông tin cư dân từ file 
// ============================================================
int loadResident(Resident *r, const char *roomPath) {
    char filePath[512];
    sprintf(filePath, "%s/%s.txt", roomPath, r->CCCD);

    FILE *f = fopen(filePath, "r");
    if (f == NULL) return 0;

    fgets(r->name,     sizeof(r->name),     f); r->name[strcspn(r->name, "\n")]         = '\0';
    fgets(r->year,     sizeof(r->year),     f); r->year[strcspn(r->year, "\n")]         = '\0';
    fgets(r->province, sizeof(r->province), f); r->province[strcspn(r->province, "\n")] = '\0';
    fgets(r->gender,   sizeof(r->gender),   f); r->gender[strcspn(r->gender, "\n")]     = '\0';
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
        printf("[Loi] Khong the ghi file cu dan.\n");
        return;
    }
    fprintf(f, "%s\n%s\n%s\n%s", r->name, r->year, r->province, r->gender);
    fclose(f);
}

// ============================================================
//  Chỉnh sửa thông tin cư dân
// ============================================================
void editResident(const char *roomPath) {

    // Hiển thị danh sách cư dân trong phòng
    if (displayResidentList(roomPath) == 0) return;

    // Buffer nhập liệu chung
    char input[32];

    // Nhập CCCD của cư dân muốn sửa
    Resident old;
    printf("\nNhap CCCD cua cu dan muon sua: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    strncpy(old.CCCD, input, 12);
    old.CCCD[12] = '\0';

    // Kiểm file cư dân có tồn tại không
    if (!loadResident(&old, roomPath)) {
        printf("[Loi] Khong tim thay cu dan voi CCCD: %s\n", old.CCCD);
        return;
    }

    // Hiển thị thông tin hiện tại
    printf("\nThong tin hien tai:\n");
    printf("  Ho ten   : %s\n", old.name);
    printf("  CCCD     : %s\n", old.CCCD);
    printf("  Nam sinh : %s\n", old.year);
    printf("  Que quan : %s\n", old.province);
    printf("  Gioi tinh: %s\n", old.gender);

    Resident updated = old; // Bắt đầu với dữ liệu cũ

    // Nhập tên mới
    printf("\nNhap ten moi (Enter de giu nguyen): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) > 0) {
        strncpy(updated.name, input, 63);
        updated.name[63] = '\0';
        normalizeName(updated.name);
    }

    // Nhập cccd mới
    printf("Nhap CCCD moi (Enter de giu nguyen): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) > 0) {
         strncpy(updated.CCCD, input, 12);
        updated.CCCD[12] = '\0';

        // Kiểm tra CCCD mới có hợp lệ không
        while (!checkCCCD(&updated, old.CCCD)) {
            printf("[Loi] CCCD khong hop le hoac da ton tai. Nhap lai: ");
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
            printf("[Loi] Khong the doi ten file.\n");
            return;
        }
        updateCCCDFile(old.CCCD, updated.CCCD);
    }

    // Ghi thông tin mới vào file
    saveResident(&updated, roomPath);

    printf("\n[Thanh cong] Da cap nhat thong tin cu dan.\n");
    displayResidentList(roomPath);
}


//  Xây dựng đường dẫn đến phòng
//  Ví dụ: tầng 1, phòng 1 -> FloorList/Tang_1/P101

void buildRoomPath(int floor, int room, char *path) {
    sprintf(path, "FloorList/Tang_%d/P%d%02d", floor, floor, room);
}


//  Hiển thị các phòng trên một tầng

void displayRoomsOnFloor(int floor) {
    char floorPath[256];
    sprintf(floorPath, "FloorList/Tang_%d", floor);

    DIR *d = opendir(floorPath);
    if (d == NULL) {
        printf("Khong tim thay tang %d.\n", floor);
        return;
    }

    printf("Cac phong tren tang %d: ", floor);

    struct dirent *entry;
    int count = 0;
    while ((entry = readdir(d)) != NULL) {
        // Dùng stat() để kiểm tra là thư mục (tương thích MinGW/Windows)
        char fullPath[512];
        sprintf(fullPath, "%s/%s", floorPath, entry->d_name);
        struct stat st;

        if (stat(fullPath, &st) == 0 && S_ISDIR(st.st_mode)) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            printf("%s  ", entry->d_name);
            count++;
        }
    }
    if (count == 0) printf("(Chua co phong nao)");
    printf("\n");
    closedir(d);
}


//  Luồng chính: nhập tầng -> nhập phòng-> chỉnh sửa cư dân

void handleEditRoom() {
    printf("\n========== CHINH SUA THONG TIN CU DAN ==========\n");

    // Nhập số tầng
    int floor;
    char input[16];
    printf("Nhap so tang: ");
    fgets(input, sizeof(input), stdin);
    floor = atoi(input);
    if (floor <= 0) {
        printf("[Loi] So tang khong hop le.\n");
        return;
    }

    // Hiển thị các phòng trên tầng đó
    displayRoomsOnFloor(floor);

    // Nhập số phòng
    int room;
    printf("Nhap so phong: ");
    fgets(input, sizeof(input), stdin);
    room = atoi(input);
    if (room <= 0) {
        printf("[Loi] So phong khong hop le.\n");
        return;
    }

    // Xây dựng đường dẫn đến phòng
    char roomPath[256];
    buildRoomPath(floor, room, roomPath);

    // Chỉnh sửa cư dân trong phòng
    editResident(roomPath);
}


//  Hàm main

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    displayHeader();
    handleEditRoom();

    return 0;
}
// 4:51
