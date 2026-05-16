#ifndef EDIT_H
#define EDIT_H

//Kiểm tra chuỗi có phải toàn là số không
int isAllDigitsEdit(const char *s);

// ============================================================
//  Kiểm tra CCCD có bị trùng không
//  - Nếu CCCD trùng với chính nó (khi sửa) thì bỏ qua
// ============================================================
int checkDuplicatesEdit(const char *CCCD, const char *skipCCCD);

//  Kiểm tra CCCD 
int checkCCCDEdit(Resident *r, const char *skipCCCD);

//  Cập nhật CCCD trong Data/cccd.txt (đổi CCCD cũ thành mới)
void updateCCCDFile(const char *oldCCCD, const char *newCCCD);

// ============================================================
//   Hiển thị danh sách cư dân trong một phòng
//  Trả về số lượng cư dân tìm thấy
// ============================================================
int displayResidentList(const char *roomPath, int floor, int room);

// ============================================================
//  Đọc thông tin cư dân từ file 
// ============================================================
int loadResident(Resident *r, const char *roomPath);

// ============================================================
//  Ghi thông tin cư dân vào file
// ============================================================
void saveResident(const Resident *r, const char *roomPath);

// ============================================================
//  Chỉnh sửa thông tin cư dân
// ============================================================
void editResident(const char *roomPath, int floor, int room);

//  Xây dựng đường dẫn đến phòng
//  Ví dụ: tầng 1, phòng 1 -> FloorList/Tang_1/P101
void buildRoomPath(int floor, int room, char *path);

//  Luồng chính: nhập tầng -> nhập phòng-> chỉnh sửa cư dân
void handleEditRoom();

#endif