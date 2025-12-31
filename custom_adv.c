#include <string.h>
#include "custom_adv.h"
#include "stdio.h"

// Hàm đóng gói dữ liệu
void fill_adv_packet(CustomAdv_t *pData, uint8_t flags, uint16_t companyID, uint32_t student_id, char *name)
{
  int n;

  // Flags
  pData->len_flags = 0x02;
  pData->type_flags = 0x01;
  pData->val_flags = flags;

  // Manufacturer Data
  pData->len_manuf = 7;  // 1 (Type) + 2 (Company) + 4 (Data)
  pData->type_manuf = 0xFF;
  pData->company_LO = companyID & 0xFF;
  pData->company_HI = (companyID >> 8) & 0xFF;

  // Dữ liệu cảm biến/Student ID (Big Endian cho dễ nhìn trên App)
  pData->student_id_3 = student_id & 0xFF;         // Byte thấp nhất (Decimal)
  pData->student_id_2 = (student_id >> 8) & 0xFF;
  pData->student_id_1 = (student_id >> 16) & 0xFF;
  pData->student_id_0 = (student_id >> 24) & 0xFF; // Byte cao nhất

  // Tên thiết bị
  n = strlen(name);
  if (n > NAME_MAX_LENGTH) {
    pData->type_name = 0x08; // Shortened Name
    n = NAME_MAX_LENGTH;
  } else {
    pData->type_name = 0x09; // Complete Name
  }

  strncpy(pData->name, name, NAME_MAX_LENGTH);
  pData->len_name = 1 + n;

  // Tổng kích thước
  pData->data_size = 3 + (1 + pData->len_manuf) + (1 + pData->len_name);
}

// Hàm bắt đầu quảng bá
void start_adv(CustomAdv_t *pData, uint8_t advertising_set_handle)
{
  sl_status_t sc;

  // Đặt dữ liệu
  sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle, 0, pData->data_size, (const uint8_t *)pData);
  app_assert_status(sc);

  // Bắt đầu phát
  sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_legacy_advertiser_connectable);
  app_assert_status(sc);
}

// Hàm cập nhật dữ liệu (Sensor update)
void update_adv_data(CustomAdv_t *pData, uint8_t advertising_set_handle, uint32_t student_id)
{
  sl_status_t sc;

  // Cập nhật giá trị mới vào struct
  pData->student_id_3 = student_id & 0xFF;
  pData->student_id_2 = (student_id >> 8) & 0xFF;
  pData->student_id_1 = (student_id >> 16) & 0xFF;
  pData->student_id_0 = (student_id >> 24) & 0xFF;

  // Gửi lại gói tin xuống Bluetooth Stack
  sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle, 0, pData->data_size, (const uint8_t *)pData);
  app_assert_status(sc);
}
