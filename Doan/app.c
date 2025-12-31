/***************************************************************************//**
 * @file app.c
 * @brief TRAM_DHT11: DHT11(PC01) + LCD(MAC, Cycle, Count) + BLE
 ******************************************************************************/
#include "em_common.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "gatt_db.h"
#include "app.h"
#include "custom_adv.h"
#include "app_timer.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_core.h"
#include "sl_udelay.h"
#include <stdio.h>

// --- THƯ VIỆN LCD ---
#include "sl_board_control.h"
#include "glib.h"
#include "dmd.h"

// --- CẤU HÌNH CHÂN DHT11: GIỮ NGUYÊN PC01 ---
// --- CẤU HÌNH CHÂN DHT11 ---
// PC01: Port C, Pin 1. Đây là chân giao tiếp 1-wire với cảm biến.
#define DHT_PORT gpioPortC
#define DHT_PIN  1

// --- THÔNG SỐ CẤU HÌNH ---
#define MEASURE_INTERVAL_MS 2000 // Chu kỳ đọc cảm biến: 2000ms (2 giây)

// --- BIẾN TOÀN CỤC ---
CustomAdv_t sData;
static app_timer_t update_timer;
static uint8_t advertising_set_handle = 0xff;
static GLIB_Context_t glibContext;

// Các biến lưu giá trị thô đọc từ DHT11 (5 bytes)
uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM; // Byte kiểm tra lỗi (Checksum)

// Biến đếm số lần đo (để biết hệ thống còn chạy hay treo)
static uint32_t measure_count = 0;


// Biến lưu MAC Address
static char mac_display_str[30] = "MAC: Loading...";


// --- HÀM KHỞI TẠO MÀN HÌNH ---
void init_lcd_system(void) {
    uint32_t status;

    sl_board_enable_display();
    DMD_init(0);
    status = GLIB_contextInit(&glibContext);
    if (status != GLIB_OK) return;

    glibContext.backgroundColor = White;
    glibContext.foregroundColor = Black;
    GLIB_clear(&glibContext);
    GLIB_setFont(&glibContext, (GLIB_Font_t *)&GLIB_FontNormal8x8);

    // Tên trạm
    GLIB_drawStringOnLine(&glibContext, "NHOM 6_DHT11", 0, GLIB_ALIGN_CENTER, 0, 5, true);
    GLIB_drawStringOnLine(&glibContext, " ", 0, GLIB_ALIGN_CENTER, 5, 0, true);

    // Placeholder
    GLIB_drawStringOnLine(&glibContext, "Wait MAC...", 1, GLIB_ALIGN_CENTER, 0, 0, true);
    GLIB_drawStringOnLine(&glibContext, "Wait Sensor...", 3, GLIB_ALIGN_CENTER, 0, 0, true);

    DMD_updateDisplay();
}

// --- HÀM BCD ---
uint8_t make_visual_dec(uint8_t val) {
    if (val > 99) val = 99;
    return ((val / 10) << 4) | (val % 10);
}

// --- HÀM 1: GỬI TÍN HIỆU START (Đánh thức cảm biến) ---
// --- DRIVER DHT11 ---
void DHT11_Start(void) {

  // Bước 1: Cấu hình chân là Output (PushPull) để MCU chủ động điều khiển
    GPIO_PinModeSet(DHT_PORT, DHT_PIN, gpioModePushPull, 1); 

    // Bước 2: Kéo chân xuống mức THẤP (Low) ít nhất 18ms
    // Mục đích: Báo cho DHT11 biết "Hãy thức dậy và chuẩn bị gửi dữ liệu"
    GPIO_PinOutClear(DHT_PORT, DHT_PIN); 
    sl_udelay_wait(20000); // Đợi 20ms (20000 us)

    // Bước 3: Kéo chân lên mức CAO (High)
    GPIO_PinOutSet(DHT_PORT, DHT_PIN);
    sl_udelay_wait(30);

    // Bước 4: Chuyển chân sang chế độ Input (InputPull) 
    // Mục đích: Thả dây để DHT11 nắm quyền điều khiển và gửi phản hồi
    GPIO_PinModeSet(DHT_PORT, DHT_PIN, gpioModeInputPull, 1);
}


// --- HÀM 2: KIỂM TRA PHẢN HỒI (Handshake) ---
uint8_t DHT11_Check_Response(void) {
    uint32_t timeout = 0;
    // Chờ DHT11 kéo chân xuống thấp (Response bắt đầu bằng mức thấp 80us)
    // Nếu chân vẫn ở mức 1 quá lâu -> Lỗi (Return 0)
    while (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 1) { if (timeout++ > 2000) return 0; }
    
    timeout = 0;
    // Chờ hết mức thấp 80us (đợi chân lên mức cao)
    while (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 0) { if (timeout++ > 2000) return 0; }
    
    timeout = 0;
    // Chờ hết mức cao 80us (đợi chân xuống thấp để bắt đầu gửi bit đầu tiên)
    while (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 1) { if (timeout++ > 2000) return 0; }
    return 1;
}


// --- HÀM 3: ĐỌC 1 BYTE (8 BIT) ---
// Đây là kỹ thuật "Bit-banging"
uint8_t DHT11_Read_Byte(void) {
    uint8_t i = 0, j;
    // Vòng lặp đọc 8 bit (từ Bit 7 về Bit 0)
    for (j = 0; j < 8; j++) {

        // 1. Chờ hết khoảng tín hiệu mức thấp đầu mỗi bit (50us)
        // Khi vòng lặp này kết thúc, nghĩa là tín hiệu vừa chuyển từ 0 lên 1
        while (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 0);

        // 2. Delay 35us (Kỹ thuật lấy mẫu ở giữa)
        // Logic: Bit 0 có độ rộng mức cao ~26-28us. Bit 1 có độ rộng ~70us.
        // Ta đợi 35us, tức là vượt qua thời gian của bit 0 nhưng chưa hết bit 1.
        sl_udelay_wait(35);

        // 3. Kiểm tra trạng thái chân NGAY LÚC NÀY
        if (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 1) {
          // Nếu sau 35us mà chân vẫn ở mức 1 -> Đây chắc chắn là Bit 1 (xung dài)
            i |= (1 << (7 - j)); // Ghi bit 1 vào vị trí tương ứng
            // Chờ cho hết phần còn lại của xung mức cao này
            while (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 1);
        }
        // Ngược lại (else): Nếu chân đã xuống 0 -> Đây là Bit 0 (xung ngắn), không cần làm gì cả (bit mặc định là 0).
    }
    return i; // Trả về giá trị byte vừa ghép được
}

// --- CALLBACK TIMER: Chạy mỗi 2 giây ---
static void update_timer_cb(app_timer_t *timer, void *data)
{
  (void)data; (void)timer; // Tránh warning biến không dùng

  // --- QUAN TRỌNG: BẮT ĐẦU CRITICAL SECTION ---
  // Lý do: Việc đọc DHT11 cần chính xác từng micro giây.
  // Nếu có ngắt (như Bluetooth) chen ngang lúc đang delay 35us -> Đo sai -> Đọc sai bit.
  // Hàm này khóa toàn bộ ngắt hệ thống.
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  DHT11_Start(); // Gửi tín hiệu bắt đầu
  uint8_t presence = DHT11_Check_Response(); // Kiểm tra có cảm biến không
  
  if (presence) {
      // Nếu cảm biến phản hồi, đọc liên tiếp 5 bytes
      Rh_byte1 = DHT11_Read_Byte();   // Độ ẩm phần nguyên
      Rh_byte2 = DHT11_Read_Byte();   // Độ ẩm phần thập phân
      Temp_byte1 = DHT11_Read_Byte(); // Nhiệt độ phần nguyên
      Temp_byte2 = DHT11_Read_Byte(); // Nhiệt độ phần thập phân
      SUM = DHT11_Read_Byte();        // Byte kiểm tra (Checksum)
  }

  // --- KẾT THÚC CRITICAL SECTION ---
  // Mở lại ngắt để Bluetooth và các tác vụ khác hoạt động bình thường
  CORE_EXIT_CRITICAL();

  char lcd_buf[32]; // Buffer để format chuỗi hiển thị

  // Kiểm tra tính toàn vẹn dữ liệu (Checksum)
  // Tổng 4 byte đầu phải bằng byte thứ 5 (SUM)
  if (presence && (SUM == (uint8_t)(Rh_byte1 + Rh_byte2 + Temp_byte1 + Temp_byte2))) {

      measure_count++; // Tăng biến đếm

      // --- 1. GỬI DATA LÊN PC QUA UART ---
      // Dùng printf, dữ liệu sẽ đi qua VCOM (USB) lên máy tính
      printf("[OK] TRAM | T:%d.%d C | H:%d.%d %% | Cycle:%dms | Count:%lu\r\n",
             Temp_byte1, Temp_byte2, Rh_byte1, Rh_byte2, MEASURE_INTERVAL_MS, measure_count);

      // --- 2. HIỂN THỊ LÊN LCD ---
      GLIB_clear(&glibContext); // Xóa màn hình cũ
      
      // Vẽ các dòng chữ lên buffer ảo
      GLIB_drawStringOnLine(&glibContext, "NHOM 6_DHT11", 0, GLIB_ALIGN_CENTER, 0, 5, true);
      GLIB_drawStringOnLine(&glibContext, mac_display_str, 1, GLIB_ALIGN_CENTER, 0, 10, true);

      sprintf(lcd_buf, "TEMP: %d.%d C", Temp_byte1, Temp_byte2);
      GLIB_drawStringOnLine(&glibContext, lcd_buf, 3, GLIB_ALIGN_LEFT, 5, 0, true);

      sprintf(lcd_buf, "HUM : %d.%d %%", Rh_byte1, Rh_byte2);
      GLIB_drawStringOnLine(&glibContext, lcd_buf, 4, GLIB_ALIGN_LEFT, 5, 0, true);
      
      // Lệnh này mới thực sự đẩy buffer ảo ra màn hình thật
      DMD_updateDisplay();

      // --- 3. CẬP NHẬT GÓI TIN BLE ADVERTISING ---
      // Đóng gói data vào 1 biến 32-bit để nhét vào gói quảng bá
      // Format: [Temp_Int][Temp_Dec][Hum_Int][ID]
      uint32_t sensor_packed = (make_visual_dec(Temp_byte1) << 24) |
                               (make_visual_dec(Temp_byte2) << 16) |
                               (make_visual_dec(Rh_byte1) << 8)  |
                               0x03; // ID định danh thiết bị

      // Gọi hàm cập nhật Manufacturer Data trong gói tin quảng bá
      // Thiết bị di động khi scan sẽ thấy dữ liệu mới này ngay lập tức
      update_adv_data(&sData, advertising_set_handle, sensor_packed);

  } else {
      // --- XỬ LÝ LỖI ---
      // Nếu Checksum sai hoặc không thấy cảm biến
      printf("[LOI] Checksum PC01 | Cycle: %d ms | Count: %lu\r\n",
             MEASURE_INTERVAL_MS, measure_count);
             
      // Hiển thị báo lỗi lên LCD
      GLIB_clear(&glibContext);
      GLIB_drawStringOnLine(&glibContext, "SENSOR ERROR!", 3, GLIB_ALIGN_CENTER, 0, 0, true);
      DMD_updateDisplay();
  }
}

// --- APP INIT ---
SL_WEAK void app_init(void)
{
  sl_status_t sc;
  printf("\r\n--- START TRAM 3 (Nhom: 6) ---\r\n");

  init_lcd_system();
  sl_sleeptimer_delay_millisecond(2000);

  // Timer đúng chu kỳ MEASURE_INTERVAL_MS
  sc = app_timer_start(&update_timer, MEASURE_INTERVAL_MS, update_timer_cb, NULL, true);
  app_assert_status(sc);
}

SL_WEAK void app_process_action(void) {}

// --- BLUETOOTH EVENTS ---
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;
  bd_addr address;
  uint8_t address_type;

  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_bt_evt_system_boot_id:
      sc = sl_bt_system_get_identity_address(&address, &address_type);
      app_assert_status(sc);

      // Format MAC: AABBCCDDEEFF (12 hex + 4 "MAC:" = 16 chars)
      sprintf(mac_display_str, "%02X%02X%02X%02X%02X%02X",
              address.addr[5], address.addr[4], address.addr[3],
              address.addr[2], address.addr[1], address.addr[0]);

      if (advertising_set_handle == 0xff) {
         sl_bt_advertiser_create_set(&advertising_set_handle);
      }
      sl_bt_advertiser_set_timing(advertising_set_handle, 160, 160, 0, 0);
      sl_bt_advertiser_set_channel_map(advertising_set_handle, 7);

      fill_adv_packet(&sData, FLAG, COMPANY_ID, 0x00000000, "TRAM_DHT11");
      start_adv(&sData, advertising_set_handle);
      break;

    case sl_bt_evt_connection_closed_id:
      sl_bt_legacy_advertiser_generate_data(advertising_set_handle, sl_bt_advertiser_general_discoverable);
      sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_advertiser_connectable_scannable);
      break;

    default:
      break;
  }
}
