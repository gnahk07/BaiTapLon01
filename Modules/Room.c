//Thư viện sử dụng
#include "Room.h"
#include "Utils.h"
#include <stdio.h>      // printf, sprintf, fgets, sscanf
#include <string.h>     // strcmp
#include <dirent.h>     // DIR, opendir, readdir, closedir
#include <direct.h>     // _mkdir (Windows)
#include <sys/stat.h>   // struct stat, stat, S_ISDIR

//Check có phòng nào ở tầng không
int checkHaveRoom(int floor) {

    //Tạo đừng dẫn đến folder phòng
    char path[256];
    sprintf(path, "FloorList/Tang_%d", floor);

    DIR *dp = opendir (path);
    if (dp == NULL) {
        //trả về 0 (không có phòng nào)
        closedir(dp);
        return 0;
    }
    //trả về 1 (có phòng)
    closedir(dp);
    return 1;
}
//In các phòng đã có sẵn
void displayRoom(int selectFloor){

    //Mở thư mục chứa phòng
    char path[256];
    sprintf(path, "./FloorList/Tang_%d", selectFloor);
    struct dirent *dir;
    DIR *dp = opendir(path);

    //Kiểm tra có phòng nào không
    if (dp == NULL) {
        setColor(12);
        printf("[Lỗi]. ");
        setColor(7);
        printf("Không mở được Folder tầng %d.\n", selectFloor);
        return;
    }

    //Tạo biến đếm số phòng xem có phòng nào không
    int roomCount = 0;
    setColor(14);

    //Đọc từng folder/file
    while ((dir = readdir(dp)) != NULL) {

        //Bỏ qua "." và ".."
        if (strcmp(dir->d_name, ".") == 0 ||
            strcmp(dir->d_name, "..") == 0) {
            continue;
        }

        //Tạo đường dẫn đầy đủ đến folder phòng
        char roomPath[256];
        sprintf(roomPath, "%s/%s", path, dir->d_name);
        struct stat st;

        //Kiểm tra có phải folder không
        if (stat(roomPath, &st) == 0 && S_ISDIR(st.st_mode)) {

            //Nếu là phòng đầu tiên thì in tiêu đề
            if (roomCount == 0) {
                printf("Hiện có sẵn các phòng: ");
            }

            //In tên phòng
            printf("%s, ", dir->d_name);
            roomCount++;
        }
    }

    //Nếu không có phòng
    if (roomCount == 0) {
        printf("Hiện không có phòng nào ở tầng %d.", selectFloor);
    } //Nếu có phòng thì xóa dấu , cuối cùng và thay bằng dấu .
    else {
        printf("\b\b.");
    }

    //Đóng thư mục
    closedir(dp);
    setColor(7);
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

//Hàm tạo phòng
void addRoom(int floorCount) {

    //Lựa chọn tầng muốn thêm phòng và kiểm tra
    int selectFloor;
    char check[10];
    while (1) {
        printf("\n-> Chọn tầng muốn thêm phòng: ");
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
        char roomPath[256];
        sprintf(roomPath, "./FloorList/Tang_%d/P%d%02d", selectFloor, selectFloor, roomOrder);

        //Mở folder
        DIR *dp = opendir(roomPath);

        //Kiểm tra Folder có tồn tại chưa, nếu chưa thì tạo Folder phòng
        if (dp != NULL) {
            closedir(dp);
            setColor(12);
            printf("[Lỗi]. Phòng P%d%02d đã tồn tại.", selectFloor, roomOrder);
            setColor(7);
        } else {
            _mkdir(roomPath);
            setColor(10);
            printf("Phòng P%d%02d đã được tạo thành công.", selectFloor, roomOrder);
            setColor(7);
        }
        
    } else {
        setColor(12);
        printf("[Lỗi]. Tầng bạn chọn không tồn tại.\n");
        setColor(7);
    }
}
