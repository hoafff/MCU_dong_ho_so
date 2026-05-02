# MCU Digital Clock Project - SN32F407_EVK

## 1. Giới thiệu dự án

Dự án này xây dựng một thiết bị đồng hồ số sử dụng board **SN32F407_EVK**.

Đồng hồ hiển thị giờ và phút trên 4 LED 7 đoạn theo định dạng:

`HH.MM`

Sau khi cấp nguồn hoặc reset, đồng hồ trở về chế độ thường và hiển thị:

`00.00`

Đồng hồ tự động đếm thời gian. Sau mỗi 60 giây, phút tăng thêm 1. Khi phút đạt 60, phút quay về 0 và giờ tăng thêm 1. Giờ được hiển thị từ `00` đến `23`.

Ngoài chức năng đồng hồ cơ bản, dự án còn hỗ trợ chỉnh giờ, chỉnh phút, cài đặt giờ hẹn, lưu giờ hẹn, báo buzzer khi đến giờ hẹn, LED báo chế độ hẹn giờ và timeout khi không thao tác.

---

## 2. Chức năng chính

### 2.1. Hiển thị đồng hồ

- Hiển thị giờ và phút bằng 4 LED 7 đoạn.
- Định dạng hiển thị: `HH.MM`.
- Sau reset, thời gian hiện tại về `00.00`.
- Giờ chạy từ `00` đến `23`.
- Phút chạy từ `00` đến `59`.

---

### 2.2. Chỉnh giờ và phút hiện tại

Sử dụng nút **SW3** để vào chế độ chỉnh thời gian.

Luồng hoạt động:

`Chế độ thường -> Bấm SW3 -> Chỉnh giờ -> Bấm SW3 -> Chỉnh phút -> Bấm SW3 -> Chế độ thường`

Khi đang chỉnh giờ:

- Hai LED hiển thị giờ `HH` sẽ nhấp nháy.
- Bấm `SW6` để tăng giờ.
- Bấm `SW10` để giảm giờ.

Khi đang chỉnh phút:

- Hai LED hiển thị phút `MM` sẽ nhấp nháy.
- Bấm `SW6` để tăng phút.
- Bấm `SW10` để giảm phút.

---

### 2.3. Cài đặt giờ hẹn

Sử dụng nút **SW16** để vào chế độ cài đặt giờ hẹn.

Luồng hoạt động:

`Chế độ thường -> Bấm SW16 -> Chỉnh giờ hẹn -> Bấm SW16 -> Chỉnh phút hẹn -> Bấm SW16 -> Lưu giờ hẹn và thoát về chế độ thường`

Khi đang chỉnh giờ hẹn:

- Hai LED hiển thị giờ `HH` sẽ nhấp nháy.
- LED D6 nhấp nháy.
- Bấm `SW6` để tăng giờ hẹn.
- Bấm `SW10` để giảm giờ hẹn.

Khi đang chỉnh phút hẹn:

- Hai LED hiển thị phút `MM` sẽ nhấp nháy.
- LED D6 tiếp tục nhấp nháy.
- Bấm `SW6` để tăng phút hẹn.
- Bấm `SW10` để giảm phút hẹn.
- Bấm `SW16` để lưu giờ hẹn và phút hẹn.

---

### 2.4. Buzzer

Buzzer có các chức năng sau:

- Khi bấm các nút `SW3`, `SW6`, `SW10`, `SW16`, buzzer kêu `pip` trong khoảng 0.3 giây.
- Khi đồng hồ chạy đến giờ hẹn, buzzer kêu `pip-pip` liên tục trong khoảng 5 giây.
- Khi timeout 30 giây xảy ra, buzzer kêu `pip` trong khoảng 0.3 giây.
- Nếu buzzer đang báo thức, bấm bất kỳ nút nào sẽ tắt báo thức.

---

### 2.5. LED D6

LED D6 được dùng để báo chế độ cài đặt giờ hẹn.

- Khi đang chỉnh giờ hẹn hoặc phút hẹn, LED D6 nhấp nháy với chu kỳ khoảng 1 giây.
- Ở các chế độ khác, LED D6 tắt.

---

### 2.6. Lưu giờ hẹn

Dự án có chức năng lưu giờ hẹn và phút hẹn.

Giờ hẹn được lưu khi người dùng bấm `SW16` ở chế độ chỉnh phút hẹn.

Dữ liệu được lưu gồm:

- Giờ hẹn.
- Phút hẹn.
- Checksum để kiểm tra dữ liệu.
- Giây hẹn mặc định bằng `0`.

Lưu ý:

Dự án hiện sử dụng **Flash nội để mô phỏng EEPROM**. Mục tiêu là giúp dữ liệu giờ hẹn không bị mất sau khi reset hoặc cấp nguồn lại.

---

### 2.7. Timeout 30 giây

Khi đang ở một trong các chế độ sau:

- Chỉnh giờ hiện tại.
- Chỉnh phút hiện tại.
- Chỉnh giờ hẹn.
- Chỉnh phút hẹn.

Nếu trong vòng 30 giây không có sự kiện bấm nút, chương trình sẽ tự động thoát về chế độ thường.

Khi timeout xảy ra:

- Thoát về chế độ thường.
- Buzzer kêu `pip` trong khoảng 0.3 giây.
- LED D6 tắt nếu đang ở chế độ chỉnh giờ hẹn hoặc phút hẹn.

Đối với chế độ chỉnh giờ hẹn, nếu timeout xảy ra thì giá trị đang chỉnh dở sẽ không được lưu. Chỉ khi bấm `SW16` ở chế độ chỉnh phút hẹn thì giờ hẹn mới được lưu.

---

## 3. Mapping nút nhấn

Các nút được sử dụng trong dự án:

| Nút | Chức năng |
|---|---|
| SW3 | Vào/chuyển/thoát chế độ chỉnh giờ và phút hiện tại |
| SW6 | Tăng giờ hoặc phút |
| SW10 | Giảm giờ hoặc phút |
| SW16 | Vào/chuyển/lưu chế độ cài đặt giờ hẹn |

---

## 4. Các chế độ hoạt động

Dự án gồm 5 chế độ chính:

| Chế độ | Ý nghĩa |
|---|---|
| MODE_NORMAL | Chế độ thường, đồng hồ chạy bình thường |
| MODE_SET_HOUR | Chỉnh giờ hiện tại |
| MODE_SET_MINUTE | Chỉnh phút hiện tại |
| MODE_ALARM_HOUR | Chỉnh giờ hẹn |
| MODE_ALARM_MINUTE | Chỉnh phút hẹn |

---

## 5. Cấu trúc code chính

Một số file chính trong dự án:

| File | Chức năng |
|---|---|
| main.c | Khởi tạo hệ thống và chạy vòng lặp chính |
| app.c | Xử lý logic chính của đồng hồ |
| app.h | Khai báo các hàm chính của ứng dụng |
| display7seg.c | Điều khiển hiển thị LED 7 đoạn |
| display7seg.h | Khai báo hàm hiển thị LED 7 đoạn |
| keypad.c | Quét và xử lý phím bấm |
| keypad.h | Khai báo mã phím và hàm keypad |
| buzzer_led.c | Điều khiển buzzer và LED D6 |
| buzzer_led.h | Khai báo hàm buzzer và LED |
| eeprom.c | Xử lý lưu trữ giờ hẹn |
| eeprom.h | Khai báo hàm đọc/ghi EEPROM mô phỏng |

---

## 6. Mô tả các file quan trọng

### 6.1. main.c

File `main.c` có nhiệm vụ khởi tạo hệ thống và chạy vòng lặp chính.

Nhiệm vụ chính:

- Khởi tạo clock hệ thống.
- Khởi tạo LED 7 đoạn.
- Khởi tạo keypad.
- Khởi tạo buzzer và LED D6.
- Khởi tạo bộ lưu trữ EEPROM/Flash.
- Gọi các task xử lý chính.

---

### 6.2. app.c

File `app.c` là file xử lý logic chính của chương trình.

Nhiệm vụ chính:

- Quản lý thời gian hiện tại.
- Quản lý giờ hẹn.
- Xử lý các chế độ hoạt động.
- Xử lý nút nhấn.
- Xử lý timeout 30 giây.
- Cập nhật LED 7 đoạn.
- Gọi hàm lưu và đọc giờ hẹn.

Các biến quan trọng:

| Biến | Ý nghĩa |
|---|---|
| s_now | Lưu thời gian hiện tại |
| s_alarm | Lưu giờ hẹn đã được xác nhận |
| s_alarm_edit | Lưu tạm giờ hẹn khi đang chỉnh |
| s_mode | Lưu chế độ hiện tại của chương trình |
| s_idle_ms | Đếm thời gian không thao tác để xử lý timeout |
| s_blink_on | Điều khiển trạng thái nhấp nháy |

---

### 6.3. display7seg.c

File `display7seg.c` điều khiển hiển thị LED 7 đoạn.

Nhiệm vụ chính:

- Hiển thị giờ và phút.
- Quét LED 7 đoạn.
- Xử lý nhấp nháy phần giờ hoặc phút khi đang chỉnh.

---

### 6.4. keypad.c

File `keypad.c` xử lý bàn phím ma trận.

Nhiệm vụ chính:

- Quét phím.
- Phát hiện nút được nhấn.
- Trả về mã phím tương ứng cho chương trình chính.

---

### 6.5. buzzer_led.c

File `buzzer_led.c` xử lý buzzer và LED D6.

Nhiệm vụ chính:

- Buzzer kêu pip khi bấm nút.
- Buzzer báo thức trong 5 giây khi đến giờ hẹn.
- Điều khiển LED D6 khi đang chỉnh giờ hẹn hoặc phút hẹn.

---

### 6.6. eeprom.c

File `eeprom.c` xử lý lưu trữ giờ hẹn.

Nhiệm vụ chính:

- Ghi dữ liệu giờ hẹn.
- Đọc dữ liệu giờ hẹn.
- Kiểm tra dữ liệu bằng checksum.
- Mô phỏng EEPROM bằng Flash nội.

---

## 7. Quy trình demo dự án

### Bước 1: Demo khởi động

Reset hoặc cấp nguồn lại cho board.

Kết quả mong muốn:

`00.00`

Giải thích:

Đồng hồ sau reset trở về chế độ thường, giờ và phút được reset về 0.

---

### Bước 2: Demo chỉnh giờ

Thao tác:

1. Bấm `SW3`.
2. Bấm `SW6`.
3. Bấm `SW10`.

Kết quả:

- Bấm `SW3`: vào chế độ chỉnh giờ, phần `HH` nhấp nháy.
- Bấm `SW6`: giờ tăng lên 1.
- Bấm `SW10`: giờ giảm xuống 1.
- Mỗi lần bấm nút, buzzer kêu `pip`.

---

### Bước 3: Demo chỉnh phút

Thao tác:

1. Bấm `SW3` lần nữa.
2. Bấm `SW6`.
3. Bấm `SW10`.
4. Bấm `SW3` lần nữa để thoát.

Kết quả:

- Bấm `SW3`: chuyển sang chỉnh phút, phần `MM` nhấp nháy.
- Bấm `SW6`: phút tăng lên 1.
- Bấm `SW10`: phút giảm xuống 1.
- Bấm `SW3`: thoát về chế độ thường.

---

### Bước 4: Demo cài giờ hẹn

Ví dụ đặt giờ hẹn là `00.02`.

Thao tác:

1. Bấm `SW16`.
2. Bấm `SW16` lần nữa để chuyển sang chỉnh phút hẹn.
3. Bấm `SW6` đến khi phút hẹn là `02`.
4. Bấm `SW16` để lưu.

Kết quả:

- Bấm `SW16`: vào chỉnh giờ hẹn.
- Phần `HH` nhấp nháy.
- LED D6 nhấp nháy.
- Bấm `SW16` lần nữa: vào chỉnh phút hẹn.
- Phần `MM` nhấp nháy.
- Bấm `SW16` lần cuối: lưu giờ hẹn và thoát về chế độ thường.
- LED D6 tắt.

---

### Bước 5: Demo báo thức

Sau khi đặt giờ hẹn, để đồng hồ chạy đến đúng giờ hẹn.

Kết quả mong muốn:

- Buzzer kêu `pip-pip` trong khoảng 5 giây.
- Sau 5 giây, buzzer tự tắt.
- Có thể bấm bất kỳ nút nào để tắt báo thức sớm.

---

### Bước 6: Demo lưu giờ hẹn sau reset

Sau khi đã lưu giờ hẹn, reset board.

Kết quả:

- Đồng hồ hiện tại trở về `00.00`.
- Bấm `SW16` để vào chế độ xem/chỉnh giờ hẹn.
- Giờ hẹn cũ vẫn còn.

Giải thích:

Giờ hẹn đã được lưu vào bộ nhớ không mất dữ liệu sau reset.

---

### Bước 7: Demo timeout 30 giây

Thao tác:

1. Bấm `SW3` hoặc `SW16` để vào chế độ chỉnh.
2. Không bấm gì trong 30 giây.

Kết quả:

- Sau 30 giây, chương trình tự thoát về chế độ thường.
- Buzzer kêu `pip`.
- LED D6 tắt nếu đang ở chế độ chỉnh giờ hẹn hoặc phút hẹn.

---

### Bước 8: Demo timeout không tự lưu giờ hẹn đang chỉnh dở

Thao tác:

1. Lưu giờ hẹn thật, ví dụ `00.02`.
2. Bấm `SW16` để vào chỉnh giờ hẹn.
3. Đổi giờ hẹn tạm sang giá trị khác, ví dụ `00.05`.
4. Không bấm gì trong 30 giây.
5. Sau khi tự thoát, bấm `SW16` vào xem lại giờ hẹn.

Kết quả mong muốn:

- Giờ hẹn vẫn là `00.02`.
- Giá trị `00.05` không được lưu.

Giải thích:

Khi timeout xảy ra, chương trình chỉ thoát về chế độ thường. Chỉ khi bấm `SW16` ở chế độ chỉnh phút hẹn thì giờ hẹn mới được lưu.

---

## 8. Ghi chú về EEPROM

Trong yêu cầu bài tập có phần EEPROM để lưu giờ hẹn và phút hẹn.

Trong dự án này, nhóm sử dụng Flash nội để mô phỏng EEPROM.

Lý do:

- Board SN32F407 không có EEPROM nội riêng như một vùng EEPROM độc lập.
- Flash nội có thể dùng để lưu dữ liệu cấu hình nhỏ.
- Dữ liệu giờ hẹn vẫn được giữ lại sau reset hoặc mất nguồn.
- Code vẫn tách thành các hàm `EEPROM_ReadByte()` và `EEPROM_WriteByte()` để việc đọc/ghi dữ liệu rõ ràng, dễ thay đổi nếu sau này dùng EEPROM ngoài.

Cách trả lời khi được hỏi:

“Nhóm em dùng Flash nội để mô phỏng EEPROM. Chương trình vẫn có các hàm EEPROM_ReadByte và EEPROM_WriteByte để đọc/ghi dữ liệu giờ hẹn. Về chức năng, giờ hẹn và phút hẹn vẫn được lưu lại sau khi reset board.”

Nếu được hỏi có dùng EEPROM ngoài qua I2C không:

“Dạ không. Nhóm em không dùng EEPROM rời qua I2C. Nhóm em mô phỏng EEPROM bằng Flash nội của vi điều khiển.”

---

## 9. Checklist chức năng đã hoàn thành

- [x] Hiển thị đồng hồ số bằng 4 LED 7 đoạn.
- [x] Định dạng hiển thị `HH.MM`.
- [x] Sau reset hiển thị `00.00`.
- [x] Đồng hồ tự tăng phút sau mỗi 60 giây.
- [x] Phút chạy từ `00` đến `59`.
- [x] Giờ chạy từ `00` đến `23`.
- [x] SW3 dùng để chỉnh giờ và phút hiện tại.
- [x] SW16 dùng để chỉnh giờ hẹn và phút hẹn.
- [x] SW6 dùng để tăng giờ hoặc phút.
- [x] SW10 dùng để giảm giờ hoặc phút.
- [x] Buzzer kêu `pip` khi bấm nút.
- [x] Buzzer kêu `pip-pip` khi đến giờ hẹn.
- [x] LED D6 nhấp nháy khi chỉnh giờ hẹn hoặc phút hẹn.
- [x] Lưu giờ hẹn và phút hẹn.
- [x] Đọc lại giờ hẹn sau reset.
- [x] Timeout 30 giây khi không thao tác.
- [x] Timeout không tự lưu giờ hẹn đang chỉnh dở.

---

## 10. Kết luận

Dự án đã hoàn thành các chức năng chính của bài tập đồng hồ số trên board **SN32F407_EVK**.

Các chức năng đã có gồm:

- Hiển thị đồng hồ số bằng 4 LED 7 đoạn.
- Đồng hồ chạy theo định dạng `HH.MM`.
- Chỉnh giờ và phút hiện tại bằng nút nhấn.
- Cài đặt giờ hẹn.
- Lưu giờ hẹn.
- Buzzer báo khi bấm nút.
- Buzzer báo thức khi đến giờ hẹn.
- LED D6 nhấp nháy khi chỉnh giờ hẹn.
- Timeout 30 giây khi không thao tác.
- Dữ liệu giờ hẹn không mất sau reset.

Dự án phù hợp với yêu cầu xây dựng thiết bị đồng hồ số sử dụng board **SN32F407_EVK**.