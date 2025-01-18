/*
 * utils.c
 *
 *  Created on: 2021年10月10日
 *      Author: solos
 */

#include "nrf52.h"
#include "nrf_bootloader_info.h"
#include "nrf_nvic.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_sdh.h"
#include "nrf_soc.h"
#include "string.h"
#include "utils2.h"

ret_code_t utils_rand_bytes(uint8_t rand[], uint8_t bytes) {
    ret_code_t err;
    while ((err = sd_rand_application_vector_get(rand, bytes)) == NRF_ERROR_SOC_RAND_NOT_ENOUGH_VALUES) {
    };
    return err;
}

void utils_get_device_id(uint8_t *p_device_id) {
    memcpy(p_device_id, &(NRF_FICR->DEVICEID[0]), 4);
    memcpy(p_device_id + 4, &(NRF_FICR->DEVICEID[1]), 4);
}

void int32_to_bytes_le(uint32_t val, uint8_t *data) {
    data[0] = val >> 24;
    data[1] = val >> 16;
    data[2] = val >> 8;
    data[3] = val >> 0;
}

void enter_dfu() {
    sd_power_gpregret_clr(0, 0);
    sd_power_gpregret_set(0, BOOTLOADER_DFU_START);
    sd_nvic_SystemReset();
}

void system_reboot() { sd_nvic_SystemReset(); }

void go_sleep() { nrf_pwr_mgmt_shutdown(NRF_PWR_MGMT_SHUTDOWN_GOTO_SYSOFF); }