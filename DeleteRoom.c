#include <stdio.h>      // Thư viện nhập xuất (printf, scanf, fopen...)
#include <windows.h>    // Thư viện Windows (SetConsoleCP hiển thị tiếng Việt)
#include <dirent.h>     // Thư viện quản lý thư mục (opendir, readdir...)
#include <string.h>     // Thư viện xử lý chuỗi (strcpy, strstr...)
#include <stdlib.h>     // Thư viện tiện ích (atoi chuyển chuỗi sang số)

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
    if (found) 
		printf("-> Đã xóa dữ liệu khỏi hệ thống tổng Data/cccd.txt\n");
}