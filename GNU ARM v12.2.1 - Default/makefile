################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/subdir.mk
-include simplicity_sdk_2024.6.2/protocol/bluetooth/src/subdir.mk
-include simplicity_sdk_2024.6.2/protocol/bluetooth/bgcommon/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/udelay/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/system/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/sleeptimer/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/power_manager/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/mpu/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/memory_manager/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/memory_manager/profiler/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/iostream/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/interrupt_manager/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/device_manager/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/device_manager/devices/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/clock_manager/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/security/sl_component/sl_protocol_crypto/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/security/sl_component/sl_mbedtls_support/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/security/sl_component/se_manager/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/radio/rail_lib/plugin/rail_util_rf_path/subdir.mk
-include simplicity_sdk_2024.6.2/platform/radio/rail_lib/plugin/rail_util_pti/subdir.mk
-include simplicity_sdk_2024.6.2/platform/radio/rail_lib/plugin/rail_util_power_manager_init/subdir.mk
-include simplicity_sdk_2024.6.2/platform/radio/rail_lib/plugin/pa-conversions/subdir.mk
-include simplicity_sdk_2024.6.2/platform/middleware/glib/glib/subdir.mk
-include simplicity_sdk_2024.6.2/platform/middleware/glib/fonts/subdir.mk
-include simplicity_sdk_2024.6.2/platform/middleware/glib/dmd/display/subdir.mk
-include simplicity_sdk_2024.6.2/platform/emlib/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/emdrv/ustimer/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/emdrv/nvm3/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/emdrv/dmadrv/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/driver/debug/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/common/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/bootloader/app_properties/subdir.mk
-include simplicity_sdk_2024.6.2/platform/bootloader/api/subdir.mk
-include simplicity_sdk_2024.6.2/platform/Device/SiliconLabs/EFR32MG21/Source/subdir.mk
-include simplicity_sdk_2024.6.2/hardware/driver/memlcd/src/memlcd_usart/subdir.mk
-include simplicity_sdk_2024.6.2/hardware/driver/memlcd/src/subdir.mk
-include simplicity_sdk_2024.6.2/hardware/driver/configuration_over_swo/src/subdir.mk
-include simplicity_sdk_2024.6.2/hardware/board/src/subdir.mk
-include simplicity_sdk_2024.6.2/app/common/util/app_timer/bm/subdir.mk
-include simplicity_sdk_2024.6.2/app/common/util/app_log/subdir.mk
-include simplicity_sdk_2024.6.2/app/bluetooth/common/in_place_ota_dfu/subdir.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := test1
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: test1.axf

# Tool invocations
test1.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "C:\Users\AD\SimplicityStudio\v5_workspace\test1\autogen\linkerfile.ld" -Wl,--wrap=_free_r -Wl,--wrap=_malloc_r -Wl,--wrap=_calloc_r -Wl,--wrap=_realloc_r -Wl,--no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -Map="test1.map" -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o test1.axf -Wl,--start-group "./app.o" "./custom_adv.o" "./main.o" "./sl_gatt_service_device_information.o" "./autogen/gatt_db.o" "./autogen/sl_bluetooth.o" "./autogen/sl_board_default_init.o" "./autogen/sl_event_handler.o" "./autogen/sl_iostream_handles.o" "./autogen/sl_iostream_init_usart_instances.o" "./autogen/sl_power_manager_handler.o" "./simplicity_sdk_2024.6.2/app/bluetooth/common/in_place_ota_dfu/sl_bt_in_place_ota_dfu.o" "./simplicity_sdk_2024.6.2/app/common/util/app_log/app_log.o" "./simplicity_sdk_2024.6.2/app/common/util/app_timer/bm/app_timer.o" "./simplicity_sdk_2024.6.2/hardware/board/src/sl_board_control_gpio.o" "./simplicity_sdk_2024.6.2/hardware/board/src/sl_board_init.o" "./simplicity_sdk_2024.6.2/hardware/driver/configuration_over_swo/src/sl_cos.o" "./simplicity_sdk_2024.6.2/hardware/driver/memlcd/src/memlcd_usart/sl_memlcd_spi.o" "./simplicity_sdk_2024.6.2/hardware/driver/memlcd/src/sl_memlcd.o" "./simplicity_sdk_2024.6.2/hardware/driver/memlcd/src/sl_memlcd_display.o" "./simplicity_sdk_2024.6.2/platform/Device/SiliconLabs/EFR32MG21/Source/startup_efr32mg21.o" "./simplicity_sdk_2024.6.2/platform/Device/SiliconLabs/EFR32MG21/Source/system_efr32mg21.o" "./simplicity_sdk_2024.6.2/platform/bootloader/api/btl_interface.o" "./simplicity_sdk_2024.6.2/platform/bootloader/api/btl_interface_storage.o" "./simplicity_sdk_2024.6.2/platform/bootloader/app_properties/app_properties.o" "./simplicity_sdk_2024.6.2/platform/common/src/sl_assert.o" "./simplicity_sdk_2024.6.2/platform/common/src/sl_core_cortexm.o" "./simplicity_sdk_2024.6.2/platform/common/src/sl_slist.o" "./simplicity_sdk_2024.6.2/platform/common/src/sl_string.o" "./simplicity_sdk_2024.6.2/platform/common/src/sl_syscalls.o" "./simplicity_sdk_2024.6.2/platform/driver/debug/src/sl_debug_swo.o" "./simplicity_sdk_2024.6.2/platform/emdrv/dmadrv/src/dmadrv.o" "./simplicity_sdk_2024.6.2/platform/emdrv/nvm3/src/nvm3_default_common_linker.o" "./simplicity_sdk_2024.6.2/platform/emdrv/nvm3/src/nvm3_hal_flash.o" "./simplicity_sdk_2024.6.2/platform/emdrv/nvm3/src/nvm3_lock.o" "./simplicity_sdk_2024.6.2/platform/emdrv/ustimer/src/ustimer.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_acmp.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_burtc.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_cmu.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_core.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_dbg.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_emu.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_gpcrc.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_gpio.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_i2c.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_iadc.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_ldma.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_letimer.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_msc.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_prs.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_rmu.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_rtcc.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_se.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_system.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_timer.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_usart.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_wdog.o" "./simplicity_sdk_2024.6.2/platform/middleware/glib/dmd/display/dmd_memlcd.o" "./simplicity_sdk_2024.6.2/platform/middleware/glib/fonts/glib_font_narrow_6x8.o" "./simplicity_sdk_2024.6.2/platform/middleware/glib/fonts/glib_font_normal_8x8.o" "./simplicity_sdk_2024.6.2/platform/middleware/glib/fonts/glib_font_number_16x20.o" "./simplicity_sdk_2024.6.2/platform/middleware/glib/glib/bmp.o" "./simplicity_sdk_2024.6.2/platform/middleware/glib/glib/glib.o" "./simplicity_sdk_2024.6.2/platform/middleware/glib/glib/glib_bitmap.o" "./simplicity_sdk_2024.6.2/platform/middleware/glib/glib/glib_circle.o" "./simplicity_sdk_2024.6.2/platform/middleware/glib/glib/glib_line.o" "./simplicity_sdk_2024.6.2/platform/middleware/glib/glib/glib_polygon.o" "./simplicity_sdk_2024.6.2/platform/middleware/glib/glib/glib_rectangle.o" "./simplicity_sdk_2024.6.2/platform/middleware/glib/glib/glib_string.o" "./simplicity_sdk_2024.6.2/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.o" "./simplicity_sdk_2024.6.2/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.o" "./simplicity_sdk_2024.6.2/platform/radio/rail_lib/plugin/rail_util_power_manager_init/sl_rail_util_power_manager_init.o" "./simplicity_sdk_2024.6.2/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.o" "./simplicity_sdk_2024.6.2/platform/radio/rail_lib/plugin/rail_util_rf_path/sl_rail_util_rf_path.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/se_manager/src/sl_se_manager.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/se_manager/src/sl_se_manager_hash.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/se_manager/src/sl_se_manager_signature.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/se_manager/src/sl_se_manager_util.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/se_manager/src/sli_se_manager_mailbox.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_mbedtls_support/src/sl_psa_crypto.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_mbedtls_support/src/sli_psa_crypto.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_protocol_crypto/src/sli_protocol_crypto_radioaes.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_protocol_crypto/src/sli_radioaes_management.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sl_psa_its_nvm3.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_psa_trng.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.o" "./simplicity_sdk_2024.6.2/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.o" "./simplicity_sdk_2024.6.2/platform/service/clock_manager/src/sl_clock_manager.o" "./simplicity_sdk_2024.6.2/platform/service/clock_manager/src/sl_clock_manager_hal_s2.o" "./simplicity_sdk_2024.6.2/platform/service/clock_manager/src/sl_clock_manager_init.o" "./simplicity_sdk_2024.6.2/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.o" "./simplicity_sdk_2024.6.2/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg21.o" "./simplicity_sdk_2024.6.2/platform/service/device_manager/src/sl_device_clock.o" "./simplicity_sdk_2024.6.2/platform/service/device_manager/src/sl_device_peripheral.o" "./simplicity_sdk_2024.6.2/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.o" "./simplicity_sdk_2024.6.2/platform/service/iostream/src/sl_iostream.o" "./simplicity_sdk_2024.6.2/platform/service/iostream/src/sl_iostream_retarget_stdio.o" "./simplicity_sdk_2024.6.2/platform/service/iostream/src/sl_iostream_stdlib_config.o" "./simplicity_sdk_2024.6.2/platform/service/iostream/src/sl_iostream_uart.o" "./simplicity_sdk_2024.6.2/platform/service/iostream/src/sl_iostream_usart.o" "./simplicity_sdk_2024.6.2/platform/service/memory_manager/profiler/src/sli_memory_profiler_stubs.o" "./simplicity_sdk_2024.6.2/platform/service/memory_manager/src/sl_memory_manager.o" "./simplicity_sdk_2024.6.2/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.o" "./simplicity_sdk_2024.6.2/platform/service/memory_manager/src/sl_memory_manager_pool.o" "./simplicity_sdk_2024.6.2/platform/service/memory_manager/src/sl_memory_manager_region.o" "./simplicity_sdk_2024.6.2/platform/service/memory_manager/src/sl_memory_manager_retarget.o" "./simplicity_sdk_2024.6.2/platform/service/memory_manager/src/sli_memory_manager_common.o" "./simplicity_sdk_2024.6.2/platform/service/mpu/src/sl_mpu.o" "./simplicity_sdk_2024.6.2/platform/service/power_manager/src/sl_power_manager.o" "./simplicity_sdk_2024.6.2/platform/service/power_manager/src/sl_power_manager_debug.o" "./simplicity_sdk_2024.6.2/platform/service/power_manager/src/sl_power_manager_hal_s2.o" "./simplicity_sdk_2024.6.2/platform/service/sleeptimer/src/sl_sleeptimer.o" "./simplicity_sdk_2024.6.2/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.o" "./simplicity_sdk_2024.6.2/platform/service/sleeptimer/src/sl_sleeptimer_hal_prortc.o" "./simplicity_sdk_2024.6.2/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtcc.o" "./simplicity_sdk_2024.6.2/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.o" "./simplicity_sdk_2024.6.2/platform/service/system/src/sl_system_init.o" "./simplicity_sdk_2024.6.2/platform/service/system/src/sl_system_process_action.o" "./simplicity_sdk_2024.6.2/platform/service/udelay/src/sl_udelay.o" "./simplicity_sdk_2024.6.2/platform/service/udelay/src/sl_udelay_armv6m_gcc.o" "./simplicity_sdk_2024.6.2/protocol/bluetooth/bgcommon/src/sli_bgcommon_debug_efr32.o" "./simplicity_sdk_2024.6.2/protocol/bluetooth/src/sl_apploader_util_s2.o" "./simplicity_sdk_2024.6.2/protocol/bluetooth/src/sl_bt_stack_init.o" "./simplicity_sdk_2024.6.2/protocol/bluetooth/src/sli_bt_accept_list_config.o" "./simplicity_sdk_2024.6.2/protocol/bluetooth/src/sli_bt_advertiser_config.o" "./simplicity_sdk_2024.6.2/protocol/bluetooth/src/sli_bt_connection_config.o" "./simplicity_sdk_2024.6.2/protocol/bluetooth/src/sli_bt_dynamic_gattdb_config.o" "./simplicity_sdk_2024.6.2/protocol/bluetooth/src/sli_bt_host_adaptation.o" "./simplicity_sdk_2024.6.2/protocol/bluetooth/src/sli_bt_l2cap_config.o" "./simplicity_sdk_2024.6.2/protocol/bluetooth/src/sli_bt_pawr_advertiser_config.o" "./simplicity_sdk_2024.6.2/protocol/bluetooth/src/sli_bt_periodic_advertiser_config.o" "./simplicity_sdk_2024.6.2/protocol/bluetooth/src/sli_bt_sync_config.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/cipher.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/cipher_wrap.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/constant_time.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/platform.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/platform_util.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_aead.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_cipher.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_client.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_driver_wrappers_no_static.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_ecp.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_ffdh.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_hash.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_mac.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_pake.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_rsa.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_se.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_slot_management.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_crypto_storage.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/psa_util.o" "./simplicity_sdk_2024.6.2/util/third_party/mbedtls/library/threading.o" "C:/Users/AD/Downloads/iot/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//protocol/bluetooth/bgcommon/lib/build/gcc/cortex-m33/bgcommon/release/libbgcommon.a" "C:/Users/AD/Downloads/iot/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//protocol/bluetooth/bgstack/ll/lib/libbluetooth_controller_efr32xg21_gcc_release.a" "C:/Users/AD/Downloads/iot/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//protocol/bluetooth/build/gcc/cortex-m33/bt_host/hal/release/libbt_hal_series2.a" "C:/Users/AD/Downloads/iot/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//protocol/bluetooth/build/gcc/cortex-m33/bt_host/release/libbt_host.a" "C:/Users/AD/Downloads/iot/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/emdrv/nvm3/lib/libnvm3_CM33_gcc.a" "C:/Users/AD/Downloads/iot/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/radio/rail_lib/autogen/librail_release/librail_efr32xg21_gcc_release.a" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: test1.out'
	arm-none-eabi-objcopy "test1.axf" "test1.out"
	@echo ' '

	@echo 'Building hex file: test1.hex'
	arm-none-eabi-objcopy -O ihex "test1.axf" "test1.hex"
	@echo ' '

	@echo 'Building bin file: test1.bin'
	arm-none-eabi-objcopy -O binary "test1.axf" "test1.bin"
	@echo ' '

	@echo 'Building s37 file: test1.s37'
	arm-none-eabi-objcopy -O srec "test1.axf" "test1.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "test1.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) test1.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
