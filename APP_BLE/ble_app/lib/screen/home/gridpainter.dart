import 'package:flutter/material.dart';

class GridPainter extends CustomPainter {
  final double scale;
  GridPainter({required this.scale});

  @override
  void paint(Canvas canvas, Size size) {
    // 1. Vẽ lưới mờ (Mỗi mét 1 dòng)
    final thinPaint = Paint()
      ..color = Colors.grey.withOpacity(0.3)
      ..strokeWidth = 1;

    for (int i = 0; i <= 8; i++) { // Lưới 8x8
      double pos = i * scale;
      // Dọc
      canvas.drawLine(Offset(pos, 0), Offset(pos, size.height), thinPaint);
      // Ngang (từ dưới lên)
      canvas.drawLine(Offset(0, size.height - pos), Offset(size.width, size.height - pos), thinPaint);
    }

    // 2. Vẽ đường chia phòng ĐẬM (Tại x=4 và y=4)
    final thickPaint = Paint()
      ..color = Colors.black
      ..strokeWidth = 4
      ..strokeCap = StrokeCap.square;

    // Đường dọc giữa (x=4)
    canvas.drawLine(Offset(4 * scale, 0), Offset(4 * scale, size.height), thickPaint);
    // Đường ngang giữa (y=4)
    canvas.drawLine(Offset(0, size.height - (4 * scale)), Offset(size.width, size.height - (4 * scale)), thickPaint);

    // 3. Vẽ tên phòng (Tùy chọn cho đẹp)
    drawRoomLabel(canvas, "Phòng 4", Offset(2 * scale, size.height - 6 * scale)); // Góc trên trái
    drawRoomLabel(canvas, "Phòng 3", Offset(6 * scale, size.height - 6 * scale)); // Góc trên phải
    drawRoomLabel(canvas, "Phòng 1", Offset(2 * scale, size.height - 2 * scale)); // Góc dưới trái
    drawRoomLabel(canvas, "Phòng 2", Offset(6 * scale, size.height - 2 * scale)); // Góc dưới phải
  }

  void drawRoomLabel(Canvas canvas, String text, Offset center) {
    final textSpan = TextSpan(
      text: text,
      style: TextStyle(color: Colors.black.withOpacity(0.5), fontSize: 16, fontWeight: FontWeight.bold),
    );
    final textPainter = TextPainter(
        text: textSpan,
        textDirection: TextDirection.ltr,
        textAlign: TextAlign.center
    );
    textPainter.layout();
    // Vẽ nền xám nhẹ cho chữ
    final bgRect = Rect.fromCenter(center: center, width: 80, height: 30);
    canvas.drawRect(bgRect, Paint()..color = Colors.grey.withOpacity(0.3));
    textPainter.paint(canvas, Offset(center.dx - textPainter.width / 2, center.dy - textPainter.height / 2));
  }

  @override
  bool shouldRepaint(old) => false;
}