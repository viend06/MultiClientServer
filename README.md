# MultiClientServer# MultiClientServer

Đây là một Project cá nhân. Project implement một chat server đơn giản hỗ trợ nhiều client kết nối cùng lúc, có đăng nhập và nhắn tin.

---

## Mô tả

Server lắng nghe kết nối TCP từ nhiều client. Mỗi client khi kết nối vào sẽ phải đăng nhập bằng tài khoản có sẵn trong file `user.txt`. Sau khi đăng nhập thành công thì có thể nhắn tin với nhau.

Các chức năng chính:
- Đăng nhập bằng username/password
- Nhắn tin broadcast tới tất cả mọi người
- Nhắn tin riêng (private message) tới 1 người cụ thể
- Thoát bằng lệnh `/quit`
- Có thể đăng nhập lại mà không cần restart client

---

## Cấu trúc project

```
.
├── Server.cpp
├── Clients.cpp
├── user.txt                      # lưu tài khoản người dùng
├── commands/
│   ├── Command.h                 # định nghĩa struct Command và enum CommandType
│   ├── CommandParser.h/.cpp      # parse chuỗi nhập vào thành Command
│   ├── CommandDispatcher.h/.cpp  # điều hướng Command tới hàm xử lý tương ứng
├── login/
│   ├── Login.h/.cpp
├── quit/
│   ├── Quit.h/.cpp
├── transport/
│   ├── send_recv.h/.cpp          # wrapper gửi/nhận dữ liệu qua socket
├── FileIO/
│   ├── FileHandler.h/.cpp        # đọc ghi file user.txt
└── makefile
```

---

## Yêu cầu

- Linux (Ubuntu khuyến nghị)
- g++ hỗ trợ C++17
- thư viện pthread

---

## Cách build và chạy

```bash
make
```

Sau khi build xong sẽ có 2 file thực thi là `server` và `client`.

Chạy server trước:
```bash
./server
```

Sau đó chạy client (có thể mở nhiều terminal):
```bash
./client
```

Server mặc định lắng nghe ở port **2026**, client kết nối vào `127.0.0.1:2026`.

---

## Tài khoản mẫu

Tài khoản được lưu trong file `user.txt`, mỗi dòng gồm `<tên> <mật khẩu>`:

```
nguyen xuan anh 123456
vien khuong duy 123456789
```

> Hiện tại chưa có chức năng đăng ký tài khoản mới, phải thêm tay vào `user.txt`.

---

## Các lệnh hỗ trợ

| Lệnh | Mô tả |
|---|---|
| `/msg <tên> <nội dung>` | Nhắn tin riêng cho người đang online |
| `/msg <nội dung>` | Broadcast cho tất cả mọi người |
| `/quit` | Thoát khỏi chat |

---

## Luồng hoạt động

```
Client kết nối vào server
   └─> Server gửi "USERNAME:"
   └─> Client nhập username, gửi lên
   └─> Server gửi "PASSWORD:"
   └─> Client nhập password, gửi lên
       ├─> Đúng → gửi "LOGIN_OK", vào chat
       └─> Sai  → gửi "LOGIN_FAIL", nhập lại

Trong chat:
   /msg alice hello   → gửi riêng cho alice
   /msg hello         → broadcast
   /quit              → server gửi QUIT_OK, client thoát
```

Server dùng mỗi thread cho mỗi client. Các dữ liệu dùng chung (`clients`, `list_user`) được bảo vệ bằng mutex.

---

## Một số vấn đề còn tồn tại

- File `user.txt` chỉ append, chạy nhiều lần sẽ bị trùng dòng (đã comment phần xóa file trong `FileHandler.cpp` để test)
- `send_recv.cpp` chưa xử lý partial send/recv
- Chưa có chức năng đăng ký tài khoản
- Chưa mã hóa, dữ liệu truyền plain text

---
