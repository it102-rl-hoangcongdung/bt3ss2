# BÁO CÁO PHÂN TÍCH TRADE-OFF VÀ TÁI CẤU TRÚC MÃ NGUỒN TÍNH CƯỚC GRABRIDE

> 👤 **Học viên:** Hoàng Công Dũng | **Mã SV:** PTIT-HCM-009
> 🏫 **Môn học:** it102-rl-hoangcongdung

---

## 📊 Sơ đồ thiết kế hệ thống (Activity Diagram)

> 💡 *Sơ đồ dưới đây được render tự động trực tiếp trên GitHub bằng Mermaid. Bạn cũng có thể tải file **`bt3.drawio`** trong repository này để mở và chỉnh sửa trực tiếp trên [Draw.io (diagrams.net)](https://app.diagrams.net).* 

```mermaid
graph TD
  Start([Bắt đầu]) --> Input[Nhập distance_km và is_surge]
  Input --> GuardDist{distance_km <= 0.0?}
  GuardDist -- Đúng (<= 0) --> ErrDist[Báo lỗi: Quãng đường không hợp lệ] --> EndFail([Kết thúc: Dừng chương trình])
  GuardDist -- Sai (> 0) --> GuardSurge{is_surge != 0 và is_surge != 1?}
  GuardSurge -- Đúng (Khác 0 và 1) --> ErrSurge[Báo lỗi: Trạng thái phụ phí không hợp lệ] --> EndFail
  GuardSurge -- Sai (Hợp lệ) --> StepCalc[Tính cước cơ bản theo bước]
  StepCalc --> CheckDistRange{distance_km <= 2.0?}
  CheckDistRange -- Đúng (<= 2km) --> BaseFare[fare = 12000]
  CheckDistRange -- Sai (> 2km) --> ExtraFare[fare = 12000 + (distance_km - 2.0) * 4500]
  BaseFare --> CheckSurge{is_surge == 1?}
  ExtraFare --> CheckSurge
  CheckSurge -- Có (1) --> ApplySurge[fare = fare * 1.2]
  CheckSurge -- Không (0) --> Finalize
  ApplySurge --> Finalize[Làm tròn và chuẩn bị xuất kết quả]
  Finalize --> PrintResult[In ra màn hình tổng cước chuyến đi]
  PrintResult --> EndSuccess([Kết thúc: Hoàn thành])
```

---

## 1. Phân tích Input, Output và Thách thức kỹ thuật

Trong bài toán TripFareEngine của hệ thống GrabRide, việc xử lý rẽ nhánh điều kiện đóng vai trò quyết định độ ổn định và khả năng bảo trì của mã nguồn.

Dữ liệu đầu vào (Input) bao gồm 'distance_km' (kiểu double đại diện cho quãng đường di chuyển) và 'is_surge' (kiểu int biểu thị trạng thái giờ cao điểm hoặc thời tiết xấu với giá trị 0 hoặc 1).

Dữ liệu đầu ra (Output) là tổng số tiền cước 'total_fare' (kiểu double) được làm tròn chính xác đến 2 chữ số thập phân.

- Điểm biên quãng đường: Đúng 2.0 km phải chịu mức giá cố định chính xác 12.000 VNĐ mà không bị cộng thêm chi phí vượt trội.
- Bẫy dữ liệu đầu vào: Quãng đường nhập vào âm hoặc bằng 0 phải bị chặn ngay lập tức. Trạng thái phụ phí chỉ chấp nhận đúng 0 hoặc 1, mọi giá trị khác đều là lỗi.

## 2. So sánh Giải pháp Legacy (Nested IF) và Giải pháp Refactored (Guard Clauses)

Đoạn mã legacy thường gom tất cả logic kiểm tra quãng đường và phụ phí vào chung một khối if/else lồng nhau sâu tới 3, 4 tầng. Điều này dẫn đến việc nhân hệ số '1.2' bị lặp lại ở nhiều nhánh khác nhau, khiến lập trình viên rất dễ bỏ sót khi nghiệp vụ thay đổi.

Giải pháp tái cấu trúc áp dụng tư duy Guard Clauses kết hợp tính toán theo bước (Step-by-Step Calculation) giúp tách biệt hoàn toàn việc kiểm tra dữ liệu lỗi, tính cước cơ bản và cộng phụ phí.

| Tiêu chí đánh giá | Giải pháp 1 (Legacy / Nested IF) | Giải pháp 2 (Refactored / Guard Clauses) |
| --- | --- | --- |
| Độ phức tạp đọc hiểu | Cao (Cognitive Load lớn do lồng nhau nhiều tầng) | Thấp (Mã phẳng, dễ đọc từ trên xuống dưới) |
| Khả năng bảo trì | Thấp (Phải sửa công thức nhân phụ phí ở nhiều nhánh) | Cao (Chỉ cần thay đổi hằng số hoặc một dòng duy nhất) |
| Trùng lặp mã nguồn | Nhiều (Code Duplication xuất hiện ở các nhánh nhân hệ số) | Không có (Tách biệt rõ ràng từng bước xử lý) |
| Khả năng mở rộng | Kém (Dễ phát sinh lỗi logic khi thêm quy tắc mới) | Tốt (Dễ dàng bổ sung loại phụ phí hoặc biểu giá mới) |

## 3. Quyết định lựa chọn và Thiết kế mã giả (Pseudocode)

Dựa trên bảng phân tích trade-off, chúng ta dứt khoát lựa chọn Giải pháp 2 vì tính an toàn, tường minh và chuẩn mực kỹ thuật phần mềm.

Dưới đây là mã giả thể hiện giải pháp được chọn:

- Bước 1: Kiểm tra 'distance_km <= 0.0' -> Thông báo lỗi và thoát hàm.
- Bước 2: Kiểm tra 'is_surge != 0' và 'is_surge != 1' -> Thông báo lỗi và thoát hàm.
- Bước 3: Tính cước cơ bản dựa trên 'distance_km'. Nếu '<= 2.0', gán '12000'. Ngược lại, tính thêm cước vượt quãng đường.
- Bước 4: Kiểm tra 'is_surge == 1' thì nhân tổng cước với hệ số '1.2'.
- Bước 5: In kết quả làm tròn 2 chữ số thập phân.

## 4. Hướng dẫn kiểm thử và Triển khai thực tế

Chương trình C hoàn chỉnh đã được viết tuân thủ chặt chẽ các quy chuẩn lập trình, đặt tại thư mục 'GRAB_RIDE_SESSION02/exercise10/main.c'.

Để biên dịch và chạy thử trên môi trường Linux/GCC hoặc VS Code, sinh viên sử dụng lệnh 'gcc main.c -o trip_fare' và chạy './trip_fare' để kiểm tra các trường hợp biên như quãng đường 2.0 km, giá trị âm hoặc trạng thái phụ phí không hợp lệ.

---

## 📁 Danh sách tệp tin nộp bài trong Repository
- 📝 `bt3.docx`: Báo cáo tài liệu phân tích nghiệp vụ hoàn chỉnh.
- 🎨 `bt3.drawio`: File thiết kế sơ đồ chuẩn theo quy định đề bài (mở trực tiếp bằng [Draw.io](https://app.diagrams.net) hoặc Lucidchart).
- 💻 `GRAB_RIDE_SESSION02/exercise10/main.c`: Mã nguồn chương trình.
