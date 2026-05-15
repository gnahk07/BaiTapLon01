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
    printf("\n=====================================  CHỨC NĂNG TẠO TẦNG/PHÒNG CHUNG CƯ  =====================================\n\n");
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
    if (strlen(CCCD) != 12) {
        printf("Nhập lại CCCD có 12 số: ");
        return 0;
    }
    //Kiểm tra có phải tất cả là số không
    if (!isAllDigits(CCCD)) {
        printf("Nhập lại CCCD chỉ toàn là số: ");
        return 0;
    }

    //Kiểm tra CCCD có trùng với ai không
    if (!checkDuplicates(CCCD)) {
        printf("Nhập lại CCCd vì đã bị trùng: ");
        return 0;
    }

    //Kiểm tra mã tỉnh
    int Province = (CCCD[0] - '0') * 100 + (CCCD[1] - '0') * 10 + (CCCD[2] - '0');
    if (!validProvince(Province)) {
        printf("Nhập lại CCCD: ");
        return 0;
    }
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
    if (Gender < 0 || Gender > 3) {
        printf("Nhập lại CCCD: ");
        return 0;
    }
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
void inputResidentInformation() {
    
    //Nhập tầng và phòng muốn thêm
    int selectFloor;
    char check[10];
    while (1) {
    	printf("Nhập tầng muốn thêm: ");
    	fgets(check, sizeof(check), stdin);
    	if (sscanf(check, "%d", &selectFloor) == 1) break;
	}
    
    //Nhập số phòng muốn thêm
    int selectRoom;
    displayRoom(selectFloor);
    while (1) {
    	printf("\nNhập số phòng muốn thêm: ");
    	fgets(check, sizeof(check), stdin);
    	if (sscanf(check, "%d", &selectRoom) == 1) break;
	}
	
	//Mở File phòng
	char roomPath[256];
	sprintf(roomPath, "FloorList/Tang_%d/P%d%02d", selectFloor, selectFloor, selectRoom);
	DIR *dp = opendir (roomPath);
	
	//Kiểm tra Folder có tồn tại không
    if (dp == NULL) {
    	closedir(dp);
    	printf("Phòng P%d%02d không tồn tại.\n\n", selectFloor, selectRoom);
    	return;
	}
    
    //Nhập số lượng thành viên và kiểm tra
    int n;
    while (1) {
        printf("Nhập số lượng muốn thêm: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &n)) break;
    }

    //Khai báo hàm cư dân
    Resident *resident = malloc(n * sizeof(Resident));
    for (int i = 0; i < n; i++) {
    	//Nhập tên cư dân
        printf("Nhập tên người thứ %d: ", i + 1);
        fgets(resident[i].name, sizeof(resident[i].name), stdin);
        resident[i].name[strcspn(resident[i].name, "\n")] = '\0';

        //Nhập căn cước công dân và rút ra năm sinh, quê quán, giới tính
        printf("Nhập CCCD: ");
        while (1) {
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
    } else if (choice == 2) {
        //Kiểm tra đã có tầng nào chưa
        if (floorCount == 0) {
            printf("Hiện không có tầng nào. Hãy tạo thêm tầng để tạo phòng.");
        } else {
            //Gọi hàm thêm phòng
            addRoom(floorCount);
        }
    } else {
        printf("Lựa chọn không hợp lệ. Vui lòng chọn lại.\n\n");
    }
}

void handleSearchProcess() {
    int subChoice;
    char keyword[64];

    printf("\n---------- CHỨC NĂNG TÌM KIẾM THÔNG TIN ----------");
    printf("\n1. Tìm theo CCCD");
    printf("\n2. Tìm theo Tên");
    printf("\n0. Quay lại");
    printf("\nLựa chọn của bạn: ");
    if (scanf("%d", &subChoice) != 1) { // Kiểm tra nhập liệu không phải số
        fflush(stdin);
        return;
    }
    fflush(stdin); // Xóa bộ nhớ đệm sau scanf

    if (subChoice == 1) {
        printf("Nhập số CCCD cần tìm: ");
        fgets(keyword, sizeof(keyword), stdin);
        cleanString(keyword); // Làm sạch từ khóa
        searchByCCCD(keyword); // Gọi hàm tìm theo CCCD
    } else if (subChoice == 2) {
        printf("Nhập tên cư dân cần tìm: ");
        fgets(keyword, sizeof(keyword), stdin);
        cleanString(keyword);
        searchByName(keyword); // Gọi hàm tìm theo tên
    }
}

// Hàm chuyển toàn bộ chuỗi thành chữ hoa để tìm kiếm không phân biệt hoa thường
void toUpperCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]); // Chuyển từng ký tự sang chữ hoa
    }
}

// Hàm xóa các ký tự xuống dòng và khoảng trắng thừa ở cuối chuỗi
void cleanString(char *str) {
    str[strcspn(str, "\r\n")] = 0; // Tìm vị trí ký tự xuống dòng và thay bằng ký tự kết thúc chuỗi
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = 0; // Cắt bỏ các dấu cách vô hình ở cuối
    }
}

// Hàm loại bỏ ký tự BOM (3 byte đặc biệt đầu file UTF-8 thường do Notepad tạo ra)
void removeBOM(char *str) {
    if ((unsigned char)str[0] == 0xEF && (unsigned char)str[1] == 0xBB && (unsigned char)str[2] == 0xBF) {
        memmove(str, str + 3, strlen(str + 3) + 1); // Dịch chuyển chuỗi lên 3 byte để đè lên BOM
    }
}

// Hàm mở file để đọc tên khách hàng ở dòng đầu tiên
void getNameFromFile(char *filePath, char *nameOut) {
    FILE *f = fopen(filePath, "r"); // Mở file hồ sơ ở chế độ đọc
    if (f != NULL) {
        if (fgets(nameOut, 64, f)) { // Đọc tối đa 64 ký tự dòng đầu
            removeBOM(nameOut);      // Xóa BOM nếu có
            cleanString(nameOut);    // Làm sạch chuỗi
        }
        fclose(f); // Đóng file sau khi đọc
    } else {
        strcpy(nameOut, "Không rõ"); // Trường hợp file lỗi
    }
}

// Hàm tìm kiếm theo CCCD
void searchByCCCD(char *searchKey) {
    int foundCount = 0; // Biến đếm số kết quả
    DIR *mainDir = opendir("FloorList"); // Mở thư mục gốc
    if (mainDir == NULL) return;

    printf("\n%-25s | %-15s | %-10s\n", "Họ tên", "Số CCCD", "Số Phòng");
    printf("----------------------------------------------------------------------\n");

    struct dirent *floorEntry;
    while ((floorEntry = readdir(mainDir)) != NULL) { // Quét qua từng Tầng
        if (strstr(floorEntry->d_name, "Tang_")) {
            char floorPath[256];
            sprintf(floorPath, "FloorList/%s", floorEntry->d_name);
            DIR *floorDir = opendir(floorPath);
            if (floorDir == NULL) continue;

            struct dirent *roomEntry;
            while ((roomEntry = readdir(floorDir)) != NULL) { // Quét qua từng Phòng
                if (roomEntry->d_name[0] == 'P') {
                    char roomPath[512];
                    sprintf(roomPath, "%s/%s", floorPath, roomEntry->d_name);
                    DIR *roomDir = opendir(roomPath);
                    if (roomDir == NULL) continue;

                    struct dirent *fileEntry;
                    while ((fileEntry = readdir(roomDir)) != NULL) { // Quét qua từng File
                        if (strstr(fileEntry->d_name, ".txt")) {
                            char currentCCCD[20];
                            strcpy(currentCCCD, fileEntry->d_name);
                            currentCCCD[strlen(currentCCCD) - 4] = '\0'; // Lấy tên file bỏ đuôi .txt

                            // So sánh chính xác số CCCD với tên file
                            if (strcmp(currentCCCD, searchKey) == 0) {
                                char filePath[1024], customerName[64];
                                sprintf(filePath, "%s/%s", roomPath, fileEntry->d_name);
                                getNameFromFile(filePath, customerName); // Lấy tên từ bên trong file
                                printf("%-25s | %-15s | %-10s\n", customerName, currentCCCD, roomEntry->d_name);
                                foundCount++;
                            }
                        }
                    }
                    closedir(roomDir);
                }
            }
            closedir(floorDir);
        }
    }
    closedir(mainDir);
    if (foundCount == 0) printf("Không tìm thấy cư dân có CCCD: %s\n", searchKey);
    else printf("=> Tìm thấy %d kết quả.\n", foundCount);
}

// Hàm tìm kiếm theo Tên
void searchByName(char *searchKey) {
    int foundCount = 0;
    char upperSearchKey[64];
    strcpy(upperSearchKey, searchKey);
    toUpperCase(upperSearchKey); // Chuyển từ khóa về chữ hoa để so sánh

    DIR *mainDir = opendir("FloorList");
    if (mainDir == NULL) return;

    printf("\n%-25s | %-15s | %-10s\n", "Họ tên", "Số CCCD", "Số Phòng");
    printf("----------------------------------------------------------------------\n");

    struct dirent *floorEntry;
    while ((floorEntry = readdir(mainDir)) != NULL) {
        if (strstr(floorEntry->d_name, "Tang_")) {
            char floorPath[256];
            sprintf(floorPath, "FloorList/%s", floorEntry->d_name);
            DIR *floorDir = opendir(floorPath);
            if (floorDir == NULL) continue;

            struct dirent *roomEntry;
            while ((roomEntry = readdir(floorDir)) != NULL) {
                if (roomEntry->d_name[0] == 'P') {
                    char roomPath[512];
                    sprintf(roomPath, "%s/%s", floorPath, roomEntry->d_name);
                    DIR *roomDir = opendir(roomPath);
                    if (roomDir == NULL) continue;

                    struct dirent *fileEntry;
                    while ((fileEntry = readdir(roomDir)) != NULL) {
                        if (strstr(fileEntry->d_name, ".txt")) {
                            char filePath[1024], customerName[64], upperName[64];
                            sprintf(filePath, "%s/%s", roomPath, fileEntry->d_name);
                            
                            getNameFromFile(filePath, customerName); // Đọc tên từ trong file
                            strcpy(upperName, customerName);
                            toUpperCase(upperName); // Chuyển tên đọc được thành chữ hoa

                            // Kiểm tra xem từ khóa có nằm trong tên khách hàng không
                            if (strstr(upperName, upperSearchKey) != NULL) {
                                char currentCCCD[20];
                                strcpy(currentCCCD, fileEntry->d_name);
                                currentCCCD[strlen(currentCCCD) - 4] = '\0'; // Lấy CCCD từ tên file

                                printf("%-25s | %-15s | %-10s\n", customerName, currentCCCD, roomEntry->d_name);
                                foundCount++;
                            }
                        }
                    }
                    closedir(roomDir);
                }
            }
            closedir(floorDir);
        }
    }
    closedir(mainDir);
    if (foundCount == 0) printf("Không tìm thấy cư dân có tên: %s\n", searchKey);
    else printf("=> Tìm thấy %d kết quả.\n", foundCount);
}

// Hàm điều hướng chính cho việc xóa thông tin
void handleCheckOutProcess()
{
    char floor[10], room[10], folderPath[256], displayRoom[15]; // Khai báo các chuỗi chứa thông tin tầng, phòng, đường dẫn
    int choice, fNum, rNum; // Khai báo các biến số để lưu lựa chọn và số tầng/phòng sau khi chuyển đổi

    printf("\n========== CHỨC NĂNG XÓA THÔNG TIN KHÁCH HÀNG ==========\n");

    // Kiểm tra số tầng (phải >= 1)
    do {
        printf("Nhập số tầng: "); 
        scanf("%s", floor); // Nhận dữ liệu tầng dưới dạng chữ
        fNum = atoi(floor); // Chuyển chữ thành số để kiểm tra
        if (fNum < 1) printf("[!] Số tầng không hợp lệ (phải >= 1). Vui lòng nhập lại!\n");
    } while (fNum < 1); // Nếu tầng nhỏ hơn 1 thì bắt nhập lại

    // Kiểm tra số phòng (phải >= 1)
    do {
        printf("Nhập số phòng: "); 
        scanf("%s", room); // Nhận dữ liệu phòng dưới dạng chữ
        rNum = atoi(room); // Chuyển chữ thành số để kiểm tra
        if (rNum < 1) printf("[!] Số phòng không hợp lệ (phải >= 1). Vui lòng nhập lại!\n");
    } while (rNum < 1); // Nếu phòng nhỏ hơn 1 thì bắt nhập lại

    // Tự động định dạng tên phòng (Ví dụ: tầng 1 phòng 5 -> P105)
    if (rNum < 10) 
    {
        // Nếu số phòng < 10, thêm số 0 ở giữa (Ví dụ: P105)
        sprintf(folderPath, "FloorList/Tang_%s/P%s0%s", floor, floor, room);
        sprintf(displayRoom, "P%s0%s", floor, room);
    } 
    else 
    {
        // Nếu số phòng >= 10, ghép bình thường (Ví dụ: P110)
        sprintf(folderPath, "FloorList/Tang_%s/P%s%s", floor, floor, room);
        sprintf(displayRoom, "P%s%s", floor, room);
    }

    printf("\n--- CHẾ ĐỘ XÓA (Phòng %s) ---", displayRoom);
    printf("\n1. Xóa một hoặc nhiều người (theo CCCD)");
    printf("\n2. Xóa toàn bộ phòng (Trả phòng sạch)");
    printf("\n0. Quay lại menu");
    printf("\nLựa chọn của bạn: ");
    scanf("%d", &choice); // Lưu lựa chọn chế độ xóa

    if (choice == 1) deleteManyPeople(folderPath, floor, displayRoom); // Gọi hàm xóa từng người
    else if (choice == 2) deleteAll(folderPath, floor, displayRoom); // Gọi hàm dọn sạch phòng
}

// Hàm xóa nhiều thông tin 
void deleteManyPeople(char *folderPath, char *floor, char *displayRoom) //*Đường dẫn thư mục, *tầng, *số phòng
{
    int num; 
    char CCCD[15], filePath[256], customerName[50];
    int countInRoom = 0; // Khởi tạo biến đếm số khách đang có trong phòng
    struct dirent *entry; // Con trỏ đọc file
    DIR *dir = opendir(folderPath); // Mở thư mục phòng để kiểm tra số lượng
    
    if (dir != NULL) { // Nếu mở thư mục thành công
        while ((entry = readdir(dir)) != NULL) { // Duyệt qua từng file
            if (strstr(entry->d_name, ".txt")) { // Nếu là file hồ sơ khách (.txt)
                countInRoom++; // Tăng biến đếm khách thực tế
            }
        }
        closedir(dir); // Đóng thư mục sau khi đếm xong
    }

    if (countInRoom == 0) { // Nếu trong phòng không có ai
        printf("[Thông báo] Phòng hiện tại đang trống, không có khách để xóa!\n");
        return; // Thoát hàm
    }

    // Kiểm tra số lượng người muốn xóa (phải >= 1 và < số người hiện có)
    do {
        printf("\n-> Hiện có %d khách. Bạn muốn xóa bao nhiêu khách hàng (số lượng ít hơn số hiện có): ", countInRoom, countInRoom - 1);
        if (scanf("%d", &num) != 1) // Kiểm tra nếu người dùng nhập chữ thay vì số
        { 
            printf("[Lỗi] Vui lòng nhập số nguyên.\n");
            fflush(stdin); // Dọn dẹp bộ nhớ đệm bàn phím để tránh lỗi
            num = -1; // Gán giá trị sai để tiếp tục vòng lặp
        }
        // Kiểm tra điều kiện: số xóa phải >= 1 VÀ phải nhỏ hơn tổng số người hiện có
        if (num != -1 && (num < 1 || num >= countInRoom)) 
        {
            printf("[!] Số lượng không hợp lệ! Để xóa %d người trở lên, vui lòng dùng chức năng 'Xóa toàn bộ phòng'.\n", countInRoom);
            printf("[!] Vui lòng nhập lại số lượng ít hơn %d.\n", countInRoom);
        }
    } while (num < 1 || num >= countInRoom); // Bắt nhập lại nếu không thỏa mãn điều kiện

    for (int i = 1; i <= num; i++) // Vòng lặp chạy từ 1 đến số lượng người cần xóa
    {
        printf("\n[Người thứ %d] Nhập số CCCD: ", i);
        scanf("%s", CCCD); // Nhập số CCCD (cũng là tên file)

        sprintf(filePath, "%s/%s.txt", folderPath, CCCD); // Ghép thành đường dẫn file hoàn chỉnh
        getCustomerName(filePath, customerName); // Tìm xem tên người này là gì trước khi xóa

        if (remove(filePath) == 0) // Lệnh xóa file vật lý trên ổ cứng
        {
            printf("[Thành công] Đã xóa hồ sơ: %s\n", customerName);
            removeFromGlobalData(CCCD); // Xóa dòng chứa CCCD này trong file tổng cccd.txt
        } 
        else printf("[Lỗi] Không tìm thấy file %s.txt tại phòng này.\n", CCCD);
    }
    displayStatus(folderPath); // In lại danh sách khách còn lại trong phòng
}

// Hàm xóa tất cả thông tin 
void deleteAll(char *folderPath, char *floor, char *displayRoom) //*Đường dẫn thư mục, *tầng, *số phòng
{
    char yes;
    printf("\n[CẢNH BÁO] Bạn có chắc muốn dọn sạch phòng %s? (y/n): ", displayRoom);
    fflush(stdin); // Xóa ký tự thừa trong bộ nhớ đệm
    scanf(" %c", &yes); // Nhận ký tự xác nhận y/n

    if (yes == 'y' || yes == 'Y') // Nếu đồng ý xóa hết
    {
        struct dirent *fileEntry;   // Con trỏ đại diện cho từng file tìm thấy
        DIR *directory = opendir(folderPath); //Thư mục = mở (đường dẫn thư mục) -> Mở thư mục phòng
        if (directory == NULL) return; // Nếu không mở được thư mục thì dừng luôn

        while ((fileEntry = readdir(directory)) != NULL) // Đọc từng mục trong thư mục
        {
            if (strstr(fileEntry->d_name, ".txt")) // Kiểm tra nếu là file văn bản .txt
            {
                char fullPath[256], cccdNumber[20];
                strcpy(cccdNumber, fileEntry->d_name); // Copy tên file vào biến tạm
                cccdNumber[strlen(cccdNumber) - 4] = '\0'; // Cắt bỏ đuôi ".txt" để lấy mỗi số CCCD
                
                sprintf(fullPath, "%s/%s", folderPath, fileEntry->d_name); // Tạo đường dẫn đầy đủ

                if (remove(fullPath) == 0) // Xóa file
                {
                    removeFromGlobalData(cccdNumber); // Cập nhật file tổng
                }
            }
        }
        closedir(directory); // Đóng thư mục sau khi quét xong
        printf("[Thành công] Đã dọn dẹp sạch toàn bộ phòng %s.\n", displayRoom);
    }
    displayStatus(folderPath); // Xem lại tình trạng phòng sau khi dọn
}

// Hàm hiển thị thông tin phòng sau khi xóa 
void displayStatus(char *folderPath) //*Đường dẫn thư mục
{
    struct dirent *fileEntry; // Con trỏ đại diện cho từng file tìm thấy
    DIR *directory = opendir(folderPath); //Thư mục = mở (đường dẫn thư mục) -> Mở thư mục phòng
    int count = 0; // Biến đếm số người trong phòng

    printf("\n--- TÌNH TRẠNG PHÒNG HIỆN TẠI ---");
    if (directory == NULL) 
    { 
        printf("\nPhòng trống (Không thấy thư mục).\n"); 
        return; 
    }

    while ((fileEntry = readdir(directory)) != NULL) // Đọc từng mục trong thư mục
    {
        if (strstr(fileEntry->d_name, ".txt")) // Nếu tìm thấy file có đuôi .txt
        {
            count++; // Tăng đếm người
            char filePath[512], customerName[50], cccdNumber[20];
            strcpy(cccdNumber, fileEntry->d_name); // Lấy tên file
            cccdNumber[strlen(cccdNumber) - 4] = '\0'; // Xóa đuôi .txt để lấy CCCD
            sprintf(filePath, "%s/%s", folderPath, fileEntry->d_name); // Ghép đường dẫn
            getCustomerName(filePath, customerName); // Đọc tên khách bên trong file
            printf("\n%d. Tên: %s | CCCD: %s", count, customerName, cccdNumber);
        }
    }
    closedir(directory); // Đóng thư mục
    if (count == 0) printf("\nPhòng trống"); // Nếu không tìm thấy file nào
    printf("\n---------------------------------\n");
}

// Hàm lấy tên khách hàng *Đường dẫn tệp
void getCustomerName(char *filePath, char *nameOut) //*tên đầu ra
{
    FILE *f = fopen(filePath, "r"); // Mở file khách hàng để đọc dữ liệu
    if (f != NULL) 
    {
        fgets(nameOut, 50, f); // Đọc dòng đầu tiên là họ tên
        nameOut[strcspn(nameOut, "\n")] = 0; // Xóa ký tự xuống dòng ở cuối tên
        fclose(f); // Đóng file
    } 
    else strcpy(nameOut, "Không rõ tên"); // Nếu file lỗi không mở được
}

// Hàm xóa dữ liệu cccd khỏi dữ liệu tổng 
void removeFromGlobalData(char *cccdToDelete) //*cccd cần xóa   
{
    FILE *f = fopen("Data/cccd.txt", "r"); // Mở file danh sách tổng để đọc
    if (f == NULL) return; // Nếu không có file tổng thì thôi

    FILE *fTemp = fopen("Data/temp.txt", "w"); // Tạo một file tạm mới để ghi dữ liệu lọc
    if (fTemp == NULL) 
    { 
        fclose(f); 
        return; 
    }

    char line[256]; 
    int found = 0; // Biến đánh dấu đã tìm thấy để xóa hay chưa
    while (fgets(line, sizeof(line), f)) // Đọc từng dòng trong file tổng
    {
        if (strstr(line, cccdToDelete) == NULL) 
        {
            fputs(line, fTemp); // Nếu không chứa CCCD cần xóa thì chép qua file tạm
        }
        else 
        {
            found = 1; // Nếu chứa CCCD cần xóa thì bỏ qua (không chép), đánh dấu là đã tìm thấy
        }
    }
    fclose(f); // Đóng file tổng
    fclose(fTemp); // Đóng file tạm

    remove("Data/cccd.txt"); // Xóa file tổng cũ
    rename("Data/temp.txt", "Data/cccd.txt"); // Đổi tên file tạm thành file tổng mới
    if (found) printf("-> Đã xóa dữ liệu khỏi hệ thống tổng Data/cccd.txt\n");
}

int main() {
    // Thiết lập tiếng Việt cho console Windows
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    createDataFolder();

    int menuChoice;

    do {
        printf("\n======= HỆ THỐNG QUẢN LÝ DÂN CƯ =======");
        printf("\n1. Chức năng thêm tầng/phòng");
        printf("\n2. Chức năng thêm người vào phòng");
        printf("\n3. Chức năng tìm kiếm thông tin");
        printf("\n4. Chức năng chỉnh sửa");
        printf("\n5. Chức năng xóa thông tin");
        printf("\nLựa chọn: ");
        
        // Nhận lựa chọn từ người dùng
        if (scanf("%d", &menuChoice) != 1) {
            fflush(stdin); // Xóa rác nếu người dùng nhập chữ
            continue;
        }
        fflush(stdin);

        switch (menuChoice) {
        	case 1: 
        		chooseAddOption(); // Gọi vào hàm tạo tầng/phòng
        		symtem(cls);
        		break;
        	case 2:
        		inputResidentInformation(); //Gọi vào hàm thêm cư dân
        		symtem(cls);
        		break;
            case 3:
                handleSearchProcess(); // Gọi vào hàm tìm kiếm thông tin
                symtem(cls);
                break;
            case 4:
            	break;
            case 5:
            	handleCheckOutProcess(); //Gọi vào hàm xóa thông tin
            	symtem(cls);
            	break;
            default:
                printf("\nLựa chọn không hợp lệ, vui lòng chọn lại!\n");
        }
    } while (menuChoice != 0);

    return 0;
}