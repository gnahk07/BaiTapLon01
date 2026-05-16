//Thư viện sử dụng
#include "Menu.h"
#include "Utils.h"
#include <stdio.h> // printf, sprintf, fopen, fclose, fgets, sscanf 
#include <string.h> // strlen, strcmp, strcpy, strstr, strcspn, memmove 
#include <ctype.h> // toupper, isspace 
#include <dirent.h> // DIR, opendir, readdir, closedir

// Hàm chuyển toàn bộ chuỗi thành chữ hoa để tìm kiếm không phân biệt hoa thường
void toUpperCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]); // Chuyển từng ký tự sang chữ hoa
    }
}

// Hàm xóa các ký tự xuống dòng và khoảng trắng thừa ở cuối chuỗi
void cleanString(char *str) {
    str[strcspn(str, "\r\n")] = 0; // Tìm vị trí ký tự xuống dòng và thay bằng ký tự kết thúc chuỗi
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = 0; // Cắt bỏ các dấu cách vô hình ở cuối
    }
}

// Hàm loại bỏ ký tự BOM (3 byte đặc biệt đầu file UTF-8 thường do Notepad tạo ra)
void removeBOM(char *str) {
    if ((unsigned char)str[0] == 0xEF && (unsigned char)str[1] == 0xBB && (unsigned char)str[2] == 0xBF) {
        memmove(str, str + 3, strlen(str + 3) + 1); // Dịch chuyển chuỗi lên 3 byte để đè lên BOM
    }
}

// Hàm mở file để đọc tên khách hàng ở dòng đầu tiên
void getNameFromFile(char *filePath, char *nameOut) {
    FILE *f = fopen(filePath, "r"); // Mở file hồ sơ ở chế độ đọc
    if (f != NULL) {
        if (fgets(nameOut, 64, f)) { // Đọc tối đa 64 ký tự dòng đầu
            removeBOM(nameOut);      // Xóa BOM nếu có
            cleanString(nameOut);    // Làm sạch chuỗi
        }
        fclose(f); // Đóng file sau khi đọc
    } else {
        strcpy(nameOut, "Không rõ"); // Trường hợp file lỗi
    }
}

// Hàm tìm kiếm theo CCCD
void searchByCCCD(char *searchKey) {
    int foundCount = 0; // Biến đếm số kết quả
    DIR *mainDir = opendir("FloorList"); // Mở thư mục gốc
    if (mainDir == NULL) return;

    //In bảng
    downLine();
    printf("╔══════════════════════════════════════════════════════════╦══════════════════╦═══════╗\n");
    printf("║ Họ và Tên                                                ║ CCCD             ║ Phòng ║\n");
    printf("╠══════════════════════════════════════════════════════════╬══════════════════╬═══════╣\n");

    struct dirent *floorEntry;
    while ((floorEntry = readdir(mainDir)) != NULL) { // Quét qua từng Tầng
        if (strstr(floorEntry->d_name, "Tang_")) {
            char floorPath[256];
            sprintf(floorPath, "FloorList/%s", floorEntry->d_name);
            DIR *floorDir = opendir(floorPath);
            if (floorDir == NULL) continue;

            struct dirent *roomEntry;
            while ((roomEntry = readdir(floorDir)) != NULL) { // Quét qua từng Phòng
                if (roomEntry->d_name[0] == 'P') {
                    char roomPath[512];
                    sprintf(roomPath, "%s/%s", floorPath, roomEntry->d_name);
                    DIR *roomDir = opendir(roomPath);
                    if (roomDir == NULL) continue;

                    struct dirent *fileEntry;
                    while ((fileEntry = readdir(roomDir)) != NULL) { // Quét qua từng File
                        if (strstr(fileEntry->d_name, ".txt")) {
                            char currentCCCD[20];
                            strcpy(currentCCCD, fileEntry->d_name);
                            currentCCCD[strlen(currentCCCD) - 4] = '\0'; // Lấy tên file bỏ đuôi .txt

                            // So sánh chính xác số CCCD với tên file
                            if (strcmp(currentCCCD, searchKey) == 0) {
                                char filePath[1024], customerName[64];
                                sprintf(filePath, "%s/%s", roomPath, fileEntry->d_name);
                                getNameFromFile(filePath, customerName); // Lấy tên từ bên trong file
                                drawThreeOption(customerName, currentCCCD, roomEntry->d_name);
                                foundCount++;
                            }
                        }
                    }
                    closedir(roomDir);
                }
            }
            closedir(floorDir);
        }
    }
    closedir(mainDir);
    if (foundCount == 0) {
        char searchKeyPath[124];
        sprintf(searchKeyPath, "Không tìm thấy cư dân với CCCD là: %s", searchKey);
        drawThreeOption(searchKeyPath, "            ", "    ");
        printf("╚══════════════════════════════════════════════════════════╩══════════════════╩═══════╝\n");
    }
    else {
        printf("╚══════════════════════════════════════════════════════════╩══════════════════╩═══════╝\n");
        setColor(10);
        printf("[Thông báo]. ");
        setColor(7);
        printf("Tìm thấy %d kết quả.", foundCount);
    }
}

// Hàm tìm kiếm theo Tên
void searchByName(char *searchKey) {
    int foundCount = 0;
    char upperSearchKey[64];
    strcpy(upperSearchKey, searchKey);
    toUpperCase(upperSearchKey); // Chuyển từ khóa về chữ hoa để so sánh

    DIR *mainDir = opendir("FloorList");
    if (mainDir == NULL) return;

    downLine();
    printf("╔══════════════════════════════════════════════════════════╦══════════════════╦═══════╗\n");
    printf("║ Họ và Tên                                                ║ CCCD             ║ Phòng ║\n");
    printf("╠══════════════════════════════════════════════════════════╬══════════════════╬═══════╣\n");

    struct dirent *floorEntry;
    while ((floorEntry = readdir(mainDir)) != NULL) {
        if (strstr(floorEntry->d_name, "Tang_")) {
            char floorPath[256];
            sprintf(floorPath, "FloorList/%s", floorEntry->d_name);
            DIR *floorDir = opendir(floorPath);
            if (floorDir == NULL) continue;

            struct dirent *roomEntry;
            while ((roomEntry = readdir(floorDir)) != NULL) {
                if (roomEntry->d_name[0] == 'P') {
                    char roomPath[512];
                    sprintf(roomPath, "%s/%s", floorPath, roomEntry->d_name);
                    DIR *roomDir = opendir(roomPath);
                    if (roomDir == NULL) continue;

                    struct dirent *fileEntry;
                    while ((fileEntry = readdir(roomDir)) != NULL) {
                        if (strstr(fileEntry->d_name, ".txt")) {
                            char filePath[1024], customerName[64], upperName[64];
                            sprintf(filePath, "%s/%s", roomPath, fileEntry->d_name);
                            
                            getNameFromFile(filePath, customerName); // Đọc tên từ trong file
                            strcpy(upperName, customerName);
                            toUpperCase(upperName); // Chuyển tên đọc được thành chữ hoa

                            // Kiểm tra xem từ khóa có nằm trong tên khách hàng không
                            if (strstr(upperName, upperSearchKey) != NULL) {
                                char currentCCCD[20];
                                strcpy(currentCCCD, fileEntry->d_name);
                                currentCCCD[strlen(currentCCCD) - 4] = '\0'; // Lấy CCCD từ tên file
                                drawThreeOption(customerName, currentCCCD, roomEntry->d_name);
                                foundCount++;
                            }
                        }
                    }
                    closedir(roomDir);
                }
            }
            closedir(floorDir);
        }
    }
    
    closedir(mainDir);
    if (foundCount == 0) {
        char searchKeyPath[124];
        sprintf(searchKeyPath, "Không tìm thấy cư dân tên: %s", searchKey);
        drawThreeOption(searchKeyPath, "            ", "    ");
        printf("╚══════════════════════════════════════════════════════════╩══════════════════╩═══════╝\n");
    }
    else {
        printf("╚══════════════════════════════════════════════════════════╩══════════════════╩═══════╝\n");
        setColor(10);
        printf("[Thông báo]. ");
        setColor(7);
        printf("Tìm thấy %d kết quả.", foundCount);
    }
}

//Hàm tìm kiếm chính
void handleSearchProcess() {
    int subChoice;
    char keyword[64];

    //In bảng
    downLine();
    drawBorderTop();
    drawRow("Tính Năng Tìm Kiếm Cư Dân");
    drawBorderMiddle();
    drawRow("1. Tìm theo CCCD.");
    drawRow("2. Tìm theo tên.");
    drawRow("0. Quay lại Menu.");
    drawBorderBottom();
    
    //Nhập lựa chọn của người dùng
    char check[10];
    while (1) {
        printf("-> Lựa chọn của bạn: ");
        fgets (check, sizeof(check), stdin);
        if (sscanf(check, "%d", &subChoice)) break;
    }

    if (subChoice == 1) {
        printf("-> Nhập số CCCD cần tìm: ");
        fgets(keyword, sizeof(keyword), stdin);
        cleanString(keyword); // Làm sạch từ khóa
        searchByCCCD(keyword); // Gọi hàm tìm theo CCCD
    } else if (subChoice == 2) {
        printf("-> Nhập tên cư dân cần tìm: ");
        fgets(keyword, sizeof(keyword), stdin);
        cleanString(keyword);
        searchByName(keyword); // Gọi hàm tìm theo tên
    }
}