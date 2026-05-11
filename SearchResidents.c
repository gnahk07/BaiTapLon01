//Thư viện sử dụng
#include <stdio.h>
#include <windows.h>
//Hiển thị Header
void displayHeader() {
    printf("\n=====================================  PHẦN MỀM TÌM KIẾM CƯ DÂN  =====================================\n\n");
}
//Tìm kiếm bằng tên hoặc CCCD
void searchMenu(Resident *head){ //Menu tìm kiếm
	if (head==NULL){
		printf("Hiện tại chưa có dữ liệu phòng nào trong hệ thống!");
		return 0;
	}
	int choice;
	char searchKey[50];
	int found=0;

	printf("1.Tìm kiếm bằng tên.\n");
	printf("2.Tìm kiếm bằng số CCCD.\n");
	printf("Hãy nhập lựa chọn (1 hoặc 2): ");
	if (scanf("%d", &choice) != 1) 
		return;
	getchar(); // Xóa bộ nhớ đệm phím Enter

	printf("Nhập thông tin cần tìm: ");
	fgets(searchKey, sizeof(searchKey), stdin);
    searchKey[strcspn(searchKey, "\n")] = 0; // Xóa ký tự xuống dòng

	printf("\n%-20s | %-15s | %-10s\n", "Họ tên", "Số CCCD", "Phòng");
    printf("------------------------------------------------------\n");
	
	Resident *current = head;
	while (current != NULL) {
        int isMatched = 0;
		int foundCount=0;
        if (choice == 1) {
            // Tìm kiếm tương đối (strstr) cho tên
            if (strstr(current->name, searchKey) != NULL) 
				isMatched = 1;
        } else if (choice == 2) {
            // Tìm kiếm tuyệt đối cho CCCD
            if (strcmp(current->CCCD, searchKey) == 0) 
				isMatched = 1;
        }
        if (isMatched) {
            printf("%-25s | %-15s | %-20s\n", current->name, current->CCCD, "Dữ liệu tệp");
            foundCount++;
        }
        current = current->next; // Duyệt node tiếp theo
    }
    // Xử lý ngoại lệ khi không tìm thấy
    if (foundCount == 0) {
        printf("Không tìm thấy cư dân phù hợp với thông tin: '%s'.\n", searchKey);
    } else {
        printf("----------------------------------------------------------------------\n");
        printf("Tìm được tổng cộng: %d cư dân.\n", foundCount);
    }
}
//Hàm main
int main () {
	
    //chỉnh sửa lỗi front chữ 
	SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    //Hiển thị Header
    displayHeader ();

	//Hiển thị menu
	searchMenu(*head);
    return 0;
}
