//Thư viện sử dụng
#include "Floor.h"
#include "Utils.h"
#include <stdio.h>    // printf, fgets, sscanf, sprintf
#include <direct.h>   // _mkdir

//Mở danh sách tầng
void openFloorList(int floorCount) {

    //Kiểm tra nếu không có tầng nào thì thông báo và kết thúc chương trình
    if (floorCount == 0) {
        setColor(12);
        printf("[Lỗi]. ");
        setColor(7);
        printf("Hiện chưa có tầng nào.");
        return;
    } 
    
    //Nếu có tầng thì in danh sách tầng
    else {
        setColor(14);
        printf("Hiện có tầng: ");
        int tempFloorCount = 0;
        while (tempFloorCount != floorCount) {
            printf("%d, ", tempFloorCount + 1);
            tempFloorCount++;
        }
    }

    //Xóa dấu phẩy cuối cùng và thay bằng dấu chấm
    printf("\b\b.");
    setColor(7);
}

//Hàm tạo tầng
void addFloor(int floorCount) {

    //Hỏi người dùng có chắc chắn muốn thêm tầng mới không và kiểm tra
    char confirm;
    char check[10];
    while (1) {
        printf("-> Bạn có muốn thêm tầng %d không [y/n]: ", floorCount);
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, " %c", &confirm) == 1 && 
            (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N')) {
            break;
        }
    }
    
    //Nếu người dùng xác nhận muốn thêm tầng mới thì thực hiện tạo thư mục mới cho tầng mới
    if (confirm == 'y' || confirm == 'Y') {

        //Tạo thư mục mới cho tầng mới
        char path[256];
        sprintf(path, "./FloorList/Tang_%d", floorCount + 1);
        _mkdir(path);

        //Xác nhận tạo thư mục mới thành công
        setColor(10);
        printf("Tầng %d đã được tạo thành công.\n\n", floorCount + 1);
        setColor(7);
    } else {
        setColor(12);
        printf("Hủy thêm tầng mới.\n\n");
        setColor(7);
    }
}
