#ifndef ROOM_H
#define ROOM_H

//In các phòng đã có sẵn
void displayRoom(int selectFloor);

//Nhập số thứ tự phòng và kiểm tra
void inputRoomOrder(int *roomOrder);

//Kiểm tra số thứ tự có phù hợp không
int checkRoomOrder(int roomOrder);

//Hàm tạo phòng
void addRoom(int floorCount);

#endif