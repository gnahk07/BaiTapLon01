#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <direct.h>
#include "floor.h"

//Đếm số tầng hiện có
void countFloors(int *floorCount) {

    //Mở thư mục FloorList
    struct dirent *dir;
    DIR *dp = opendir("./FloorList");

    //Kiểm tra thư mục có tồn tại không. Nếu không tồn tại thì tạo thư mục FloorList
    if (dp == NULL) {

        //Tạo thư mục FloorList và thoát chương trình
        _mkdir("FloorList");
        return;
    }

    //Đếm số tầng bằng cách đếm số thư mục con trong FloorList
    while ((dir = readdir(dp)) != NULL) {

        //Bỏ qua các thư mục đặc biệt "." và ".."
        if (strcmp(dir->d_name, ".") != 0 && 
            strcmp(dir->d_name, "..") != 0) {

            //Tạo đường dẫn đầy đủ đến thư mục con
            char path[256];
            sprintf(path, "./FloorList/%s", dir->d_name);
            struct stat st;

            //Kiểm tra nếu đường dẫn là một thư mục thì tăng biến đếm tầng lên 1
            if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
                (*floorCount)++;
            }
        }
    }

    //Đóng thư mục sau khi đếm xong
    closedir(dp);
}

//Mở danh sách tầng
void openFloorList(int floorCount) {

    //Kiểm tra nếu không có tầng nào thì thông báo và kết thúc chương trình
    if (floorCount == 0) {
        printf("Hiện chưa có tầng nào.");
        return;
    } 
    
    //Nếu có tầng thì in danh sách tầng
    else {
        printf("Hiện có tầng: ");
        int tempFloorCount = 0;
        while (tempFloorCount != floorCount) {
            printf("%d, ", tempFloorCount + 1);
            tempFloorCount++;
        }
    }

    //Xóa dấu phẩy cuối cùng và thay bằng dấu chấm
    printf("\b\b.");
}

//Hàm thêm tầng
void addFloor() {

    //Hỏi người dùng có chắc chắn muốn thêm tầng mới không và kiểm tra
    char confirm;
    char check[10];
    while (1) {
        printf("Bạn có chắc chắn muốn thêm tầng mới không? (y/n): ");
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
        int floorCount = 0;
        countFloors(&floorCount);
        sprintf(path, "./FloorList/Tang_%d", floorCount + 1);
        _mkdir(path);

        //Xác nhận tạo thư mục mới thành công
        printf("Tầng mới '%d' đã được tạo thành công.\n\n", floorCount + 1);
    } else {
        printf("Hủy thêm tầng mới.\n\n");
    }
}
