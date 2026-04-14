//Thư viện sử dụng
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dirent.h>
#include <direct.h>
#include <sys/stat.h>
#include <string.h>

//Hiển thị Header
void displayHeader() {
    printf("\n=====================================  PHẦN MỀM TẠO PHÒNG TRUNG CƯ  =====================================\n\n");
}

//Đếm số tầng hiện có
void countFloors(int *floorCount) {

    //Mở thư mục FloorList
    struct dirent *dir;
    DIR *dp = opendir("./FloorList");

    //Kiểm tra thư mục có tồn tại không
    if (dp == NULL) {
        *floorCount = 0;
        closedir(dp);
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
        printf("Hiện chưa có tầng nào.\n");
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

    //Hỏi người dùng có chắc chắn muốn thêm tầng mới không
    char confirm;
    printf("Bạn có chắc chắn muốn thêm tầng mới không? (y/n): ");
    scanf(" %c", &confirm);

    //Nếu người dùng xác nhận muốn thêm tầng mới thì thực hiện tạo thư mục mới cho tầng mới
    if (confirm == 'y' || confirm == 'Y') {

        //Tạo thư mục mới cho tầng mới
        char path[256];
        int floorCount = 0;
        countFloors(&floorCount);
        sprintf(path, "./FloorList/Tang_%d", floorCount + 1);
        mkdir(path);
        printf("Tầng mới '%d' đã được tạo thành công.\n", floorCount + 1);
    } else {
        printf("Hủy thêm tầng mới.\n");
    }
}

//Lựa chọn thêm tầng hoặc thêm phòng
void chooseAddOption() {

    //Yêu cầu người dùng nhập lựa chọn
    int choice;
    printf("\nBạn muốn thêm gì? (1: Thêm tầng, 2: Thêm phòng): ");
    scanf("%d", &choice);

    //Xử lý lựa chọn của người dùng
    if (choice == 1) {
        //Gọi hàm thêm tầng
        addFloor();
    } else if (choice == 2) {
        //Gọi hàm thêm phòng
        //addRoom();
    } else {
        printf("Lựa chọn không hợp lệ. Vui lòng chọn lại.\n");
        chooseAddOption();
    }
}



//Hàm main
int main () {

    //chỉnh sửa lỗi front chữ 
	SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    //Hiển thị Header
    displayHeader ();

    //Đếm số tầng hiện có
    int floorCount = 0;
    countFloors (&floorCount);

    //Mở danh sách tầng
    openFloorList(floorCount);

    //Lựa chọn thêm tầng hoặc thêm phòng và gọi hàm tương ứng
    chooseAddOption();

    return 0;
}