//Thư viện sử dụng
#include "Menu.h"
#include <stdio.h>

//Hàm vẽ viền trên của bảng
void drawBorderTop() {
    printf("╔════════════════════════════════════════════════╗\n");
}

//Hàm vẽ đường phân cách giữa bảng
void drawBorderMiddle() {
    printf("╠════════════════════════════════════════════════╣\n");
}

//Hàm vẽ viền dưới của bảng
void drawBorderBottom() {
    printf("╚════════════════════════════════════════════════╝\n");
}

//Đếm ký tự UTF-8 gần đúng
int utf8len(const char *s) {
    int len = 0;
    while (*s) {
        //Byte không phải continuation byte
        if ((*s & 0xC0) != 0x80) {
            len++;
        }
        s++;
    }
    return len;
}

//Hàm in nội dung bên trong bảng
void drawRow(char text[]) {
    int width = 46;

    //Đếm số ký tự Unicode
    int len = utf8len(text);

    printf("║ %s", text);

    //Padding khoảng trắng
    for (int i = 0; i < width - len; i++) {
        printf(" ");
    }

    printf(" ║\n");
} 

//hàm in tên, CCCD, phòng
void drawThreeOption(char name[], char CCCD[], char room[]) {

    int max = 87; //87 - 20 - 12 - 4
    int len = utf8len(name);

    printf("║ %s", name);

    for (int i = 0; i < 57 - len; i++) {
        printf(" ");
    }
    printf("║ %s     ║ %s  ║\n", CCCD, room);
}

//Tạo khoảng xuống dòng
void downLine() {
    printf("\n\n");
}

//Hàm in nội dung bên trong bảng
void draw(char name[], char CCCD[], int count) {
    int width = 70;

    //Đếm số ký tự Unicode
    int len1 = utf8len(name);

    printf("║ %d. %s", count, name );

    //Padding khoảng trắng
    for (int i = 0; i < width - len1 - 16; i++) {
        printf(" ");
    }

    printf ("║ %s", CCCD);

    printf(" ║\n");
} 