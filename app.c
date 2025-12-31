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
#define DHT_PORT gpioPortC
#define DHT_PIN  1

// --- THÔNG SỐ CẤU HÌNH ---
#define MEASURE_INTERVAL_MS 2000 // Chu kỳ đo 2000ms

// --- BIẾN TOÀN CỤC ---
CustomAdv_t sData;
static app_timer_t update_timer;
static uint8_t advertising_set_handle = 0xff;
static GLIB_Context_t glibContext;

uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM;

// Biến lưu MAC Address
static char mac_display_str[30] = "MAC: Loading...";

// Biến đếm số lần đo
static uint32_t measure_count = 0;

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

// --- DRIVER DHT11 ---
void DHT11_Start(void) {
    GPIO_PinModeSet(DHT_PORT, DHT_PIN, gpioModePushPull, 1);
    GPIO_PinOutClear(DHT_PORT, DHT_PIN);
    sl_udelay_wait(20000);
    GPIO_PinOutSet(DHT_PORT, DHT_PIN);
    sl_udelay_wait(30);
    GPIO_PinModeSet(DHT_PORT, DHT_PIN, gpioModeInputPull, 1);
}

uint8_t DHT11_Check_Response(void) {
    uint32_t timeout = 0;
    while (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 1) { if (timeout++ > 2000) return 0; }
    timeout = 0;
    while (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 0) { if (timeout++ > 2000) return 0; }
    timeout = 0;
    while (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 1) { if (timeout++ > 2000) return 0; }
    return 1;
}

uint8_t DHT11_Read_Byte(void) {
    uint8_t i = 0, j;
    for (j = 0; j < 8; j++) {
        while (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 0);
        sl_udelay_wait(35);
        if (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 1) {
            i |= (1 << (7 - j));
            while (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 1);
        }
    }
    return i;
}

// --- CALLBACK TIMER ---
static void update_timer_cb(app_timer_t *timer, void *data)
{
  (void)data; (void)timer;

  // 1. Đọc cảm biến
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  DHT11_Start();
  uint8_t presence = DHT11_Check_Response();
  if (presence) {
      Rh_byte1 = DHT11_Read_Byte();
      Rh_byte2 = DHT11_Read_Byte();
      Temp_byte1 = DHT11_Read_Byte();
      Temp_byte2 = DHT11_Read_Byte();
      SUM = DHT11_Read_Byte();
  }
  CORE_EXIT_CRITICAL();

  char lcd_buf[32];

  if (presence && (SUM == (uint8_t)(Rh_byte1 + Rh_byte2 + Temp_byte1 + Temp_byte2))) {

      // Tăng số lần đo
      measure_count++;

      // A. Log Console (Hiện cả Cycle và Count)
      printf("[OK] TRAM | T:%d.%d C | H:%d.%d %% | Cycle:%dms | Count:%lu\r\n",
             Temp_byte1, Temp_byte2, Rh_byte1, Rh_byte2, MEASURE_INTERVAL_MS, measure_count);

      // B. Update LCD
      GLIB_clear(&glibContext);

      // Dòng 0: Tên
      GLIB_drawStringOnLine(&glibContext, "NHOM 6_DHT11", 0, GLIB_ALIGN_CENTER, 0, 5, true);
      GLIB_drawStringOnLine(&glibContext, " ", 3, GLIB_ALIGN_CENTER, 0, 5, true);
      // Dòng 1: MAC
      GLIB_drawStringOnLine(&glibContext, mac_display_str, 1, GLIB_ALIGN_CENTER, 0, 10, true);

      // Dòng 3 & 4: Temp & Hum
      sprintf(lcd_buf, "TEMP: %d.%d C", Temp_byte1, Temp_byte2);
      GLIB_drawStringOnLine(&glibContext, lcd_buf, 3, GLIB_ALIGN_LEFT, 5, 0, true);

      sprintf(lcd_buf, "HUM : %d.%d %%", Rh_byte1, Rh_byte2);
      GLIB_drawStringOnLine(&glibContext, lcd_buf, 4, GLIB_ALIGN_LEFT, 5, 0, true);

      // Dòng 6: Hiển thị Chu kỳ (Cycle)
      sprintf(lcd_buf, "Cycle: %d ms", MEASURE_INTERVAL_MS);
      GLIB_drawStringOnLine(&glibContext, lcd_buf, 6, GLIB_ALIGN_CENTER, 0, 0, true);

      // Dòng 7: Hiển thị Số lần đo (Count)
      sprintf(lcd_buf, "Count: %lu", measure_count);
      GLIB_drawStringOnLine(&glibContext, lcd_buf, 7, GLIB_ALIGN_CENTER, 0, 0, true);

      DMD_updateDisplay();

      // C. Bluetooth (Gói ID 01)
      uint32_t sensor_packed = (make_visual_dec(Temp_byte1) << 24) |
                               (make_visual_dec(Temp_byte2) << 16) |
                               (make_visual_dec(Rh_byte1) << 8)  |
                               0x03; // ID

      update_adv_data(&sData, advertising_set_handle, sensor_packed);

  } else {
      // Báo lỗi Console
      printf("[LOI] Checksum PC01 | Cycle: %d ms | Count: %lu\r\n",
             MEASURE_INTERVAL_MS, measure_count);

      // Báo lỗi LCD
      GLIB_clear(&glibContext);
      GLIB_drawStringOnLine(&glibContext, "TRAM3_DHT11", 0, GLIB_ALIGN_CENTER, 0, 5, true);
      GLIB_drawStringOnLine(&glibContext, mac_display_str, 1, GLIB_ALIGN_CENTER, 0, 0, true);
      GLIB_drawStringOnLine(&glibContext, "SENSOR ERROR!", 3, GLIB_ALIGN_CENTER, 0, 0, true);

      // Vẫn hiện thông số hệ thống
      sprintf(lcd_buf, "Cycle: %d ms", MEASURE_INTERVAL_MS);
      GLIB_drawStringOnLine(&glibContext, lcd_buf, 6, GLIB_ALIGN_CENTER, 0, 0, true);
      sprintf(lcd_buf, "Count: %lu", measure_count);
      GLIB_drawStringOnLine(&glibContext, lcd_buf, 7, GLIB_ALIGN_CENTER, 0, 0, true);

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
