#include "df_proto_info.h"
#include "df_buffer.h"
#include "utils2.h"

#include "nrf_log.h"
#include "ble_main.h"
#include "version2.h"
#include "ble_main.h"

void df_proto_handler_info_get_version(df_event_t *evt) {
    if (evt->type == DF_EVENT_DATA_RECEVIED) {
        df_frame_t out;

        NEW_BUFFER_ZERO(buff, out.data, sizeof(out.data));
        buff_put_string(&buff, version_get_version(version_get()));
        char ble_addr[24];
        ble_get_addr_str(ble_addr);
        buff_put_string(&buff, ble_addr);

        OUT_FRAME_WITH_DATA_0(out, DF_PROTO_CMD_INFO_VERSION_INFO, DF_STATUS_OK, buff_get_size(&buff));

        df_core_send_frame(&out);
    }
}

void df_proto_handler_info_enter_dfu(df_event_t *evt) {
    if (evt->type == DF_EVENT_DATA_RECEVIED) {
        df_frame_t out;
        OUT_FRAME_NO_DATA(out, DF_PROTO_CMD_INFO_ENTER_DFU, DF_STATUS_OK);
        df_core_send_frame(&out);
    } else if (evt->type == DF_EVENT_DATA_TRANSMIT_READY) {
        NRF_LOG_INFO("enter dfu ...");
        ble_disable();
        enter_dfu();
    }
}

const df_cmd_entry_t df_proto_handler_info_entries[] = {
    {DF_PROTO_CMD_INFO_VERSION_INFO, df_proto_handler_info_get_version},
    {DF_PROTO_CMD_INFO_ENTER_DFU, df_proto_handler_info_enter_dfu},
    {0, NULL}};