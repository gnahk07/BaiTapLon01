//Thư viện sử dụng
#include "Utils.h"
#include "Menu.h"
#include "Floor.h"
#include "Room.h"
#include <stdio.h>      // printf, scanf, fgets, sscanf, fopen, fclose, fgets, fputs, rename, remove
#include <stdlib.h>     // atoi
#include <string.h>     // strcmp, strstr, strcpy, strlen, strcspn
#include <dirent.h>     // DIR, opendir, readdir, closedir
#include <direct.h>     // _rmdir (Windows)
#include <windows.h>    // Sleep

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
    if (found) {
        setColor(14);
		printf("Đã xóa %s khỏi hệ thống tổng Data\n", cccdToDelete);
        setColor(7);
    }
}

//Hàm xóa phòng
void deleteRoom(int floorCount) {

    //Khai báo hàm
    int floor, room;
    char check[10];

    //Nhập tầng
    openFloorList(floorCount);
    while (1) {
        printf("\nChọn tầng muốn xóa phòng: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &floor)) break;
    }

    //Kiểm tra có hợp lệ không 
    if (floor <= 0 || floor > floorCount) {
        setColor(12);
        printf("[Lỗi]. Tầng không hợp lệ.");
        setColor(7);
        return;
    }

    //Nhập số phòng
    displayRoom(floor);
    if (checkHaveRoom(floor) == 0) return;
    while (1) {
        printf("\nChọn phòng muốn xóa: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &room)) break;
    }

    //Kiểm tra phòng hợp lệ
    if (room < 1 || room > 99) {
        setColor(12);
        printf("[Lỗi]. Phòng không hợp lệ.\n");
        setColor(7);
        return;
    }

    //Tạo đường dẫn phòng
    char path[256];
    sprintf(path, "FloorList/Tang_%d/P%d%02d", floor, floor, room);

    //Kiểm tra folder phòng có tồn tại không
    DIR *dp = opendir(path);
    if (dp == NULL) {
        setColor(12);
        printf("[Lỗi]. Phòng không tồn tại\n");
        setColor(7);
        return;
    }
    struct dirent *entry;
    int hasResident = 0;

    //Kiểm tra có file cư dân không
    while ((entry = readdir(dp)) != NULL) {

        //Bỏ qua "." và ".."
        if (strcmp(entry->d_name, ".") != 0 &&
            strcmp(entry->d_name, "..") != 0) {

            //Nếu có file txt => có cư dân
            if (strstr(entry->d_name, ".txt")) {
                hasResident = 1;
                break;
            }
        }
    }

    closedir(dp);

    //Nếu có cư dân thì hỏi xác nhận
    if (hasResident) {
        char confirm;
        setColor(12);
        printf("\n[!] Phòng vẫn còn cư dân.\n");
        setColor(7);
        while (1) {
            printf("-> Bạn có muốn xóa toàn bộ cư dân và phòng không? (y/n): ");
            fgets(check, sizeof(check), stdin);
            if (sscanf(check, " %c", &confirm) == 1 && 
                (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N')) {
                break;
            }
        }

        //Nếu không đồng ý
        if (confirm == 'n' || confirm == 'N') {

            setColor(14);
            printf("Đã hủy xóa phòng.\n");
            setColor(7);
            return;
        }

        //Nếu đồng ý thì xóa toàn bộ file txt
        dp = opendir(path);

        if (dp == NULL) {
            setColor(12);
            printf("[Lỗi]. Không thể mở lại phòng.\n");
            setColor(7);
            return;
        }

        while ((entry = readdir(dp)) != NULL) {

            if (strcmp(entry->d_name, ".") != 0 &&
                strcmp(entry->d_name, "..") != 0) {

                if (strstr(entry->d_name, ".txt")) {

                    char fullPath[512];
                    char cccd[50];

                    //Lấy CCCD từ tên file
                    strcpy(cccd, entry->d_name);
                    cccd[strlen(cccd) - 4] = '\0';

                    //Tạo đường dẫn file
                    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

                    //Xóa file cư dân
                    remove(fullPath);

                    //Xóa khỏi dữ liệu tổng
                    removeFromGlobalData(cccd);
                }
            }
        }

        closedir(dp);
    }

    //Xóa folder phòng
    if (_rmdir(path) == 0) {
        setColor(10);
        printf("Đã xóa phòng P%d%02d.\n", floor, room);
        setColor(7);
    } else {
        setColor(12);
        printf("[Lỗi]. Không thể xóa phòng.\n");
        setColor(7);
    }
}

//Hàm xóa toàn bộ phòng trong tầng
void deleteAllRooms(char floorPath[]) {

    struct dirent *dir;
    DIR *dp = opendir(floorPath);
    if (dp == NULL) {
        return;
    }

    //Đọc từng folder phòng
    while ((dir = readdir(dp)) != NULL) {
        //Bỏ qua "." và ".."
        if (strcmp(dir->d_name, ".") != 0 &&
            strcmp(dir->d_name, "..") != 0) {

            //Tạo đường dẫn phòng
            char roomPath[256];
            sprintf(roomPath, "%s/%s", floorPath, dir->d_name);

            //Xóa folder phòng
            _rmdir(roomPath);
        }
    }

    closedir(dp);
}

//Hàm xóa tầng
void deleteFloor(int floorCount) {

    //Nhập tầng muốn xóa
    char check[10], confirm;
    openFloorList(floorCount);
    printf("\n");
    while (1) {
        printf("-> Bạn có muốn xóa tầng %d không [y/n]", floorCount);
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, " %c", &confirm) == 1 && 
            (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N')) {
            break;
        }
    }

    if (confirm == 'n' || confirm == 'N') {
        setColor(14);
        printf("Đã hủy xóa tầng %d.", floorCount);
        setColor(7);
    }

    //Tạo đường dẫn tầng
    char path[256];
    sprintf(path, "./FloorList/Tang_%d", floorCount);

    //Kiểm tra folder tầng có tồn tại không
    DIR *dp = opendir(path);
    if (dp == NULL) {
        setColor(12);
        printf("[Lỗi]. Tầng không tồn tại\n");
        setColor(7);
        return;
    }
    closedir(dp);

    //Kiểm tra tầng còn phòng không
    DIR *checkRoom = opendir(path);
    struct dirent *dir;
    int hasRoom = 0;

    while ((dir = readdir(checkRoom)) != NULL) {
        //Bỏ qua . và ..
        if (strcmp(dir->d_name, ".") != 0 &&
            strcmp(dir->d_name, "..") != 0) {
            hasRoom = 1;
            break;
        }
    }

    closedir(checkRoom);

    //Nếu còn phòng thì không cho xóa
    if (hasRoom) {
        char confirm;
        char check[10];
        while (1) {
            printf("-> Tầng vẫn còn phòng. Bạn có chắc muốn xóa không (y/n): "); 
            fgets(check, sizeof(check), stdin);
            if (sscanf(check, " %c", &confirm) == 1 && 
                (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N')) {
                break;
            }
        }

        //Nếu người dùng chọn yes thì xóa tầng
        if (confirm == 'y' || confirm == 'Y') {

            //Xóa toàn bộ phòng
            deleteAllRooms(path);

            //Xóa tầng
            if (_rmdir(path) == 0) {
                setColor(10);
                printf("Đã xóa tầng %d.\n", floorCount);
                setColor(7);
                return;
            } else {
                setColor(12);
                printf("[Lỗi]. Không thể xóa tầng %d\n", floorCount);
                setColor(7);
                return;
            }

        } else {
            setColor(10);
            printf("Đã hủy xóa tầng.\n");
            setColor(7);
            return;
        }
    }

    //Xóa tầng
    if (_rmdir(path) == 0) {
        setColor(10);
        printf("Đã xóa tầng %d thành công!\n", floorCount);
        setColor(7);
        return;
    } else {
        setColor(12);
        printf("[Lỗi]. Không thể xóa tầng!\n");
        setColor(7);
        return;
    }
}

//Kiểm tra lựa chọn muốn xóa tầng hay phòng
void checkOption() {

    downLine();

    //In bảng
    drawBorderTop();
    drawRow("Tính Năng Xóa Tầng/Phòng");
    drawBorderMiddle();
    drawRow("1. Xóa tầng.");
    drawRow("2. Xóa phòng");
    drawRow("0. Thoát");
    drawBorderBottom();

    //Lấy lựa chọn của người dùng
    int select;
    char check[10];
    while (1) {
        printf("->Lựa chọn của bạn: ");
        fgets(check, sizeof(check), stdin);
        if (sscanf(check, "%d", &select)) break;
    }

    //Đếm số tầng
    int floorCount = 0;
    countFloors(&floorCount);

    //Chạy tính năng
    switch (select) {
        case 1:
            deleteFloor(floorCount);
            return;
        case 2:
            deleteRoom(floorCount);
            return;
        case 0:
            printf("Thoát tính năng xóa tầng/phòng\n");
            return;
    }
    setColor(12);
    printf("[Lỗi]. ");
    setColor(7);
    printf("Lựa chọn không hợp lệ.");
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

// Hàm hiển thị thông tin phòng sau khi xóa 
void displayStatus(char *folderPath) //*Đường dẫn thư mục
{
    struct dirent *fileEntry; // Con trỏ đại diện cho từng file tìm thấy
    DIR *directory = opendir(folderPath); //Thư mục = mở (đường dẫn thư mục) -> Mở thư mục phòng
    int count = 0; // Biến đếm số người trong phòng

    setColor(14);
    printf("\n\n [!]. Tình Trạn Phòng Hiện Tại.");
    setColor(7);

    printf("\n╔══════════════════════════════════════════════════════════╦══════════════╗\n");
    if (directory == NULL) 
    { 

        printf("║ Phòng trống (Không thấy thư mục).                                       ║\n"); 
        printf("╚══════════════════════════════════════════════════════════╩══════════════╝\n");
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
            draw(customerName, cccdNumber, count);
        }
    }
    closedir(directory); // Đóng thư mục
    if (count == 0) printf("║ Phòng trống                                              ║              ║\n"); // Nếu không tìm thấy file nào
    printf("╚══════════════════════════════════════════════════════════╩══════════════╝\n");
}

// Hàm xóa tất cả thông tin 
void deleteAll(char *folderPath, char *floor, char *displayRoom) //*Đường dẫn thư mục, *tầng, *số phòng
{
    char yes;
    setColor(12);
    printf("\n[CẢNH BÁO]. ");
    setColor(7);
    char check[10];
    while (1) { // Nhận ký tự xác nhận y/n
        printf("Bạn có chắc muốn dọn sạch phòng %s? (y/n): ", displayRoom);
        fgets (check, sizeof(check), stdin);
        if (sscanf(check, "%c", &yes)) break;
    } 

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
        setColor(10);
        printf("[Thành công] Đã dọn dẹp sạch toàn bộ phòng %s.\n", displayRoom);
        setColor(7);
    }
    displayStatus(folderPath); // Xem lại tình trạng phòng sau khi dọn
    while(getchar() != '\n');
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
        setColor(14);
        printf("[Thông báo]. ");
        setColor(7);
        printf("Phòng hiện tại đang trống, không có cư dân để xóa!\n");
        return; // Thoát hàm
    }

    // Kiểm tra số lượng người muốn xóa (phải >= 1 và < số người hiện có)
    do {
        printf("-> Hiện có %d khách. Bạn muốn xóa bao nhiêu khách hàng (số lượng ít hơn số hiện có): ", countInRoom, countInRoom - 1);
        if (scanf("%d", &num) != 1) // Kiểm tra nếu người dùng nhập chữ thay vì số
        { 
            setColor(12);
            printf("[Lỗi]. ");
            setColor(7);
            printf("Vui lòng nhập số nguyên.\n");
            while (getchar() != '\n');
            num = -1; // Gán giá trị sai để tiếp tục vòng lặp
        }
        // Kiểm tra điều kiện: số xóa phải >= 1 VÀ phải nhỏ hơn tổng số người hiện có
        if (num != -1 && (num < 1 || num >= countInRoom)) 
        {
            setColor(12);
            printf("[!}. Số lượng không hợp lệ! Để xóa %d người trở lên, vui lòng dùng chức năng 'Xóa toàn bộ phòng'.\n", countInRoom);
            printf("[!]. Vui lòng nhập lại số lượng ít hơn %d.\n", countInRoom);
            setColor(7);
        }
    } while (num < 1 || num >= countInRoom); // Bắt nhập lại nếu không thỏa mãn điều kiện

    for (int i = 1; i <= num; i++) // Vòng lặp chạy từ 1 đến số lượng người cần xóa
    {
        printf("-> [Người thứ %d] Nhập số CCCD: ", i);
        scanf("%s", CCCD); // Nhập số CCCD (cũng là tên file)

        sprintf(filePath, "%s/%s.txt", folderPath, CCCD); // Ghép thành đường dẫn file hoàn chỉnh
        getCustomerName(filePath, customerName); // Tìm xem tên người này là gì trước khi xóa

        if (remove(filePath) == 0) // Lệnh xóa file vật lý trên ổ cứng
        {
            setColor(10);
            printf("[Thành công] Đã xóa hồ sơ: %s\n", customerName);
            setColor(7);
            removeFromGlobalData(CCCD); // Xóa dòng chứa CCCD này trong file tổng cccd.txt
        } 
        else {
            setColor(12);
            printf("[Lỗi] Không tìm thấy CCCD %s tại phòng này.\n", CCCD);
            setColor(7);
        }
    }
    displayStatus(folderPath); // In lại danh sách khách còn lại trong phòng
    while(getchar() != '\n');
}

// Hàm điều hướng chính cho việc xóa thông tin
void handleCheckOutProcess()
{
    char floor[10], room[10], folderPath[256], displayRoomTemp[15]; // Khai báo các chuỗi chứa thông tin tầng, phòng, đường dẫn
    int choice, fNum, rNum; // Khai báo các biến số để lưu lựa chọn và số tầng/phòng sau khi chuyển đổi

    // Kiểm tra số tầng (phải >= 1)
    int floorCount = 0;
    countFloors(&floorCount);
    openFloorList(floorCount);
    printf("\n");
    do {
        char check[10];
        while (1) {
            printf("-> Nhận số tầng: ");
            fgets (check, sizeof(check), stdin);
            if (sscanf(check, "%s", &floor)) break;
        }
        fNum = atoi(floor); // Chuyển chữ thành số để kiểm tra
        if (fNum < 1) {
            setColor(12);
            printf("[Lỗi]. ");
            setColor(7);
            printf("Số tầng không hợp lệ (phải >= 1). Vui lòng nhập lại!\n");
        }
    } while (fNum < 1); // Nếu tầng nhỏ hơn 1 thì bắt nhập lại

    // Kiểm tra số phòng (phải >= 1)
    int floorNum = atoi(floor);
    displayRoom(floorNum);
    do {
        printf("\n-> Nhập số phòng: "); 
        scanf("%s", room); // Nhận dữ liệu phòng dưới dạng chữ
        rNum = atoi(room); // Chuyển chữ thành số để kiểm tra
        if (rNum < 1 || rNum > 99) {
            setColor(12);
            printf("[Lỗi]. ");
            setColor(7);
            printf("Số phòng không hợp lệ (phải nằm trong [1, 99]). Vui lòng nhập lại!\n");
        }
    } while (rNum < 1 && rNum > 99); // Nếu phòng nhỏ hơn 1 và lớn hơn 99 thì bắt nhập lại

    // Tự động định dạng tên phòng (Ví dụ: tầng 1 phòng 5 -> P105)
    if (rNum < 10) 
    {
        // Nếu số phòng < 10, thêm số 0 ở giữa (Ví dụ: P105)
        sprintf(folderPath, "FloorList/Tang_%s/P%s0%s", floor, floor, room);
        sprintf(displayRoomTemp, "P%s0%s", floor, room);
    } 
    else 
    {
        // Nếu số phòng >= 10, ghép bình thường (Ví dụ: P110)
        sprintf(folderPath, "FloorList/Tang_%s/P%s%s", floor, floor, room);
        sprintf(displayRoomTemp, "P%s%s", floor, room);
    }

    //Tạo tiêu đề
    char caption[64];
    sprintf(caption, "Tính Năng Xóa Cư Dân (Phòng %s)", displayRoomTemp);

    //Tạo Bảng
    downLine();
    drawBorderTop();
    drawRow(caption);
    drawBorderMiddle();
    drawRow("1. Xóa một hoặc nhiều người (theo CCCD).");
    drawRow("2. Xóa toàn bộ phòng (Trả phòng sạch).");
    drawRow("0. Quay lại menu.");
    drawBorderBottom();

    //Nhập lựa chọn của người dùng
    while (getchar() != '\n');
    char check[10];
    while (1) {
        printf("-> Lựa chọn của bạn: ");
        fgets (check, sizeof(check), stdin);
        if (sscanf(check, "%d", &choice)) break;
    }

    //In danh sách cư dân
    downLine();
    printResidentCCCDAndName(folderPath, fNum, rNum);

    if (choice == 1) deleteManyPeople(folderPath, floor, displayRoomTemp); // Gọi hàm xóa từng người
    if (choice == 2) deleteAll(folderPath, floor, displayRoomTemp); // Gọi hàm dọn sạch phòng
    
}