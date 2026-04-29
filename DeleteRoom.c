#include <stdio.h>      // Thư viện nhập xuất tiêu chuẩn
#include <windows.h>    // Thư viện Windows (xử lý mã hóa, quản lý file)
#include <dirent.h>     // Thư viện quản lý thư mục
#include <string.h>     // Thư viện xử lý chuỗi
#include <stdlib.h>     // Thư viện tiêu chuẩn (hàm atoi)

// 1. Hàm xóa một tệp khách hàng cụ thể theo cccd
void deleteOnepeople(char *folderPath, char *floor, char *displayRoom) {
    char CCCD[12], filePath[256];
    
    printf("\n-> Nhập số căn cước công dân của khách hàng: ");
    scanf("%s", CCCD); 

    // Ghép đường dẫn: folderPath + / + ID + .txt
    sprintf(filePath, "%s/%s.txt", folderPath, CCCD);

    // remove() trả về 0 nếu xóa thành công
    if (remove(filePath) == 0) {
        printf("[Thành công] Đã xóa dữ liệu khách hàng: %s\n", CCCD);
    } else {
        // Thông báo lỗi chi tiết khi không tìm thấy file
        printf("[Lỗi] Không tìm thấy thông tin khách hàng tại phòng %s (Tầng %s).\n", displayRoom, floor);
    }
}

// 2. Hàm xóa toàn bộ tệp khách hàng trong một phòng (Dọn phòng trống)
void deleteAll(char *folderPath, char *floor, char *displayRoom) {
    char yes;
    printf("\n[CẢNH BÁO] Bạn có chắc chắn muốn xóa TOÀN BỘ khách hàng tại phòng %s không?", displayRoom);
    printf("\nNhấn 'y' để xác nhận, phím bất kỳ để hủy: ");
    
    fflush(stdin); // Xóa bộ nhớ đệm đầu vào
    scanf(" %c", &yes); 

    if (yes == 'y' || yes == 'Y') {
        struct dirent *fileEntry; 
        DIR *directory = opendir(folderPath); 

        // Kiểm tra nếu đường dẫn thư mục không tồn tại
        if (directory == NULL) {
            printf("[Lỗi] Không tìm thấy thông tin khách hàng tại phòng %s (Tầng %s).\n", displayRoom, floor);
            return;
        }

        int deletedCount = 0; 
        
        // Duyệt qua từng tệp tin trong thư mục
        while ((fileEntry = readdir(directory)) != NULL) {
            // Chỉ tìm các tệp có đuôi ".txt"
            if (strstr(fileEntry->d_name, ".txt")) {
                char fullPath[256];
                sprintf(fullPath, "%s/%s", folderPath, fileEntry->d_name);
                
                // Thực hiện lệnh xóa
                if (remove(fullPath) == 0) {
                    deletedCount++;
                }
            }
        }
        closedir(directory); // Đóng thư mục
        
        if (deletedCount > 0) {
            printf("[Thành công] Đã dọn dẹp phòng. Đã xóa %d tệp khách hàng.\n", deletedCount);
        } else {
            printf("[Thông báo] Phòng %s hiện đang trống.\n", displayRoom);
        }
    } else {
        printf("[Hủy bỏ] Đã dừng thao tác xóa toàn bộ.\n");
    }
}

// 3. Hàm điều hướng chính (Sẽ gọi hàm này từ Menu chính của cả nhóm)
void handleCheckOutProcess() {
    char floor[10], room[10], folderPath[256], displayRoom[15];
    int choice;

    printf("\n========== HỆ THỐNG TRẢ PHÒNG CHUNG CƯ ==========\n");
    printf("Nhập số tầng (ví dụ: 1): "); 
    scanf("%s", floor);
    printf("Nhập số phòng (ví dụ: 1 hoặc 12): "); 
    scanf("%s", room);

    // Chuyển chuỗi số phòng sang số nguyên để kiểm tra
    int roomNum = atoi(room);

    // Logic tự động định dạng tên phòng (ví dụ: 1 -> P101, 12 -> P112)
    if (roomNum < 10) {
        // Cấu trúc: FloorList/Tang_1/P101
        sprintf(folderPath, "FloorList/Tang_%s/P%s0%s", floor, floor, room);
        sprintf(displayRoom, "P%s0%s", floor, room);
    } else {
        // Cấu trúc: FloorList/Tang_1/P112
        sprintf(folderPath, "FloorList/Tang_%s/P%s%s", floor, floor, room);
        sprintf(displayRoom, "P%s%s", floor, room);
    }

    printf("\nCHỌN CHẾ ĐỘ XÓA:\n");
    printf("1. Xóa một khách hàng cụ thể (theo ID)\n");
    printf("2. Xóa tất cả khách hàng (Trả phòng)\n");
    printf("0. Quay lại menu chính\n");
    printf("Lựa chọn của bạn: ");
    scanf("%d", &choice);

    if (choice == 1) {
        deleteOnepeople(folderPath, floor, displayRoom);
    } else if (choice == 2) {
        deleteAll(folderPath, floor, displayRoom);
    }
}

int main() {
    // Thiết lập hiển thị tiếng Việt trên Console
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    int menuChoice;
    do {
        printf("\n=======	MENU	=======\n");
        printf("1. Chạy chức năng Trả phòng\n");
        printf("0. Thoát\n");
        printf("Lựa chọn: ");
        scanf("%d", &menuChoice);
        
        if (menuChoice == 1) {
            handleCheckOutProcess();
        }
        
    } while (menuChoice != 0);

    return 0;
}