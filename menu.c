#include <stdio.h>
#include <windows.h>
#include "menu.h"

//In menu ra màng hình
void menu() {

    //Chỉnh sửa lỗi front chữ 
	SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    //In lần lượt các chức năng của phần mềm
    printf("1. Tạo tầng mới");
    printf("2. Tạo phòng mới cho tầng\n");
    printf("3. Thêm cư dân vào phòng\n");
    printf("4. Chỉnh sửa thông tin cư dân\n");
    printf("5. Xóa cư dân\n");
    printf("6. Tìm kiếm cư dân\n");
    printf("7. In danh sách\n");
}

//Lựa chọn thêm tầng hoặc thêm phòng
void chooseAddOption() {

    //Đếm số tầng hiện có
    int floorCount = 0;
    countFloors (&floorCount);

    //Yêu cầu người dùng nhập lựa chọn và kiểm tra
    int choice;
    char check[10];
    while (1) {
        //Mở danh sách tầng
        openFloorList(floorCount);

        //Yêu cầu người dùng nhâp lựa chọn và kiểm tra
        printf("\nBạn muốn thêm gì? (1: Thêm tầng, 2: Thêm phòng): ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &choice) == 1) break;
        printf ("Lựa chọn không hợp lệ. Vui lòng chọn lại.\n\n");
    }

    //Xử lý lựa chọn của người dùng
    if (choice == 1) {
        //Gọi hàm thêm tầng
        addFloor();
        chooseAddOption();
    } else if (choice == 2) {
        //Kiểm tra đã có tầng nào chưa
        if (floorCount == 0) {
            printf("Hiện không có tầng nào. Hãy tạo thêm tầng để tạo phòng.");
            chooseAddOption();
        } else {
            //Gọi hàm thêm phòng
            addRoom(floorCount);
            chooseAddOption();
        }
    } else {
        printf("Lựa chọn không hợp lệ. Vui lòng chọn lại.\n\n");
        chooseAddOption();
    }
}
