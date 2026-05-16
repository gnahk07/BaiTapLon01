#include <stddef.h>
#ifndef MENU_H
#define MENU_H

//Hàm vẽ viền trên của bảng
void drawBorderTop();

//Hàm vẽ đường phân cách giữa bảng
void drawBorderMiddle();

//Hàm vẽ viền dưới của bảng
void drawBorderBottom();

//Đếm ký tự UTF-8 gần đúng
int utf8len(const char *s);

//Hàm in nội dung bên trong bảng
void drawRow(char text[]);

//hàm in tên, CCCD, phòng
void drawThreeOption(char name[], char CCCD[], char room[]);

//Tạo khoảng xuống dòng
void downLine();

#endif