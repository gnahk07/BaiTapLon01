//Thu vien su dung
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <dirent.h>
#include <sys/stat.h>

// ============================================================
//  Cau truc cu dan
// ============================================================
typedef struct Resident {
    char name[64];
    char CCCD[13];
    char year[5];
    char gender[5];
    char province[32];
} Resident;

// ============================================================
//  Hien thi Header
// ============================================================
void displayHeader() {
    printf("\n=====================================  PHAN MEM CHINH SUA PHONG TRUNG CU  =====================================\n\n");
}

// ============================================================
//  Kiem tra chuoi co toan la so khong
// ============================================================
int isAllDigits(const char *s) {
    for (int i = 0; s[i]; i++) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

// ============================================================
//  Kiem tra ma tinh co hop le khong
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
//  Kiem tra CCCD co bi trung khong
//  - Neu CCCD trung voi chinh no (khi sua) thi bo qua
// ============================================================
int checkDuplicates(const char *CCCD, const char *skipCCCD) {
    FILE *f = fopen("Data/cccd.txt", "r");
    if (f == NULL) return 1; // Khong mo duoc file thi cho qua

    char line[16];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';

        // Bo qua chinh CCCD dang duoc sua
        if (skipCCCD != NULL && strcmp(line, skipCCCD) == 0) continue;

        if (strcmp(line, CCCD) == 0) {
            fclose(f);
            return 0; // Bi trung
        }
    }
    fclose(f);
    return 1; // Khong trung
}

// ============================================================
//  Kiem tra CCCD va trich xuat thong tin (nam sinh, que quan, gioi tinh)
//  Tra ve 1 neu hop le, 0 neu khong hop le
// ============================================================
int checkCCCD(Resident *r, const char *skipCCCD) {

    // Kiem tra do dai
    if (strlen(r->CCCD) != 12) return 0;

    // Kiem tra toan so
    if (!isAllDigits(r->CCCD)) return 0;

    // Kiem tra trung lap
    if (!checkDuplicates(r->CCCD, skipCCCD)) return 0;

    // Lay ma tinh (3 chu so dau)
    int provinceCode = (r->CCCD[0]-'0')*100 + (r->CCCD[1]-'0')*10 + (r->CCCD[2]-'0');
    if (!validProvince(provinceCode)) return 0;

    // Doc ten tinh tu file
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

    // Lay gioi tinh (chu so thu 4)
    int genderDigit = r->CCCD[3] - '0';
    if (genderDigit < 0 || genderDigit > 3) return 0;
    strcpy(r->gender, (genderDigit % 2 == 0) ? "Nam" : "Nu");

    // Lay nam sinh (chu so thu 5-6)
    int yearVal = (r->CCCD[4]-'0')*10 + (r->CCCD[5]-'0');
    if (genderDigit == 0 || genderDigit == 1)
        sprintf(r->year, "19%02d", yearVal);
    else
        sprintf(r->year, "20%02d", yearVal);

    return 1;
}

// ============================================================
//  Chuan hoa ten: viet hoa chu cai dau moi tu
// ============================================================
void normalizeName(char name[]) {
    for (int i = 0; name[i]; i++)
        name[i] = tolower((unsigned char)name[i]);

    if (name[0] != '\0')
        name[0] = toupper((unsigned char)name[0]);

    for (int i = 1; name[i]; i++)
        if (name[i-1] == ' ' && name[i] != ' ')
            name[i] = toupper((unsigned char)name[i]);
}

// ============================================================
//  Cap nhat CCCD trong Data/cccd.txt (doi CCCD cu thanh moi)
// ============================================================
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
//  Hien thi danh sach cu dan trong mot phong
//  Tra ve so luong cu dan tim thay
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

        // Mo file doc ten cu dan
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
//  Doc thong tin cu dan tu file
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
//  Ghi thong tin cu dan vao file
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
//  Chinh sua thong tin mot cu dan trong phong
// ============================================================
void editResident(const char *roomPath) {

    // Hien thi danh sach cu dan
    if (displayResidentList(roomPath) == 0) return;

    // Buffer tam lon de tranh ket \n trong stdin
    char input[32];

    // Nhap CCCD cu dan muon sua
    Resident old;
    printf("\nNhap CCCD cua cu dan muon sua: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    strncpy(old.CCCD, input, 12);
    old.CCCD[12] = '\0';

    // Kiem tra file cu dan co ton tai khong
    if (!loadResident(&old, roomPath)) {
        printf("[Loi] Khong tim thay cu dan voi CCCD: %s\n", old.CCCD);
        return;
    }

    // Hien thi thong tin hien tai
    printf("\nThong tin hien tai:\n");
    printf("  Ho ten   : %s\n", old.name);
    printf("  CCCD     : %s\n", old.CCCD);
    printf("  Nam sinh : %s\n", old.year);
    printf("  Que quan : %s\n", old.province);
    printf("  Gioi tinh: %s\n", old.gender);

    Resident updated = old; // Bat dau voi du lieu cu

    // ---- Nhap ten moi ----
    printf("\nNhap ten moi (Enter de giu nguyen): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) > 0) {
        strncpy(updated.name, input, 63);
        updated.name[63] = '\0';
        normalizeName(updated.name);
    }

    // ---- Nhap CCCD moi ----
    printf("Nhap CCCD moi (Enter de giu nguyen): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) > 0) {
        strncpy(updated.CCCD, input, 12);
        updated.CCCD[12] = '\0';

        // Kiem tra hop le, neu sai thi nhap lai
        while (!checkCCCD(&updated, old.CCCD)) {
            printf("[Loi] CCCD khong hop le hoac da ton tai. Nhap lai: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            strncpy(updated.CCCD, input, 12);
            updated.CCCD[12] = '\0';
        }
    }

    // ---- Luu thay doi ----

    // Neu CCCD thay doi: doi ten file va cap nhat cccd.txt
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

    // Ghi thong tin moi vao file
    saveResident(&updated, roomPath);

    printf("\n[Thanh cong] Da cap nhat thong tin cu dan.\n");
    displayResidentList(roomPath);
}

// ============================================================
//  Xay dung duong dan den phong
//  Vi du: tang 1, phong 1 -> FloorList/Tang_1/P101
// ============================================================
void buildRoomPath(int floor, int room, char *path) {
    sprintf(path, "FloorList/Tang_%d/P%d%02d", floor, floor, room);
}

// ============================================================
//  Hien thi cac phong co san tren mot tang
// ============================================================
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
        // Dung stat() de kiem tra la thu muc (tuong thich MinGW/Windows)
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

// ============================================================
//  Luong chinh: nhap tang > nhap phong > chinh sua cu dan
// ============================================================
void handleEditRoom() {
    printf("\n========== CHINH SUA THONG TIN CU DAN ==========\n");

    // Nhap so tang
    int floor;
    char input[16];
    printf("Nhap so tang: ");
    fgets(input, sizeof(input), stdin);
    floor = atoi(input);
    if (floor <= 0) {
        printf("[Loi] So tang khong hop le.\n");
        return;
    }

    // Hien thi cac phong tren tang do
    displayRoomsOnFloor(floor);

    // Nhap so phong
    int room;
    printf("Nhap so phong: ");
    fgets(input, sizeof(input), stdin);
    room = atoi(input);
    if (room <= 0) {
        printf("[Loi] So phong khong hop le.\n");
        return;
    }

    // Xay dung duong dan den phong
    char roomPath[256];
    buildRoomPath(floor, room, roomPath);

    // Chinh sua cu dan trong phong
    editResident(roomPath);
}

// ============================================================
//  Ham main
// ============================================================
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    displayHeader();
    handleEditRoom();

    return 0;
}
