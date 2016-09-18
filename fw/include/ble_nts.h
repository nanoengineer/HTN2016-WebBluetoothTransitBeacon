#ifndef BLE_NTS_H__
#define BLE_NTS_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// BASE UUID: 87de0449-51b5-43c3-9ccb-993004dd54aa

#define NTS_UUID_BASE               {0xAA, 0x54, 0xDD, 0x04, 0x30, 0x99, 0xCB, 0x9C, \
                                    0xC3, 0x43, 0xB5, 0x51, 0xFF, 0xFF, 0xDE, 0x87}
#define NTS_UUID_SERVICE            0x0001
#define NTS_UUID_ROUTE_CHAR         0x0002
#define NTS_UUID_CURRENT_STOP_CHAR  0x0003
#define NTS_UUID_STOP_REQ_CHAR      0x0004
#define NTS_UUID_HELP_REQ_CHAR      0x0005


// Forward declaration of the ble_nts_t type.
typedef struct ble_nts_s ble_nts_t;

typedef void (*ble_nts_stop_req_write_handler_t) (ble_nts_t * p_nts, bool stop_requested);
typedef void (*ble_nts_help_req_write_handler_t) (ble_nts_t * p_nts, bool help_requested);


typedef struct
{
    ble_nts_stop_req_write_handler_t stop_req_write_handler;
    ble_nts_help_req_write_handler_t help_req_write_handler;
    uint16_t                         route_no;
    uint16_t                         current_stop;
} ble_nts_init_t;

/**@brief LED Button Service structure. This structure contains various status information for the service. */
struct ble_nts_s
{
    uint16_t                          service_handle;
    ble_gatts_char_handles_t          route_char_handles;
    ble_gatts_char_handles_t          stop_req_char_handles;
    ble_gatts_char_handles_t          curren_stop_char_handles;
    ble_gatts_char_handles_t          help_req_char_handles;
    uint8_t                           uuid_type;
    uint16_t                          conn_handle;
    ble_nts_stop_req_write_handler_t  stop_req_write_handler;
    ble_nts_help_req_write_handler_t  help_req_write_handler;
};


uint32_t ble_nts_init(ble_nts_t * p_nts, const ble_nts_init_t * p_nts_init);


void ble_nts_on_ble_evt(ble_nts_t * p_nts, ble_evt_t * p_ble_evt);



#ifdef __cplusplus
}
#endif

#endif // BLE_NTS_H__

/** @} */
