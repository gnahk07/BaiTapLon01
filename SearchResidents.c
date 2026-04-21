//Thư viện sử dụng
#include <stdio.h>
#include <windows.h>
//Tìm kiếm bằng tên
void searchByName(Resident *head , char name[]){ 
	Resident *temp=head;
	int found=0;
}
//Tìm kiếm bằng CCCD
void searchByCCCD(Resident *head , int CCCD[]){
	
}

//Hiển thị Header
void displayHeader() {
    printf("\n=====================================  PHẦN MỀM TÌM KIẾM CƯ DÂN  =====================================\n\n");
}
void searchMenu(Resident *head){ //Menu tìm kiếm
	int choice;
	do{
		printf("1.Tìm kiếm bằng tên.\n");
		printf("2.Tìm kiếm bằng số CCCD.\n");
		printf("0.Quay trở lại.\n");
		printf("Hãy nhập lựa chọn (1,2 hoặc 0): ");
		scanf("%d",&choice);
		getchar();
	}
}



//Hàm main
int main () {

    //chỉnh sửa lỗi front chữ 
	SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    //Hiển thị Header
    displayHeader ();


    return 0;
}
