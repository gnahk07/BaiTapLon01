#ifndef ROOM_H
#define ROOM_H

////Hàm kiểm tra lựa chọn tầng để thêm phòng có phù hợp không
void checkSelectFloor(int selectFloor, int floorCount, int *check);

//In các phòng đã có sẵn
void displayRoom(int selectFloor);

//Nhập số thứ tự phòng và kiểm tra
void inputRoomOrder(int *roomOrder);

//Kiểm tra số thứ tự có phù hợp không
int checkRoomOrder(int roomOrder);

//Hàm thêm phòng
void addRoom(int floorCount);

#endif