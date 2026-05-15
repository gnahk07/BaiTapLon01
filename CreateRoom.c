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
    char year[5];
    char gender[5];
    char province[32];
}Resident;

//Hiển thị Header
void displayHeader() {
    printf("\n=====================================  PHẦN MỀM TẠO PHÒNG TRUNG CƯ  =====================================\n\n");
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
        printf("Không mở được Folder tầng %d.\n", selectFloor);
    }

    //Tạo biến đếm số phòng xem có phòng nào không
    int roomCount = 0;

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

//Kiểm tra cccd có trùng với cccd nào khác không
int checkDuplicates(char CCCD[]) {

    //Mở file chứa CCCD đã có trong trung cư
    FILE *f = fopen("Data/cccd.txt", "r");
    if (f == NULL) {
        printf("Khong mo duoc file!\n");
        return 0;
    }

    //Đọc từng dòng trong file
    char cccdInFile[13];
    while (fgets(cccdInFile, sizeof(cccdInFile), f) != NULL) {

        //Xóa ký tự xuống dòng
        cccdInFile[strcspn(cccdInFile, "\n")] = '\0';

        //So sánh có trùng không
        if (strcmp(CCCD, cccdInFile) == 0) {
            fclose(f);
            return 0;
        }
    }

    //Đóng file chứa CCCD đã có trong trung cư
    fclose(f);
    return 1;
}

//Kiểm tra căn cước công dân có hợp lệ không
int checkCCCD(char year[], char province[], char CCCD[], char gender[]) {

    //Kiểm tra có đủ 12 ký tự không
    if (strlen(CCCD) != 12) return 0;

    //Kiểm tra có phải tất cả là số không
    if (!isAllDigits(CCCD)) return 0;

    //Kiểm tra CCCD có trùng với ai không
    if (!checkDuplicates(CCCD)) return 0;

    //Kiểm tra mã tỉnh
    int Province = (CCCD[0] - '0') * 100 + (CCCD[1] - '0') * 10 + (CCCD[2] - '0');
    if (!validProvince(Province)) return 0;
    
    //Lưu quê quán
    char path[256];
    sprintf(path, "Data/Province/%03d.txt", Province);
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
    if (Gender % 2 == 0) strcpy(gender, "Nam");
    else strcpy(gender, "Nữ");

    //Kiểm tra năm sinh
    int Year = (CCCD[4] - '0') * 10 + (CCCD[5] - '0');
    if (Gender == 0 || Gender == 1) sprintf(year, "19%d", Year);
    if (Gender == 2 || Gender == 3) sprintf(year, "20%d", Year);

    return 1;
}

//Bình thường hóa tên
void normalizeName(char name[]) {

    //Chuyển toàn bộ tên thành chữ thường
    for (int i = 0; name[i]; i++) {
        name[i] = tolower(name[i]);
    }

    //Viết hoa chữ cái đầu tiên
    if (name[0] != '\0') {
        name[0] = toupper(name[0]);
    }

    //Viết hoa các chữ cái sau dấu cách
    for (int i = 1; name[i]; i++) {
        if (name[i - 1] == ' ') {
            name[i] = toupper(name[i]);
        }
    }
}

//Nhập thông tin cư dân
void inputResidentInformation(char *roomPath) {
    
    //Nhập số lượng thành viên và kiểm tra
    int n;
    char check[10];
    while (1) {
        printf("Nhập số lượng thành viên trong gia đình [1, 5]: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &n) == 1 && n >= 1 && n <= 5) break;
    }

    //Khai báo hàm cư dân
    Resident *resident = malloc(n * sizeof(Resident));

    //Tạo vòng lặp để nhập thông tin từng cư dân
    for (int i = 0; i < n; i++) {

        //Nhập tên cư dân
        printf("Nhập tên người thứ %d: ", i + 1);
        fgets(resident[i].name, sizeof(resident[i].name), stdin);
        resident[i].name[strcspn(resident[i].name, "\n")] = '\0';

        //Nhập căn cước công dân và rút ra năm sinh, quê quán, giới tính
        while (1) {
            printf("Nhập CCCD (12 số): ");
            fgets(resident[i].CCCD, sizeof(resident[i].CCCD), stdin);
            resident[i].CCCD[strcspn(resident[i].CCCD, "\n")] = '\0';
            if (checkCCCD(resident[i].year, resident[i].province, resident[i].CCCD, resident[i].gender)) break;
        }

        //Chỉnh sửa tên. Chữ cái đầu viết thường, các chữ sau viết hoa
        normalizeName(resident[i].name);

        //Tạo đường dẫn đầy đủ để tạo file chứa thông tin cư dân
        char residentPath[256];
        sprintf(residentPath, "%s/%s.txt", roomPath, resident[i].CCCD);

        //Tạo file .txt để lưu thông tin của cư dân
        FILE *f1 = fopen (residentPath, "w");

        //Kiểm tra có tạo được file không
        if (f1 == NULL) {
            printf("Không tạo được File để chứa cư dân\n");
            return;
        }
        
        //Lưu thông tin cư dân
        fprintf(f1, "%s\n%s\n%s\n%s", resident[i].name, resident[i].year, resident[i].province, resident[i].gender);

        //Đóng file
        fclose (f1);

        //Lưu CCCD vào danh sách
        FILE *f2 = fopen ("Data/cccd.txt", "a");
        fprintf(f2, "%s\n", resident[i].CCCD);
        fclose (f2);

        while (getchar() != '\n');
    }

    //Giải phóng bộ nhớ
    free(resident);
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
        sprintf(roomPath, "./FloorList/Tang_%d/P%d%02d", selectFloor, selectFloor, roomOrder);

        //Mở folder
        DIR *dp = opendir(roomPath);

        //Kiểm tra Folder có tồn tại chưa, nếu chưa thì tạo Folder phòng
        if (dp != NULL) {
            closedir(dp);
            printf("Phòng P%d%02d đã tồn tại.\n\n", selectFloor, roomOrder);
        } else {
            _mkdir(roomPath);
            printf("Phòng P%d%02d đã được tạo thành công.\n\n", selectFloor, roomOrder);

            //Nhập thông tin cư dân
            inputResidentInformation(roomPath);
        }
        
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
    createDataFolder();

    //Lựa chọn thêm tầng hoặc thêm phòng và gọi hàm tương ứng
    chooseAddOption();

    return 0;
}