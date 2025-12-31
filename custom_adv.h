#ifndef _CUSTOM_ADV_H_
#define _CUSTOM_ADV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sl_bt_api.h"
#include "app_assert.h"
#include <stdint.h>
#include <string.h>

#define NAME_MAX_LENGTH 14

 // Định nghĩa cờ và ID công ty
#define FLAG  0x06
#define COMPANY_ID  0x02FF

typedef struct
{
  // 3 bytes cho Flags
  uint8_t len_flags;
  uint8_t type_flags;
  uint8_t val_flags;

  // Manufacturer Specific Data
  uint8_t len_manuf;
  uint8_t type_manuf;

  // Company ID (Little-endian)
  uint8_t company_LO;
  uint8_t company_HI;

  // Dữ liệu tùy chỉnh (Student ID / Sensor Data)
  uint8_t student_id_0;
  uint8_t student_id_1;
  uint8_t student_id_2;
  uint8_t student_id_3;

  // Local Name (Độ dài thay đổi)
  uint8_t len_name;
  uint8_t type_name;
  char name[NAME_MAX_LENGTH];

  // Các biến phụ trợ (không gửi đi)
  char dummy;
  uint8_t data_size;
} CustomAdv_t;

// Khai báo hàm
void fill_adv_packet(CustomAdv_t *pData, uint8_t flags, uint16_t companyID, uint32_t student_id, char *name);
void start_adv(CustomAdv_t *pData, uint8_t advertising_set_handle);
void update_adv_data(CustomAdv_t *pData, uint8_t advertising_set_handle, uint32_t student_id);

#ifdef __cplusplus
}
#endif

#endif // _CUSTOM_ADV_H_
