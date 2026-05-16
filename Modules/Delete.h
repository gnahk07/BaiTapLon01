#ifndef DELETE_H
#define DELETE_H

//Hàm xóa phòng
void deleteRoom(int floorCount);

//Hàm xóa toàn bộ phòng trong tầng
void deleteAllRooms(char floorPath[]);

//Hàm xóa tầng
void deleteFloor(int floorCount);

//Kiểm tra lựa chọn muốn xóa tầng hay phòng
void checkOption();

// Hàm xóa nhiều thông tin 
void deleteManyPeople(char *folderPath, char *floor, char *displayRoom);

// Hàm xóa tất cả thông tin 
void deleteAll(char *folderPath, char *floor, char *displayRoom);

//Hàm in nội dung bên trong bảng
void draw(char name[], char CCCD[], int count);

// Hàm hiển thị thông tin phòng sau khi xóa 
void displayStatus(char *folderPath);

// Hàm lấy tên khách hàng
void getCustomerName(char *filePath, char *nameOut);

// Hàm xóa dữ liệu cccd khỏi dữ liệu tổng 
void removeFromGlobalData(char *cccdToDelete);

// Hàm điều hướng chính cho việc xóa thông tin
void handleCheckOutProcess();

#endif