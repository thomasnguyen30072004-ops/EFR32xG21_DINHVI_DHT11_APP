# Đồ Án IoT - Hệ Thống Đo Nhiệt Độ Và Độ Ẩm Sử Dụng EFR32xG21 Và BLE

### Bạn có thể fork repo và tạo pull request, hoặc là có thể clone repos này để tham khảo, repos này vẫn đang trong quá trình thực hiện và chưa hoàn thành xong.
```
git clone: https://github.com/vynt2401/DoAn_IoT
```

## Mô tả dự án

Dự án này là đồ án môn học Thực Hành Giao Tiếp Máy Tính Và Thu Nhận Dữ Liệu (TH-GTMT-TNDL) năm 2025, thuộc Khoa Điện Tử - Viễn Thông, Bộ Môn Máy Tính – Hệ Thống Nhúng.

Dự án tập trung vào Xây dựng hệ thống đo nhiệt độ và độ ẩm sử dụng vi điều khiển EFR32xG21. Hệ thống đọc dữ liệu từ cảm biến I2C (ví dụ: AHT20, MPU6050, MAX30102), hiển thị trên LCD, gửi dữ liệu qua UART đến PC, và quảng bá qua BLE đến các thiết bị di động. Trên PC, sử dụng ứng dụng C để cấu hình hệ thống và lưu dữ liệu vào cơ sở dữ liệu (tùy chọn như SQLite hoặc MySQL).

Dự án cũng mở rộng sang Hệ thống định vị trong nhà sử dụng BLE beacons với ít nhất 3 thiết bị, tính toán vị trí dựa trên RSSI và hiển thị trên app di động.

Cũng nhu hướng dẫn Xây dựng mạng BLE Mesh với 4-5 node, áp dụng nén dữ liệu, bảo mật, và đánh giá hiệu suất (năng lượng, delay, mất mát gói tin).

Dự án nhằm áp dụng kiến thức về giao tiếp I2C, UART, BLE, và xử lý dữ liệu IoT trong môi trường thực tế. Toàn bộ mã nguồn, tài liệu, và hướng dẫn được lưu trữ tại repository này.



### Tính năng chính --> Hệ Thống Đo Nhiệt Độ Và Độ Ẩm

```
Đọc dữ liệu từ cảm biến I2C (ít nhất 2 thông tin: nhiệt độ, độ ẩm, nhịp tim, SpO2, v.v.).
Hiển thị dữ liệu trên LCD (bao gồm 2-3 thông tin cảm biến và chu kỳ đọc).
Gửi dữ liệu qua UART đến PC để cấu hình (chu kỳ đo, chu kỳ quảng bá BLE) và lưu vào cơ sở dữ liệu.
Quảng bá dữ liệu qua BLE (gói tin chứa tên thiết bị và giá trị cảm biến).
Ứng dụng PC (viết bằng C) để xem dữ liệu, cấu hình, và lưu trữ.
```

### Định Vị Trong Nhà Sử Dụng BLE

```
Sử dụng 3 thiết bị BLE beacons tại vị trí cố định, quảng bá dữ liệu cảm biến và định danh.
App di động quét BLE, tính toán khoảng cách dựa trên RSSI, xác định vị trí người dùng trên bản đồ ma trận (ví dụ: Phòng 1: tọa độ [0-3,0-3]).
Hiển thị vị trí người dùng và dữ liệu cảm biến từ 3 beacons trên giao diện app.
Lưu ý: Vị trí chỉ mang tính tương đối do nhiễu RSSI.
```

### Mạng BLE Mesh

```
Xây dựng mesh với 5 node: 1 Provisioner, 3 node mesh (Client, Relay, Server), 1 advertiser.
Provisioner cấu hình và thêm node vào mesh.
Node advertiser thu thập dữ liệu cảm biến và quảng bá.
Mesh nodes scan, truyền dữ liệu qua Client → Relay → Server.
Áp dụng nén dữ liệu (ví dụ: snappy), bảo mật (mã hóa AES), và xử lý dữ liệu.
Đánh giá: Năng lượng (Energy Profiler), tỷ lệ mất gói tin, delay, low power techniques.
Upload dữ liệu từ PC lên cơ sở dữ liệu đám mây.
```

# Yêu cầu thiết bị

Dựa trên yêu cầu từ đồ án:

BRD4180B Radio Board (EFR32xG21 2.4 GHz 20 dBm) + Wireless Starter Kit Mainboard (BRD4001A) (1-5 bộ tùy phần mở rộng).

Cảm biến I2C (AHT20/MPU6050/MAX30102) + dây kết nối (tự chuẩn bị, 1-3 cái).

Smartphone hỗ trợ BLE + App như Simplicity Connect hoặc app tự viết.


# Phần mềm sử dụng 

***
Simplicity Studio v5
Android Studio
Visual Studio Code
***

# Cấu trúc dự án

```
DoAn_IoT/
├── firmware/          # Mã nguồn firmware cho EFR32xG21 (C code)
│   ├── main.c
│   ├── i2c_sensor.c
│   ├── uart_pc.c
│   ├── ble_advertise.c
│   └── lcd_display.c
├── pc_app/            # Ứng dụng PC (C code)
│   ├── config.c
│   ├── data_logger.c
│   └── database_interface.c  # Kết nối SQLite/MySQL
├── mobile_app/        # App di động (Flutter/Android Studio)
│   ├── lib/
│   └── pubspec.yaml
├── docs/              # Tài liệu
│   ├── DoAn_TH-GTMT-TNDL_2025.pdf  # Yêu cầu đồ án gốc
│   └── report.md      # Báo cáo chi tiết
├── images/            # Hình ảnh minh họa
│   ├── system_diagram.png
│   ├── positioning_map.png
│   └── mesh_diagram.png
└── README.md          # File này
```
# Hướng dẫn cài đặt

1. Cài Đặt Simplicity Studio: Tải từ Silicon Labs, import project firmware.
2. Flash Firmware: Kết nối board EFR32xG21, build và flash code từ thư mục firmware/.
3. Cài Đặt PC App: Compile code C trong pc_app/ bằng GCC, kết nối UART.
4. Cài Đặt Mobile App: Mở thư mục mobile_app/ trong Android Studio/Flutter, build APK và
cài trên smartphone.
5. Cơ Sở Dữ Liệu: Cài SQLite hoặc MySQL, cấu hình trong PC app.

# Yêu cầu môi trường

OS: Windows/Linux.
Công Cụ: GCC, Simplicity Studio v5, Flutter (nếu app di động).


# Hướng đẫn sử dụng

1. Chạy Hệ Thống Đo Lường:
Kết nối cảm biến I2C đến board.
Flash firmware, chạy PC app để cấu hình chu kỳ.
Quét BLE trên smartphone để xem dữ liệu quảng bá.

2. Định Vị:
Đặt 3 beacons tại vị trí cố định.
Chạy app di động, quét BLE để tính vị trí và hiển thị bản đồ.

3. BLE Mesh:
Cấu hình Provisioner để thêm node.
Chạy mesh, sử dụng Energy Profiler để đo hiệu suất.
Upload dữ liệu từ PC lên DB.
