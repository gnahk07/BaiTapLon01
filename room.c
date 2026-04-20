#include <stdio.h>
#include <dirent.h>
#include <direct.h>
#include <string.h>
#include <sys/stat.h>
#include "room.h"

//Hàm kiểm tra lựa chọn tầng để thêm phòng có phù hợp không
void checkSelectFloor(int selectFloor, int floorCount, int *check) {

    //Kiểm tra lựa chọn
    if (selectFloor >= 1 && selectFloor <= floorCount){
        *check = 1;
    }
}

//In các phòng đã có sẵn
void displayRoom(int selectFloor){

    //Mở thư mục chứa phòng
    char path[256];
    sprintf(path, "./FloorList/Floor_%d", selectFloor);
    struct dirent *dir;
    DIR *dp = opendir(path);

    //Kiểm tra có phòng nào không
    if (dp == NULL) {
        printf("Không có phòng nào ở tầng %d.\n", selectFloor);
    } else {
        
        //In các phòng hiện có
        printf("Hiện có sẵn các phòng: ");
        while ((dir = readdir(dp)) != NULL) {
            printf("%.*s, ", dir->d_name);
        }

        //Xóa dấu phẩy cuối cùng và thay bằng dấu chấm
        printf("\b\b.");
    }

    //Đóng thư mục sau khi in dãy phòng xong
    closedir(dp);
}

//Nhập số thứ tự phòng và kiểm tra
void inputRoomOrder(int *roomOrder) {
    char check[10];
    while (1) {
        printf("\nNhập số thứ tự phòng mới từ [0, 99]: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", roomOrder) == 1) break;
    }
}

//Kiểm tra số thứ tự có phù hợp không
int checkRoomOrder(int roomOrder) {
    if (roomOrder <= 0 || roomOrder >= 100) {
        return 0;
    }
    return 1;
}

//Hàm thêm phòng
void addRoom(int floorCount) {

    //Lựa chọn tầng muốn thêm phòng và kiểm tra
    int selectFloor;
    char check[10];
    while (1) {
        printf("Chọn tầng muốn thêm phòng: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &selectFloor) == 1) break;
    }

    //Kiểm tra lựa chọn có phù hợp không
    int flat = 0;
    checkSelectFloor(selectFloor, floorCount, &flat);

    //Kiểm tra lựa chọn có phù hợp không nếu đã phù hợp thì tạo phòng
    if (flat == 1) {
        //Gọi hàm in ra các dãy phòng đã có sẵn
        displayRoom(selectFloor);

        //Nhập số thứ tự phòng
        int roomOrder;
        inputRoomOrder(&roomOrder);

        //Kiểm tra số thứ tự phòng
        while (checkRoomOrder(roomOrder) == 0) {
            inputRoomOrder(&roomOrder);
        }

        //Tạo đường dẫn đầy đủ đến nơi chứa tên phòng
        char temp[256];
        sprintf(temp, "./FloorList/Floor_%d", selectFloor);

        //Kiểm tra Folder có tồn tại chưa
        struct stat *dir;
        if (stat(temp, dir) == 0 && (dir->st_mode & S_IFDIR)) {
            printf("Phòng P%d%d đã tồn tại\n", selectFloor, roomOrder);
        } else {
            // Tạo folder
            if (_mkdir(temp) == 0) {
                printf("Tạo folder thành công\n");
            } else {
                printf("Tạo thất bại\n");
            }
        }
    } else {
        //Cho người dùng chọn lại phòng
        printf("Tầng bạn chọn không tồn tại, chọn lại tầng.\n");
        addRoom(floorCount);
    }
}
