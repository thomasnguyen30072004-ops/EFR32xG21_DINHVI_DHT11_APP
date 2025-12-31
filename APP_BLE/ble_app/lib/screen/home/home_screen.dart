import 'dart:async';
import 'dart:math';
import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:permission_handler/permission_handler.dart';

// Đảm bảo bạn import đúng đường dẫn 2 file này
import '../../BeaconModel/BeaconNode.dart';
import 'gridpainter.dart';

// --- CẤU HÌNH ---
// Mẹo: Đặt điện thoại cách mạch 1 mét, xem log RSSI trung bình là bao nhiêu rồi điền vào đây.
const int TX_POWER_AT_1M = -59;
// N: 2.0 (Thoáng) -> 4.0 (Nhiều vật cản). Phòng kín thường là 2.5 - 3.0
const double ENV_FACTOR_N = 3.0;

class IndoorNavPage extends StatefulWidget {
  final String MAC_ANCHOR_1;
  final String MAC_ANCHOR_2;
  final String MAC_ANCHOR_3;

  const IndoorNavPage({
    super.key,
    required this.MAC_ANCHOR_1,
    required this.MAC_ANCHOR_2,
    required this.MAC_ANCHOR_3
  });

  @override
  _IndoorNavPageState createState() => _IndoorNavPageState();
}

class _IndoorNavPageState extends State<IndoorNavPage> {
  // Tọa độ người dùng mặc định
  double userX = 4.0;
  double userY = 4.0;

  bool isScanning = false;
  late StreamSubscription<List<ScanResult>> _scanSubscription;

  late List<BeaconNode> anchors;

  @override
  void initState() {
    super.initState();

    anchors = [
      BeaconNode(
          id: "Waiting..",
          macAddr: widget.MAC_ANCHOR_1,
          x: 0.5, y: 7.5,
          color: Colors.green
      ),
      BeaconNode(
          id: "Waiting..",
          macAddr: widget.MAC_ANCHOR_2,
          x: 7.5, y: 7.5,
          color: Colors.blue
      ),
      BeaconNode(
          id: "Waiting..",
          macAddr: widget.MAC_ANCHOR_3,
          x: 7.5, y: 0.5,
          color: Colors.orange
      ),
    ];

    _initBleSystem();
  }

  Future<void> _initBleSystem() async {
    print(">>> BẮT ĐẦU KHỞI TẠO BLE...");
    var status = await [
      Permission.bluetoothScan,
      Permission.bluetoothConnect,
      Permission.location,
    ].request();

    if (status[Permission.location]!.isDenied) {
      return;
    }

    try {
      await FlutterBluePlus.startScan(
        timeout: null, // Quét liên tục
        androidScanMode: AndroidScanMode.lowLatency,
      );
      setState(() => isScanning = true);
    } catch (e) {
      print("!!! LỖI KHI SCAN: $e");
    }

    _scanSubscription = FlutterBluePlus.scanResults.listen((results) {
      for (ScanResult r in results) {
        try {
          // Tìm trạm khớp MAC
          var node = anchors.firstWhere((a) => a.macAddr == r.device.remoteId.toString());

          // Xử lý gói tin và tính toán
          _processPacket(node, r);

          // Cập nhật vị trí lên màn hình
          if (mounted) _calculateUserPosition();
        } catch (e) {
          // Bỏ qua thiết bị lạ
        }
      }
    }
    );
  }

  int bcdToDecimal(int byte) {
    return ((byte >> 4) * 10) + (byte & 0x0F);
  }

  // --- HÀM XỬ LÝ GÓI TIN (ĐÃ NÂNG CẤP BỘ LỌC) ---
  void _processPacket(BeaconNode node, ScanResult r) {

    // ============================================================
    // 1. THUẬT TOÁN LỌC TRUNG BÌNH (MOVING AVERAGE FILTER)
    // ============================================================

    // Thêm RSSI mới vào lịch sử
    node.rssiHistory.add(r.rssi);

    // Chỉ giữ lại 20 mẫu mới nhất để tính toán (cửa sổ trượt)
    if (node.rssiHistory.length > 20) {
      node.rssiHistory.removeAt(0);
    }

    // Tính trung bình cộng
    double sum = node.rssiHistory.fold(0, (p, c) => p + c);
    double avgRssi = sum / node.rssiHistory.length;

    // Cập nhật giá trị đã lọc vào node (để hiển thị mượt hơn)
    node.rssi = avgRssi.toInt();
    node.lastUpdate = DateTime.now();

    // ============================================================
    // 2. TÍNH KHOẢNG CÁCH (DÙNG GIÁ TRỊ ĐÃ LỌC avgRssi)
    // ============================================================

    double exponent = (TX_POWER_AT_1M - avgRssi) / (10 * ENV_FACTOR_N);
    double rawDistance = pow(10, exponent).toDouble();

    // CHỐT CHẶN (CLAMPING):
    // Nếu tính ra > 12 mét (lớn hơn đường chéo phòng) thì ép về 12m
    // Giúp chấm đỏ không bị bay ra khỏi bản đồ khi nhiễu
    if (rawDistance > 12.0) rawDistance = 12.0;

    node.distance = rawDistance;

    // ============================================================
    // 3. GIẢI MÃ GÓI TIN (THEO LOGIC BIG ENDIAN BẠN ĐÃ CHỌN)
    // ============================================================
    if (r.advertisementData.manufacturerData.isNotEmpty) {
      r.advertisementData.manufacturerData.forEach((companyId, bytes) {


        if (bytes.length >= 4) {
          // Thứ tự: [TempInt, TempDec, HumInt, ID]

          int tInt = bcdToDecimal(bytes[0]);
          int tDec = bcdToDecimal(bytes[1]);
          int hInt = bcdToDecimal(bytes[2]);
          int deviceId = bytes[3]; // ID nằm cuối

          node.id = "Trạm $deviceId";
          node.temp = tInt + (tDec / 10.0);
          node.hum = hInt.toDouble();
        }
      });
    }
  }

  void _calculateUserPosition() {
    double tuSoX = 0, tuSoY = 0, mauSo = 0;
    int activeNodes = 0;
    DateTime now = DateTime.now();

    // Duyệt từng trạm
    for (var node in anchors) {
      // Chỉ chấp nhận trạm online trong 15s
      if (now.difference(node.lastUpdate).inSeconds < 15) {
        activeNodes++;

        // Thuật toán Trọng số
        double weight = 1.0 / (pow(node.distance, 2) + 0.1);

        tuSoX += node.x * weight;
        tuSoY += node.y * weight;
        mauSo += weight;
      }
    }

    if (activeNodes >= 1 && mauSo > 0) {
      setState(() {
        userX = tuSoX / mauSo;
        userY = tuSoY / mauSo;
      });
    }
  }

  @override
  void dispose() {
    _scanSubscription.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.white,
      appBar: AppBar(
        centerTitle: true,
        title: ElevatedButton(
            style: ElevatedButton.styleFrom(
              backgroundColor: Colors.blue,
              foregroundColor: Colors.white,
            ),
            onPressed: () async{
              await FlutterBluePlus.stopScan();
              setState(() {
                // Reset trạng thái
                for (var n in anchors) {
                  n.rssi = -100;
                  n.distance = 10.0;
                  n.id = "Đang cập nhật...";
                  n.rssiHistory.clear(); // Xóa lịch sử lọc
                }
              });
              await FlutterBluePlus.startScan(
                timeout: null,
                androidScanMode: AndroidScanMode.lowLatency,
              );
            }, child: Text('Update Location')),
        backgroundColor: Colors.yellow.shade700,
      ),
      body: Column(
        children: [
          // DANH SÁCH TRẠM
          Container(
            height: 250,
            color: Colors.grey[100],
            child: ListView.builder(
              itemCount: anchors.length,
              itemBuilder: (ctx , i) {
                final node = anchors[i];
                bool isOnline = DateTime.now().difference(node.lastUpdate).inSeconds < 15;
                return Card(
                  margin: const EdgeInsets.symmetric(horizontal: 10, vertical: 5),
                  elevation: 2,
                  child: ListTile(
                    leading: CircleAvatar(
                      backgroundColor: isOnline ? node.color : Colors.grey,
                      child: const Icon(Icons.bluetooth, color: Colors.white),
                    ),
                    title: Text(node.id, style: const TextStyle(fontWeight: FontWeight.bold)),
                    subtitle: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        const SizedBox(height: 4),
                        Text("Temp: ${node.temp}°C  |  Hum: ${node.hum}%"),
                        Text("RSSI (Avg): ${node.rssi} dBm  |  Dist: ${node.distance.toStringAsFixed(2)}m"),
                        Text("MAC: ${node.macAddr}", style: const TextStyle(fontSize: 10, color: Colors.grey)),
                      ],
                    ),
                    trailing: isOnline
                        ? const Icon(Icons.check_circle, color: Colors.green)
                        : const Icon(Icons.error, color: Colors.red),
                  ),
                );
              },
            ),
          ),

          const Divider(thickness: 2, height: 2),

          // BẢN ĐỒ VỊ TRÍ
          Expanded(
            child: LayoutBuilder(
              builder: (context, constraints) {
                double mapSizeInMeters = 8.0;
                double safeMargin = 30.0;
                double drawWidth = constraints.maxWidth - (safeMargin * 2);
                double scale = drawWidth / mapSizeInMeters;

                return Container(
                  color: Colors.white,
                  padding: EdgeInsets.only(
                      left: safeMargin,
                      right: safeMargin,
                      top: 40,
                      bottom: safeMargin
                  ),
                  child: Stack(
                    clipBehavior: Clip.none,
                    children: [
                      // Lớp 1: Lưới tọa độ
                      CustomPaint(
                        size: Size.infinite,
                        painter: GridPainter(scale: scale),
                      ),

                      // Lớp 2: Các trạm cố định
                      ...anchors.map((node) => Positioned(
                        left: node.x * scale - 20,
                        bottom: node.y * scale - 20,
                        child: Column(
                          children: [
                            Container(
                              decoration: BoxDecoration(
                                  color: node.color.withOpacity(0.2),
                                  border: Border.all(color: node.color),
                                  borderRadius: BorderRadius.circular(8)
                              ),
                              padding: const EdgeInsets.all(4),
                              child: Icon(Icons.router, color: node.color, size: 24),
                            ),
                            Text(
                                node.id,
                                style: const TextStyle(fontWeight: FontWeight.bold, fontSize: 10, color: Colors.black87)
                            ),
                            Text(
                                "(${node.x}, ${node.y})",
                                style: const TextStyle(fontSize: 8, color: Colors.grey)
                            ),
                          ],
                        ),
                      )),

                      // Lớp 3: Người dùng
                      AnimatedPositioned(
                        duration: const Duration(milliseconds: 300),
                        curve: Curves.easeOut,
                        left: userX * scale - 15,
                        bottom: userY * scale - 15,
                        child: Column(
                          children: [
                            Container(
                              padding: const EdgeInsets.all(4),
                              decoration: BoxDecoration(
                                  color: Colors.red.withOpacity(0.2),
                                  shape: BoxShape.circle,
                                  border: Border.all(color: Colors.red, width: 2)
                              ),
                              child: const Icon(Icons.person_pin_circle, color: Colors.red, size: 30),
                            ),
                            Container(
                              padding: const EdgeInsets.symmetric(horizontal: 4, vertical: 2),
                              decoration: BoxDecoration(
                                  color: Colors.white.withOpacity(0.9),
                                  borderRadius: BorderRadius.circular(4),
                                  border: Border.all(color: Colors.grey.shade300)
                              ),
                              child: Text(
                                "You (${userX.toStringAsFixed(1)}, ${userY.toStringAsFixed(1)})",
                                style: const TextStyle(color: Colors.red, fontWeight: FontWeight.bold, fontSize: 10),
                              ),
                            ),
                          ],
                        ),
                      ),
                    ],
                  ),
                );
              },
            ),
          ),
        ],
      ),
    );
  }
}