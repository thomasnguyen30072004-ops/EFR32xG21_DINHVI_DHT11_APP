import 'package:ble_app/screen/home/enter_mac.dart';
import 'package:ble_app/screen/home/home_screen.dart';
import 'package:flutter/material.dart';

// ============================================================================
// 1. CẤU HÌNH HỆ THỐNG (BẮT BUỘC PHẢI SỬA CÁC DÒNG NÀY)
// ============================================================================

// Thay thế bằng địa chỉ MAC thực tế của 3 bo mạch EFR32 bạn đang có.
// Dùng app "nRF Connect" trên điện thoại để quét và tìm địa chỉ này (VD: C0:04:15:XX:XX:XX).



void main() {
  runApp(const MaterialApp(
    debugShowCheckedModeBanner: false,
    home: EnterMac(),
  ));
}



