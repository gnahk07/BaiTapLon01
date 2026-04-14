# Bài tập lớn môn Kỹ thuật lập trình
*Đề Tài: Quản lý trung cư.* 
- Yêu cầu:
  + Thêm, xóa, chỉnh sửa, tìm kiếm các phòng.
  + Thêm những người cư trú vào phòng tương ứng, (mỗi phòng cho phép tối đa 3-4 người cư trú cùng lúc).
  + Những người cư trú phải kê khai: Họ tên, CCCD, ngày/tháng/năm sinh, quê quán.
  + Có thể tìm kiếm người cư trú theo tên, CCCD.
- Yêu cầu kỹ thuật:
  + Hàm và tổ chức chương trình: Chương trình chia thành ít nhất 4 module/hàm độc lập. Có hàm trả về giá trị, hàm dùng tham biến (con trỏ) và hàm đệ quy (nếu phù hợp).
  + Con trỏ và quản lý bộ nhớ động: Dùng con trỏ để thao tác dữ liệu, cấp phát và giải phóng bộ nhớ động đúng cách (malloc/calloc/free).
  + Danh sách liên kết đơn: Dùng danh sách liên kết làm cấu trúc lưu trữ dữ liệu chính trong bộ nhớ. Có đầy đủ các thao tác: thêm, xóa, sửa, tìm kiếm, sắp xếp.
  + Thao tác tệp tin: Đọc/ghi dữ liệu ra tệp nhị phân hoặc văn bản; tự động tải dữ liệu khi khởi động và lưu khi thoát chương trình.
  + Xử lý ngoại lệ và tiền xử lý: Kiểm soát lỗi nhập liệu người dùng; sử dụng macro/define hợp lý; tổ chức chương trình thành nhiều tệp (.h và .c).
- Yêu cầu chung:
  + Có 4 file .c tương ứng với: CreateRoom.c, DeleteRoom.c, EditRoom.c, SearchResidents.c, 
  + Code sạch, comment mỗi phần code.
  + Đặt tên hàm, biến bằng tiếng anh. Chữ cái đầu tiên viết thường, các chữ sau chữ đầu viết hoa. Ví dụ: "createRoom".
  + Trong hàm main phải được tối ưu.
  + Khi đẩy Code lên Github, hãy xóa tệp .exe. Hãy đặt "Summary" là các phần update của phần code được đẩy lên và "Description" là chi tiết các phần được update trong code.

*Các tính năng của từng File code.*
- CreateRoom.c:
  + Chọn tầng. Ví dụ tầng 1, 2, 3...
  + In các phòng đã có sẵn.
  + Tạo tiếp các phòng.
  + Kê khai các thành viên trong phòng gồm: tên, CCCD, ngày/tháng/năm sinh, quê quán.
  + Ví dụ: Chọn tầng 2, tầng 2 có sẵn P201, P202, P203. Nếu muốn tạo phòng thì phải khác các phòng đã có sẵn
- DeleteRoom.c:
  + Chọn tầng. Ví dụ tầng 1, 2, 3...
  + In các phòng đã có sẵn.
  + Lựa chọn phòng muốn xóa.
- EditRoom.c:
  + Chọn tầng. Ví dụ tầng 1, 2, 3...
  + In các phòng đã có sẵn.
  + Lựa chọn phòng để thay đổi.
  + Các lựa chọn để thay đổi: xóa người cư trú, đổi thông tin người cư trú.
- SearchResidents.c:
  + In ra các mục tìm kiếm bao gồm: tìm kiếm theo tên, tìm kiếm theo CCCD.
