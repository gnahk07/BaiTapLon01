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
	scanf("%d",&choice);
	getchar();

	printf("Nhập thông tin cần tìm: ");
	fgets(searchKey, sizeof(searchKey), stdin);
    searchKey[strcspn(searchKey, "\n")] = 0; // Xóa ký tự xuống dòng
	
	
		
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
