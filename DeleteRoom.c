#include <stdio.h>      // Thư viện nhập xuất tiêu chuẩn
#include <windows.h>    // Thư viện Windows (xử lý mã hóa, quản lý file)
#include <dirent.h>     // Thư viện quản lý thư mục
#include <string.h>     // Thư viện xử lý chuỗi
#include <stdlib.h>     // Thư viện tiêu chuẩn (hàm atoi)

//Đọc tên khách hàng từ bên trong file .txt
void getCustomerName(char *filePath, char *nameOut) 
{
    FILE *f = fopen(filePath, "r");
    if (f != NULL) 
    {
        fgets(nameOut, 50, f); // Đọc dòng đầu tiên (Họ tên)
        nameOut[strcspn(nameOut, "\n")] = 0; // Xóa ký tự xuống dòng
        fclose(f);
    } 
    else 
    {
        strcpy(nameOut, "Không rõ tên");
    }
}

//Hiển thị tình trạng khách hàng kèm Tên và CCCD
void displayStatus(char *folderPath) 
{
    struct dirent *fileEntry;
    DIR *directory = opendir(folderPath);
    int count = 0;

    printf("\nSau khi xóa thông tin:");
    
    if (directory == NULL) 
    {
        printf("\nPhòng trống (Không tìm thấy thư mục).\n");
        return;
    }

    while ((fileEntry = readdir(directory)) != NULL) 
    {
        if (strstr(fileEntry->d_name, ".txt")) 
        {
            count++;
            char filePath[512], customerName[50], cccdNumber[20];
            
            // Lấy CCCD từ tên file (bỏ đuôi .txt)
            strcpy(cccdNumber, fileEntry->d_name);
            cccdNumber[strlen(cccdNumber) - 4] = '\0';

            // Lấy Tên bằng cách đọc nội dung file
            sprintf(filePath, "%s/%s", folderPath, fileEntry->d_name);
            getCustomerName(filePath, customerName);
            
            printf("\n%d. Tên: %s | CCCD: %s", count, customerName, cccdNumber);
        }
    }
    closedir(directory);

    if (count == 0) printf("\nPhòng trống");
    printf("\n");
}

//Hàm xóa một tệp khách hàng cụ thể theo cccd
void deleteOnePeople(char *folderPath, char *floor, char *displayRoom)
{
    char CCCD[15], filePath[256], customerName[50];
    
    printf("\n-> Nhập số căn cước công dân của khách hàng: ");
    scanf("%s", CCCD); 

    sprintf(filePath, "%s/%s.txt", folderPath, CCCD);

    //Đọc tên khách hàng TRƯỚC KHI xóa file
    getCustomerName(filePath, customerName);

    if (remove(filePath) == 0) 
    {
        //Thông báo kèm cả Tên và CCCD theo yêu cầu
        printf("[Thành công] Đã xóa dữ liệu khách hàng: %s (Số CCCD: %s)\n", customerName, CCCD);
    } 
    else 
    {
        printf("[Lỗi] Không tìm thấy hoặc đã xóa trước đó tại phòng %s (Tầng %s).\n", displayRoom, floor);
    }

    displayStatus(folderPath);
}

//Hàm xóa toàn bộ tệp khách hàng
void deleteAll(char *folderPath, char *floor, char *displayRoom) 
{
    char yes;
    printf("\n[CẢNH BÁO] Bạn có chắc chắn muốn xóa TOÀN BỘ khách hàng tại phòng %s không?", displayRoom);
    printf("\nNhấn 'y' để xác nhận, phím bất kỳ để hủy: ");
    
    fflush(stdin); 
    scanf(" %c", &yes); 

    if (yes == 'y' || yes == 'Y') 
    {
        struct dirent *fileEntry;
        DIR *directory = opendir(folderPath);

        if (directory == NULL) 
        {
            printf("[Lỗi] Không tìm thấy thông tin tại phòng %s.\n", displayRoom);
            return;
        }

        while ((fileEntry = readdir(directory)) != NULL) 
        {
            if (strstr(fileEntry->d_name, ".txt")) 
            {
                char fullPath[256], customerName[50], cccdNumber[20];
                
                // Lấy thông tin để in thông báo xóa từng người
                strcpy(cccdNumber, fileEntry->d_name);
                cccdNumber[strlen(cccdNumber) - 4] = '\0';
                sprintf(fullPath, "%s/%s", folderPath, fileEntry->d_name);
                getCustomerName(fullPath, customerName);

                if (remove(fullPath) == 0) 
                {
                    printf("-> Đã xóa: %s (CCCD: %s)\n", customerName, cccdNumber);
                }
            }
        }
        closedir(directory);
        printf("[Thành công] Đã dọn dẹp sạch phòng %s.\n", displayRoom);
    } 
    else 
    {
        printf("[Hủy bỏ] Đã dừng thao tác.\n");
    }

    displayStatus(folderPath);
}

// 3. Hàm điều hướng chính
void handleCheckOutProcess() 
{
    char floor[10], room[10], folderPath[256], displayRoom[15];
    int choice;

    printf("\n========== HỆ THỐNG TRẢ PHÒNG CHUNG CƯ ==========\n");
    printf("Nhập số tầng: "); scanf("%s", floor);
    printf("Nhập số phòng: "); scanf("%s", room);

    int roomNum = atoi(room);

    if (roomNum < 10) 
    {
        sprintf(folderPath, "FloorList/Tang_%s/P%s0%s", floor, floor, room);
        sprintf(displayRoom, "P%s0%s", floor, room);
    } 
    else 
    {
        sprintf(folderPath, "FloorList/Tang_%s/P%s%s", floor, floor, room);
        sprintf(displayRoom, "P%s%s", floor, room);
    }

    printf("\nCHỌN CHẾ ĐỘ XÓA:\n");
    printf("1. Xóa một khách hàng cụ thể (theo CCCD)\n");
    printf("2. Xóa tất cả khách hàng (Trả phòng)\n");
    printf("0. Quay lại menu chính\n");
    printf("Lựa chọn của bạn: ");
    scanf("%d", &choice);

    if (choice == 1) deleteOnePeople(folderPath, floor, displayRoom);
    else if (choice == 2) deleteAll(folderPath, floor, displayRoom);
}

int main() 
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    int menuChoice;
    do 
    {
        printf("\n======= MENU =======\n");
        printf("1. Chạy chức năng Trả phòng\n");
        printf("0. Thoát\n");
        printf("Lựa chọn: ");
        scanf("%d", &menuChoice);
        if (menuChoice == 1) handleCheckOutProcess();
    } while (menuChoice != 0);
    return 0;
}