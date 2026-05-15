//Thư viện sử dụng
#include "Utils.h"
#include "Floor.h"
#include "Room.h"
#include "Menu.h"
#include <stdio.h>      // printf, sprintf, fopen, fclose, fprintf
#include <stdlib.h>     // srand, rand, system
#include <string.h>     // strcmp
#include <time.h>       // time
#include <windows.h>    // Sleep
#include <dirent.h>     // DIR, opendir, readdir, closedir
#include <direct.h>     // _mkdir (Windows)
#include <sys/stat.h>   // struct stat, stat, S_ISDIR
//#include <wchar.h>      // Hỗ trợ tiếng Việt khi in bảng

//Hàm fake loading
void fakeLoading() {

    //Khởi tạo random
    srand(time(NULL));

    system("cls");
    printf("Đang lấy dữ liệu hệ thống.\n");

    //Chạy từ 0 -> 100%
    for (int i = 0; i <= 100; i++) {

        //Đưa con trỏ về đầu dòng
        printf("\r[");

        int j;

        //In phần đã hoàn thành
        for (j = 0; j < i / 5; j++) {
            printf("■");
        }

        //In phần còn lại
        for (; j < 20; j++) {
            printf(" ");
        }

        //In phần trăm
        printf("] %d%%", i);

        //Flush để in ngay lập tức
        fflush(stdout);

        //Giai đoạn đầu nhanh
        if (i < 30) {
            Sleep(rand() % 30 + 10);
        }

        //Giữa hơi chậm
        else if (i < 50) {
            Sleep(rand() % 80 + 40);
        }

        //Cuối nhanh
        else if (i < 75) {
            Sleep(rand() % 150 + 30);
        }

        //Fake stuck 99%
        else if (i == 99) {

            //Dừng lâu hơn
            Sleep(1000);
        }

        //100%
        else {
            Sleep(100);
        }
    }

    //In đọc dữ liệu thành công và làm sạch màn hình
    printf("\n");
    printf("Đọc dữ liệu thành công!!\n");
    Sleep(1000);
    system("cls");
}

//Tạo nơi chứa dữ liệu tỉnh thành
void initializeProvinceFiles() {

    //Tạo thư mục Province nếu chưa tồn tại
    DIR *dp = opendir("Data/Province");
    if (dp == NULL) {
        _mkdir("Data/Province");
    } else {
        closedir(dp);
    }

    //Danh sách mã tỉnh và tên tỉnh
    struct Province {
        int code;
        char name[64];
    };

    //Danh sách đầy đủ mã tỉnh và tên tỉnh
    struct Province provinces[] = {
        {1, "Hà Nội"},
        {2, "Hà Giang"},
        {4, "Cao Bằng"},
        {6, "Bắc Kạn"},
        {8, "Tuyên Quang"},
        {10, "Lào Cai"},
        {11, "Điện Biên"},
        {12, "Lai Châu"},
        {14, "Sơn La"},
        {15, "Yên Bái"},
        {17, "Hòa Bình"},
        {19, "Thái Nguyên"},
        {20, "Lạng Sơn"},
        {22, "Quảng Ninh"},
        {24, "Bắc Giang"},
        {25, "Phú Thọ"},
        {26, "Vĩnh Phúc"},
        {27, "Bắc Ninh"},
        {30, "Hải Dương"},
        {31, "Hải Phòng"},
        {33, "Hưng Yên"},
        {34, "Thái Bình"},
        {35, "Hà Nam"},
        {36, "Nam Định"},
        {37, "Ninh Bình"},
        {38, "Thanh Hóa"},
        {40, "Nghệ An"},
        {42, "Hà Tĩnh"},
        {44, "Quảng Bình"},
        {45, "Quảng Trị"},
        {46, "Thừa Thiên Huế"},
        {48, "Đà Nẵng"},
        {49, "Quảng Nam"},
        {51, "Quảng Ngãi"},
        {52, "Bình Định"},
        {54, "Phú Yên"},
        {56, "Khánh Hòa"},
        {58, "Ninh Thuận"},
        {60, "Bình Thuận"},
        {62, "Kon Tum"},
        {64, "Gia Lai"},
        {66, "Đắk Lắk"},
        {67, "Đắk Nông"},
        {68, "Lâm Đồng"},
        {70, "Bình Phước"},
        {72, "Tây Ninh"},
        {74, "Bình Dương"},
        {75, "Đồng Nai"},
        {77, "Bà Rịa - Vũng Tàu"},
        {79, "Hồ Chí Minh"},
        {80, "Long An"},
        {82, "Tiền Giang"},
        {83, "Bến Tre"},
        {84, "Trà Vinh"},
        {86, "Vĩnh Long"},
        {87, "Đồng Tháp"},
        {89, "An Giang"},
        {91, "Kiên Giang"},
        {92, "Cần Thơ"},
        {93, "Hậu Giang"},
        {94, "Sóc Trăng"},
        {95, "Bạc Liêu"},
        {96, "Cà Mau"}
    };

    //Lấy số lượng phần tử trong mảng provinces
    int n = sizeof(provinces) / sizeof(provinces[0]);

    //Tạo file cho từng tỉnh
    for (int i = 0; i < n; i++) {

        char path[256];

        //Tạo địa chỉ để đến nơi chứa File
        sprintf(path, "Data/Province/%03d.txt", provinces[i].code);

        //Kiểm tra file tồn tại chưa
        FILE *f = fopen(path, "r");

        //Nếu chưa tồn tại thì tạo file
        if (f == NULL) {

            f = fopen(path, "w");

            if (f != NULL) {
                fprintf(f, "%s", provinces[i].name);
                fclose(f);
            }
        } else {
            fclose(f);
        }
    }
}

//Kiểm tra thư mục Data có tồn tại không, nếu không thì tạo
void createDataFolder() {

    //Kiểm tra thư mục Data có tồn tại chưa
    DIR *dp = opendir("Data");

    //Nếu chưa tồn tại thì tạo thư mục
    if (dp == NULL) {
        _mkdir("Data");
    } else {
        closedir(dp);
    }

    //Tạo file cccd.txt
    FILE *f = fopen("Data/cccd.txt", "a");

    if (f != NULL) {
        fclose(f);
    }

    //Tạo file Province
    initializeProvinceFiles();
}

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

//Hàm kiểm tra lựa chọn tầng để thêm phòng có phù hợp không
void checkSelectFloor(int selectFloor, int floorCount, int *check) {

    //Kiểm tra lựa chọn
    if (selectFloor >= 1 && selectFloor <= floorCount){
        *check = 1;
    }
}

//Lựa chọn thêm tầng hoặc thêm phòng
void chooseAddOption() {

    //Đếm số tầng hiện có
    int floorCount = 0;
    countFloors (&floorCount);

    //Yêu cầu người dùng nhập lựa chọn và kiểm tra
    int choice;
    char check[10];

    //In bảng tính năng
    drawBorderTop();
    drawRow("Tính Năng Thêm Tầng/Phòng");
    drawBorderMiddle();
    drawRow("1. Thêm tầng");
    drawRow("2. Thêm phòng");
    drawRow("0. Thoát");
    drawBorderBottom();

    //Nhận lựa chọn người dùng
    while (1) {
        printf("->Lựa chọn của bạn: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &choice) == 1) break;
    }

    //Xử lý lựa chọn của người dùng
    if (choice == 1) {
        addFloor(floorCount);         //Gọi hàm thêm tầng
        Sleep(2000);
    } else if (choice == 2) {
        //Kiểm tra đã có tầng nào chưa
        if (floorCount == 0) {
            printf("Hiện không có tầng nào. Hãy tạo thêm tầng để tạo phòng.");
        } else {
            openFloorList(floorCount);
            addRoom(floorCount);        //Gọi hàm thêm phòng
            Sleep(2000);
        }
    } else if (choice == 0) {
        return;
    } else {
        printf("Lựa chọn không lợp lệ");
        Sleep(2000);
    }
}
