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

    //Kiểm tra thư mục có tồn tại không. Nếu không tồn tại thì tạo thư mục FloorList
    if (dp == NULL) {

        //Tạo thư mục FloorList
        char path[25] = "FloorList";
        mkdir(path);

        //Đặt số tầng bằng 0
        *floorCount = 0;
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

        //Xác nhận tạo thư mục mới thành công
        printf("Tầng mới '%d' đã được tạo thành công.\n", floorCount + 1);
    } else {
        printf("Hủy thêm tầng mới.\n");
    }
}

//Hàm kiểm tra lựa chọn tầng để thêm phòng có phù hợp không
void checkSelectFloor(int selectFloor, int floorCount, int *check) {

    //Kiểm tra lựa chọn
    for (int i = 1; i <= floorCount; i++) {
        if (selectFloor == i) {
            *check = 1;
        }
    }
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
        printf("Không có phòng nào ở tầng %d.\n", selectFloor);
        return;
    }

    //In các phòng hiện có
    printf("Hiện có sẵn các phòng: ");
    while ((dir = readdir(dp)) != NULL) {

        //Lấy tên File và tính độ dài của file
        char *name = dir->d_name;
        int len = strlen(name);

        //Kiểm tra tên phòng với đuôi file là .txt và in ra tên phòng
        if (len > 4 && strcmp(name + len - 4, ".txt") == 0) {
            printf("%.*s, ", len - 4, name);
        }
    }

    //Xóa dấu phẩy cuối cùng và thay bằng dấu chấm
    printf("\b\b.");

    //Đóng thư mục sau khi in dãy phòng xong
    closedir(dp);
}

//Hàm thêm phòng
void addRoom(int floorCount) {

    //In các tầng hiện có
    printf ("");

    //Lựa chọn tầng muốn thêm phòng
    int selectFloor;
    printf("Chọn tầng muốn thêm phòng: ");
    scanf("%d", &selectFloor);

    //Kiểm tra lựa chọn có phù hợp không
    int check = 0;
    checkSelectFloor(selectFloor, floorCount, &check);

    //Kiểm tra lựa chọn có phù hợp không nếu đã phù hợp thì tạo phòng
    if (check == 1) {
        //Gọi hàm in ra các dãy phòng đã có sẵn
        displayRoom(selectFloor);

        //Nhập tên phòng
        char roomName[128];
        printf("\nNhập tên phòng mới: ");
        scanf(" %127s", roomName);

        //Tạo đường dẫn đầy đủ đến nơi chứa tên phòng
        char roomPath[256];
        sprintf(roomPath, "./FloorList/Tang_%d/%s.txt", selectFloor, roomName);

        //Tạo file chứa dữ liệu
        FILE *fp = fopen(roomPath, "r");
        if (fp != NULL) {
            fclose(fp);
            printf("Phòng %s đã tồn tại.\n", roomName);
        } else {
            fp = fopen(roomPath, "w");
            if (fp != NULL) {
                fclose(fp);
                printf("Phòng %s đã được tạo thành công.\n", roomName);
            } else {
                printf("Không thể tạo phòng. Vui lòng kiểm tra quyền truy cập.\n");
            }
        }
    } else {
        //Cho người dùng chọn lại phòng
        printf("Tầng bạn chọn không tồn tại, chọn lại tầng.\n");
        addRoom(floorCount);
    }
}

//Lựa chọn thêm tầng hoặc thêm phòng
void chooseAddOption(int floorCount) {

    //Yêu cầu người dùng nhập lựa chọn
    int choice;
    printf("\nBạn muốn thêm gì? (1: Thêm tầng, 2: Thêm phòng): ");
    scanf("%d", &choice);

    //Xử lý lựa chọn của người dùng
    if (choice == 1) {
        //Gọi hàm thêm tầng
        addFloor();
    } else if (choice == 2) {
        //Kiểm tra đã có tầng nào chưa
        if (floorCount == 0) {
            printf("Hiện không có tầng nào. Hãy tạo thêm tầng để tạo phòng.");
            chooseAddOption(floorCount);
        } else {
            //Gọi hàm thêm phòng
            addRoom(floorCount);
        }
    } else {
        printf("Lựa chọn không hợp lệ. Vui lòng chọn lại.\n");
        chooseAddOption(floorCount);
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
    chooseAddOption(floorCount);

    return 0;
}