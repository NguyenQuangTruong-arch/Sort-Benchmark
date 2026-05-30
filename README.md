# Báo cáo Đồ án

## Thông tin chung
- Tên môn học: Cấu trúc dữ liệu và giải thuật
- Thời gian học: Học kỳ 2, Năm học 2025-2026
- Họ tên sinh viên: Nguyễn Đình Nhân, Nguyễn Quang Trường, Lê Quang Quốc Tuấn
- MSSV: 25120402, 25120460, 25120461

## 1. Thuật toán cài đặt tốt nhất ở lần chạy đầu tiên (Run 1)
- Thuật toán cài đặt tốt nhất: Radix Sort (cơ số 256) kết hợp Fast I/O cơ bản.
- Các phương thức tối ưu hóa liên quan:
  - Tối ưu I/O: Thay thế cin/cout và scanf/printf bằng bộ đệm mảng char tự quản lý kích thước 64KB (1 << 16) bằng fread và fwrite. Đọc ký tự được xử lý thủ công qua hàm get_char() để ép kiểu số nguyên.
  - Xử lý số âm bằng bitwise: Sử dụng phép XOR bit dấu (src[i] ^= 0x80000000) để dịch chuyển toàn bộ dải giá trị số nguyên có dấu về số không dấu (unsigned int) nhằm sắp xếp đúng thứ tự mà không cần phân chia hai mảng âm/dương riêng biệt.
- Vì:
  - Thuật toán có độ phức tạp thời gian tuyến tính O(N), Tốt hơn so với các thuật toán dựa trên so sánh khi kích thước mảng N rất lớn.
  - Bottleneck lớn nhất của bài toán thao tác mảng lớn là quá trình đọc/ghi chuỗi ký tự từ bộ nhớ. Sử dụng buffer 64KB giúp giảm thiểu tối đa số lần gọi hàm đọc ghi, giúp tăng tốc độ I/O lên nhiều lần so với dùng hàm I/O chuẩn.

## 2. Sinh Test Case (test_gen.cpp) và Tác động đến Thuật toán
- Cách thức sinh test case: 
  - Sinh ngẫu nhiên số lượng phần tử N ở mức tối đa cho phép.
  - Các phần tử được sinh ra tập trung vào các số có độ dài chuỗi ký tự tối đa (chạm ngưỡng INT_MAX: 2147483647 và INT_MIN: -2147483648) và được sắp xếp theo thứ tự đảo ngược hoặc ngẫu nhiên hoàn toàn.
- Thuật toán mục tiêu:
  - Nhắm vào các cài đặt sử dụng Quick Sort hoặc Intro Sort hoặc sử dụng hàm I/O chuẩn (cin, scanf).
- Lý do và Cơ chế làm tăng thời gian chạy:
  - Độ dài ký tự tối đa: Ép các hàm phân tách chuỗi thành số (string parsing) phải thực hiện số bước lặp lớn nhất cho mỗi số (lên tới 11 ký tự/số), làm quá tải thời gian của các bài dùng cin.
  - Tính ngẫu nhiên: Các số phân phối ngẫu nhiên triệt tiêu khả năng dự đoán rẽ nhánh (branch prediction) của CPU. Thuật toán sắp xếp thông thường khi gặp mảng kích thước khổng lồ phân bố ngẫu nhiên sẽ phải thực hiện hoán đổi vị trí (swap) và di chuyển dữ liệu liên tục, gây ra hiện tượng cache miss, làm tăng thời gian chạy.

## 3. Thuật toán cài đặt tốt nhất ở lần chạy thứ hai (Run 2)
- Thuật toán cài đặt tốt nhất: Radix Sort (cơ số 256) tối ưu hóa kiến trúc (SIMD/AVX2) và quản lý bộ nhớ bậc cao.
- Các phương thức tối ưu hóa liên quan (tối ưu tiếp tục so với lần 1):
  - Tối ưu hóa trình biên dịch: Bổ sung chỉ thị pragma GCC optimize("O3,unroll-loops") và pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt"). Cờ unroll-loops giúp mở rộng các vòng lặp đếm phân phối, kết hợp với AVX2 để CPU có thể tính toán song song nhiều chỉ mục mảng cùng lúc bằng thanh ghi vector 256-bit.
  - Tối ưu hóa gọi hàm (Inline Functions): Ép trình biên dịch inline toàn bộ các hàm đọc ghi (get_char, put_char, read_int, print_int) thông qua từ khóa static inline, giảm chi phí gọi hàm khi gọi hàm hàng nhiều lần.
  - Hardcode nhánh điều kiện biên: Thiết lập nhánh điều kiện riêng để in cứng hằng số chuỗi "2147483648\n" nếu x == -2147483648.
- Làm thế nào để tối ưu tiếp tục so với lần 1:
  - Lần 1 dù đã có độ phức tạp O(N) nhưng vẫn lãng phí ở các thao tác sao chép mảng tạm, chi phí gọi hàm I/O và tràn số khi xử lý trị tuyệt đối của INT_MIN. Lần 2 đã khắc phục triệt để bằng việc quản lý vòng đời con trỏ (không copy mảng), sử dụng cờ tối ưu AVX2 để tận dụng tối đa sức mạnh CPU, và can thiệp ở mức vi kiến trúc (branching/inlining). Điều này giúp rút ngắn vài mili-giây (ms) thời gian chạy.
