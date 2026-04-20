//Thư viện sử dụng
#include <stdio.h>
#include <windows.h>

//Hàm main
int main () {

    //chỉnh sửa lỗi front chữ 
	SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    //Hiển thị Header
    displayHeader ();

    //Lựa chọn thêm tầng hoặc thêm phòng và gọi hàm tương ứng
    chooseAddOption();

    return 0;
}