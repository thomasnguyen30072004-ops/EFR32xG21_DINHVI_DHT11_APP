import 'dart:ui';

class BeaconNode {
   String id;      // Tên hiển thị (Trạm 1, 2...)
  final String macAddr; // Địa chỉ MAC nhận diện
  final double x;       // Tọa độ mét (X)
  final double y;       // Tọa độ mét (Y)
  final Color color;    // Màu sắc hiển thị

  // Dữ liệu thay đổi liên tục (State)
  int rssi = -100;
  double distance = 10.0;
  double temp = 0.0;
  double hum = 0.0;
  DateTime lastUpdate = DateTime.now();

   List<int> rssiHistory = []; // Danh sách lưu 20 giá trị RSSI gần nhất


  BeaconNode({
    required this.id,
    required this.macAddr,
    required this.x,
    required this.y,
    required this.color
  });
}