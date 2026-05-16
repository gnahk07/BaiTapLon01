#ifndef SREACH_H
#define SREACH_H

// Hàm chuyển toàn bộ chuỗi thành chữ hoa để tìm kiếm không phân biệt hoa thường
void toUpperCase(char *str);

// Hàm xóa các ký tự xuống dòng và khoảng trắng thừa ở cuối chuỗi
void cleanString(char *str);

// Hàm loại bỏ ký tự BOM (3 byte đặc biệt đầu file UTF-8 thường do Notepad tạo ra)
void removeBOM(char *str);

// Hàm mở file để đọc tên khách hàng ở dòng đầu tiên
void getNameFromFile(char *filePath, char *nameOut);

// Hàm tìm kiếm theo CCCD
void searchByCCCD(char *searchKey);

// Hàm tìm kiếm theo Tên
void searchByName(char *searchKey);

//Hàm tìm kiếm chính
void handleSearchProcess();

#endif