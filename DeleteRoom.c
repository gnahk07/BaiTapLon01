//Thư viện sử dụng
#include <stdio.h>
#include <windows.h>

//Hiển thị Header
void displayHeader() {
    printf("\n=====================================  PHẦN MỀM XÓA PHÒNG TRUNG CƯ  =====================================\n\n");
}




//Hàm main
int main () {

    //chỉnh sửa lỗi front chữ 
	SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    //Hiển thị Header
    displayHeader ();

    printf ("Hello");

    return 0;
}