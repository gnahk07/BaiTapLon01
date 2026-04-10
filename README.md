# Bài tập lớn môn Kỹ thuật lập trình
*Đề Tài: Quản lý phòng trọ.* 
- Yêu cầu:
  + Thêm, xóa các phòng trọ.
  + Thêm những người cư trú vào phòng tương ứng, (mỗi phòng trọ cho phép tối đa 3-4 người cư trú cùng lúc).
  + Những người cư trú ở nhà trọ phải kê khai: Họ tên, CCCD, tuổi, nghề nghiệp, quê quán...
  + Xác nhận thời gian đóng tiền trọ, thông báo những người sắp đến hạn nộp tiền trọ theo số ngày từ thấp đến cao.
- Yêu cầu kỹ thuật:
  + Hàm và tổ chức chương trình: Chương trình chia thành ít nhất 4 module/hàm độc lập. Có hàm trả về giá trị, hàm dùng tham biến (con trỏ) và hàm đệ quy (nếu phù hợp).
  + Con trỏ và quản lý bộ nhớ động: Dùng con trỏ để thao tác dữ liệu, cấp phát và giải phóng bộ nhớ động đúng cách (malloc/calloc/free).
  + Danh sách liên kết đơn: Dùng danh sách liên kết làm cấu trúc lưu trữ dữ liệu chính trong bộ nhớ. Có đầy đủ các thao tác: thêm, xóa, sửa, tìm kiếm, sắp xếp.
  + Thao tác tệp tin: Đọc/ghi dữ liệu ra tệp nhị phân hoặc văn bản; tự động tải dữ liệu khi khởi động và lưu khi thoát chương trình.
  + Xử lý ngoại lệ và tiền xử lý: Kiểm soát lỗi nhập liệu người dùng; sử dụng macro/define hợp lý; tổ chức chương trình thành nhiều tệp (.h và .c).
- Yêu cầu chung:
  + Có 4 file .c tương ứng với: CreateRoom.c, DeleteRoom.c, ChangeRoom.c, SearchDueDate.c.
  + Code sạch, comment mỗi phần code.
  + Đặt tên hàm, biến bằng tiếng anh.
  + Trong hàm main phải được tối ưu.