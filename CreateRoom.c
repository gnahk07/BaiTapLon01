//Thư viện sử dụng
#include <stdio.h>
#include <windows.h>
#include <dirent.h>

//Hiển thị Header
void displayHeader() {
    printf("\n=====================================  PHẦN MỀM TẠO PHÒNG TRUNG CƯ  =====================================\n\n");
}






//Hàm main
int main () {

    //chỉnh sửa lỗi front chữ 
	SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    //Hiển thị Header
    displayHeader ();




    return 0;
}