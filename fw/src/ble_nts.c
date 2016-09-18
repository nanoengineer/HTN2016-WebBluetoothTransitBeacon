/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the license.txt file.
 */
#include "sdk_config.h"
#include "ble_nts.h"
#include "ble_srv_common.h"
#include "sdk_common.h"


static void on_connect(ble_nts_t * p_nts, ble_evt_t * p_ble_evt)
{
    p_nts->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


static void on_disconnect(ble_nts_t * p_nts, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_nts->conn_handle = BLE_CONN_HANDLE_INVALID;
}

static void on_write(ble_nts_t * p_nts, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if ((p_evt_write->handle == p_nts->stop_req_char_handles.value_handle) &&
        (p_evt_write->len == 2) &&
        (p_nts->stop_req_write_handler != NULL))
    {
        p_nts->stop_req_write_handler(p_nts, *((uint16_t *)(p_evt_write->data)));
    }

    else if ((p_evt_write->handle == p_nts->help_req_char_handles.value_handle) &&
        (p_evt_write->len == 1) &&
        (p_nts->help_req_write_handler != NULL))
    {
        p_nts->help_req_write_handler(p_nts, *(p_evt_write->data));
    }
}


void ble_nts_on_ble_evt(ble_nts_t * p_nts, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_nts, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_nts, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_nts, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}

static uint32_t route_char_add(ble_nts_t * p_nts, const ble_nts_init_t * p_nts_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_nts->uuid_type;
    ble_uuid.uuid = NTS_UUID_ROUTE_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(p_nts_init->route_no);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(p_nts_init->route_no);
    attr_char_value.p_value      = (uint8_t*)&(p_nts_init->route_no);

    return sd_ble_gatts_characteristic_add(p_nts->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_nts->route_char_handles);
}

static uint32_t current_stop_char_add(ble_nts_t * p_nts, const ble_nts_init_t * p_nts_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_nts->uuid_type;
    ble_uuid.uuid = NTS_UUID_CURRENT_STOP_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(p_nts_init->current_stop);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(p_nts_init->current_stop);
    attr_char_value.p_value      = (uint8_t*)&(p_nts_init->current_stop);

    return sd_ble_gatts_characteristic_add(p_nts->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_nts->curren_stop_char_handles);
}

static uint32_t stop_req_char_add(ble_nts_t * p_nts, const ble_nts_init_t * p_nts_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_nts->uuid_type;
    ble_uuid.uuid = NTS_UUID_STOP_REQ_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint8_t);
    attr_char_value.p_value      = NULL;

    return sd_ble_gatts_characteristic_add(p_nts->service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_nts->stop_req_char_handles);
}

static uint32_t help_req_char_add(ble_nts_t * p_nts, const ble_nts_init_t * p_nts_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_nts->uuid_type;
    ble_uuid.uuid = NTS_UUID_HELP_REQ_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint8_t);
    attr_char_value.p_value      = NULL;

    return sd_ble_gatts_characteristic_add(p_nts->service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_nts->help_req_char_handles);
}

uint32_t ble_nts_init(ble_nts_t * p_nts, const ble_nts_init_t * p_nts_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure.
    p_nts->conn_handle       = BLE_CONN_HANDLE_INVALID;
    p_nts->stop_req_write_handler = p_nts_init->stop_req_write_handler;
    p_nts->help_req_write_handler = p_nts_init->help_req_write_handler;

    // Add service.
    ble_uuid128_t base_uuid = {NTS_UUID_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_nts->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_nts->uuid_type;
    ble_uuid.uuid = NTS_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_nts->service_handle);
    VERIFY_SUCCESS(err_code);

    // Add characteristics.
    err_code = route_char_add(p_nts, p_nts_init);
    VERIFY_SUCCESS(err_code);

    err_code = current_stop_char_add(p_nts, p_nts_init);
    VERIFY_SUCCESS(err_code);

    err_code = stop_req_char_add(p_nts, p_nts_init);
    VERIFY_SUCCESS(err_code);

    err_code = help_req_char_add(p_nts, p_nts_init);
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}

uint32_t ble_nts_on_stop_req(ble_nts_t * p_nts, uint16_t stop_id_requested)
{
	return NRF_SUCCESS;
}

uint32_t ble_nts_on_help_req(ble_nts_t * p_nts, uint8_t help_requested)
{
	return NRF_SUCCESS;
}
