//Thư viện sử dụng
#include <stdio.h>
#include <windows.h>
#include "Modules/Resident.h"
#include "Modules/Delete.h"
#include "Modules/Edit.h"
#include "Modules/Floor.h"
#include "Modules/Menu.h"
#include "Modules/Room.h"
#include "Modules/Sreach.h"
#include "Modules/Utils.h"

//Hàm main
int main () {
    //Chỉnh sửa lỗi front chữ Tiếng Việt
	SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    //Kiểm tra File Data
    createDataFolder();

    //Làm hiệu ứng Loading
    //fakeLoading();

    //Xóa màng hình sau khi loading xong
    system("cls");

    while (1) {
        //In bảng tính năng
        drawBorderTop();
        drawRow("CHÀO MỪNG ĐẾN VỚI PHẦN MỀM QUẢN LÝ CHUNG CƯ");
        drawBorderMiddle();
        drawRow("1. Thêm tầng/phòng.");
        drawRow("2. Thêm cư dân.");
        drawRow("3. Xóa tầng/phòng.");
        drawRow("4. Xóa cư dân.");
        drawRow("5. Chỉnh sửa cư dân.");
        drawRow("6. Tìm kiếm cư dân.");
        drawRow("0. Thoát.");
        drawBorderBottom();

        //Nhận lựa chọn của người dùng
        int select;
        char check[10];
        while (1) {
            printf("-> Lựa chọn của bạn: ");
            fgets (check, sizeof(check), stdin);
            if (sscanf(check, "%d", &select)) break;
        }
        
        //Chạy tính năng
        switch(select) {
            case 1:
                chooseAddOption();
                break;
            case 2:
                inputResidentInformation();
                break;
            case 3:
                checkOption();
                break;
            case 4:
                handleCheckOutProcess();
                break;
            case 5:
                handleEditRoom();
                break;
            case 6:
                handleSearchProcess();
                break;
            case 0:
                system("cls");
                printf("Đã thoát chương trình.");
                Sleep(4000);
                exit(0);
        }
        enter();
        system("cls");
    }
}