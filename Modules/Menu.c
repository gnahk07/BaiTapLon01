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

//Tạo khoảng xuống dòng
void downLine() {
    printf("\n\n");
}