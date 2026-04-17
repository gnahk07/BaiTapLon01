//Thư viện sử dụng
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dirent.h>
#include <direct.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

//Định nghĩa cấu trúc cư dân
typedef struct Resident {
	char name[64];
	char CCCD[13];
}Resident;

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
    sprintf(path, "./FloorList/Tang_%d", selectFloor);
    struct dirent *dir;
    DIR *dp = opendir(path);

    //Kiểm tra có phòng nào không
    if (dp == NULL) {
        printf("Không có phòng nào ở tầng %d.\n", selectFloor);
    } else {
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

//Kiểm tra có phải tất cả là số không
int isAllDigits(char *s) {
    for (int i = 0; s[i]; i++) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}

//Kiểm tra mã tỉnh
int validProvince(int CCCD) {

    //Mã tỉnh hợp lệ
    int provinces[] = {
        1, 2, 4, 6, 8, 10, 11, 12, 14, 15,
        17, 19, 20, 22, 24, 25, 26, 27, 30,
        31, 33, 34, 35, 36, 37, 38, 40, 42,
        44, 45, 46, 48, 49, 51, 52, 54, 56,
        58, 60, 62, 64, 66, 67, 68, 70, 72,
        74, 75, 77, 79, 80, 82, 83, 84, 86,
        87, 89, 91, 92, 93, 94, 95, 96
    };

    //Lấy số lượng mã tỉnh
    int n = sizeof (provinces)/sizeof(provinces[0]);

    //Kiểm tra mã có hợp lệ không
    for (int i = 0; i < n; i++) {
        if (CCCD == provinces[i]) return 1;
    }
    return 0;
}

//Kiểm tra căn cước công dân có hợp lệ không
int checkCCCD(char year[], char province[], char CCCD[], char gender[]) {

    //Kiểm tra có đủ 12 ký tự không
    if (strlen(CCCD) != 12) return 0;

    //Kiểm tra có phải tất cả là số không
    if (!isAllDigits(CCCD)) return 0;

    //Kiểm tra mã tỉnh
    int Province = (CCCD[0] - '0') * 100 + (CCCD[1] - '0') * 10 + (CCCD[2] - '0');
    if (!validProvince(Province)) return 0;
    
    //Lưu quê quán
    char path[256];
    sprintf(path, "Province/%d", Province);
    FILE *f = fopen (path, "r");
    if (f == NULL) {
        strcpy(province, "Không tìm thấy quê quán");
    } else {
        fgets(province, 32, f);
        province[strcspn(province, "\n")] = '\0';
        fclose(f);
    }

    //Kiểm tra giới tính
    int Gender = CCCD[3] - '0';
    if (Gender < 0 || Gender > 3) return 0;
    if (Gender / 2 == 0) strcpy(gender, "Nam");
    else strcpy(gender, "Nữ");

    //Kiểm tra năm sinh
    int Year = (CCCD[4] - '0') * 10 + (CCCD[5] - '0');
    if (Gender == 0 || Gender == 1) sprintf(year, "19%d", Year);
    if (Gender == 2 || Gender == 3) sprintf(year, "20%d", Year);

    return 1;
}

//Nhập thông tin cư dân
void inputResidentInformation() {
    
    //Nhập số lượng thành viên và kiểm tra
    int n;
    char check[10];
    while (1) {
        printf("Nhập số lượng thành viên trong gia đình [1, 5]: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &n) == 1 && n >= 1 && n <= 5) break;
    }

    //Khai báo hàm cư dân
    Resident resident[n];

    //Tạo vòng lặp để nhập thông tin từng cư dân
    for (int i = 0; i < n; i++) {

        //Nhập tên cư dân
        getchar();
        printf("Nhập tên người thứ %d: ", i + 1);
        fgets(resident[i].name, sizeof(resident[i].name), stdin);
        resident[i].name[strcspn(resident[i].name, "\n")] = '\0';

        //Nhập căn cước công dân và rút ra năm sinh, quê quán, giới tính
        char year[5];
        char gender[5];
        char province[32];
        while (1) {
            printf("Nhập CCCD (12 số): ");
            fgets(resident[i].CCCD, sizeof(resident[i].CCCD), stdin);
            resident[i].CCCD[strcspn(resident[i].CCCD, "\n")] = '\0';
            if (checkCCCD(year, province, resident[i].CCCD, gender)) break;
        }

        //Tạo file .txt để lưu
        FILE 
    }
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
        char roomPath[256];
        sprintf(roomPath, "./FloorList/Tang_%d/P%d%02d.txt", selectFloor, selectFloor, roomOrder);

        //Tạo file chứa dữ liệu
        FILE *fp = fopen(roomPath, "r");
        if (fp != NULL) {
            fclose(fp);
            printf("Phòng P%d%02d đã tồn tại.\n\n", selectFloor, roomOrder);
        } else {
            fp = fopen(roomPath, "w");
            if (fp != NULL) {
                fclose(fp);
                printf("Phòng P%d%02d đã được tạo thành công.\n\n", selectFloor, roomOrder);
            } else {
                printf("Không thể tạo phòng. Vui lòng kiểm tra quyền truy cập.\n\n");
            }
        }

        //Nhập thông tin cư dân
        inputResidentInformation();
        
    } else {
        //Cho người dùng chọn lại phòng
        printf("Tầng bạn chọn không tồn tại, chọn lại tầng.\n");
        addRoom(floorCount);
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