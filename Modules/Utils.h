#ifndef UTILS_H
#define UTILS_H

//Hàm đổi màu chữ
void setColor(int color);

//In danh sách cư dân với CCCD và tên
int printResidentCCCDAndName(const char roomPath[], int floor, int room);

//In danh sách
void printAllResident();

//Nhấn Enter để tiếp tục
void enter();

//Hàm fake loading
void fakeLoading();

//Tạo nơi chứa dữ liệu Tỉnh Thành
void initializeProvinceFiles();

//Kiểm tra folder Data có tồn tại không, nếu không thì tạo
void createDataFolder();

//Đếm số tầng hiện có
void countFloors(int *floorCount);

//Hàm kiểm tra lựa chọn tầng để thêm phòng có phù hợp không
void checkSelectFloor(int selectFloor, int floorCount, int *check);

//Lựa chọn thêm tầng hoặc thêm phòng
void chooseAddOption();

#endif