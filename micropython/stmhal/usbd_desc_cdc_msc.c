/*
 * This file is part of the Micro Python project, http://micropython.org/
 */

/**
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/Src/usbd_desc.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    26-February-2014
  * @brief   This file provides the USBD descriptors and string formating method.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_conf.h"

// So we don't clash with existing ST boards, we use the unofficial FOSS VID.
// This needs a proper solution.
#define USBD_VID                      0xf055
#define USBD_PID                      0x9800
#define USBD_LANGID_STRING            0x409
#define USBD_MANUFACTURER_STRING      "Micro Python"
#define USBD_PRODUCT_HS_STRING        "Pyboard Virtual Comm Port in HS Mode"
#define USBD_SERIALNUMBER_HS_STRING   "000000000010"
#define USBD_PRODUCT_FS_STRING        "Pyboard Virtual Comm Port in FS Mode"
#define USBD_SERIALNUMBER_FS_STRING   "000000000011"
#define USBD_CONFIGURATION_HS_STRING  "VCP Config"
#define USBD_INTERFACE_HS_STRING      "VCP Interface"
#define USBD_CONFIGURATION_FS_STRING  "VCP Config"
#define USBD_INTERFACE_FS_STRING      "VCP Interface"

// USB Standard Device Descriptor
__ALIGN_BEGIN static uint8_t hUSBDDeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END = {
    0x12,                       // bLength
    USB_DESC_TYPE_DEVICE,       // bDescriptorType
    0x00,                       // bcdUSB
    0x02,
    0x00,                       // bDeviceClass
    0x00,                       // bDeviceSubClass
    0x00,                       // bDeviceProtocol
    USB_MAX_EP0_SIZE,           // bMaxPacketSize
    LOBYTE(USBD_VID),           // idVendor
    HIBYTE(USBD_VID),           // idVendor
    LOBYTE(USBD_PID),           // idVendor
    HIBYTE(USBD_PID),           // idVendor
    0x00,                       // bcdDevice rel. 2.00
    0x02,
    USBD_IDX_MFC_STR,           // Index of manufacturer string
    USBD_IDX_PRODUCT_STR,       // Index of product string
    USBD_IDX_SERIAL_STR,        // Index of serial number string
    USBD_MAX_NUM_CONFIGURATION  // bNumConfigurations
};

__ALIGN_BEGIN static uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END = {
    USB_LEN_LANGID_STR_DESC,
    USB_DESC_TYPE_STRING,
    LOBYTE(USBD_LANGID_STRING),
    HIBYTE(USBD_LANGID_STRING),
};

__ALIGN_BEGIN static uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

// set the PID
void USBD_SetPID(uint16_t pid) {
    hUSBDDeviceDesc[10] = LOBYTE(pid);
    hUSBDDeviceDesc[11] = HIBYTE(pid);
}

/**
  * @brief  Returns the device descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
STATIC uint8_t *USBD_VCP_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
    *length = sizeof(hUSBDDeviceDesc);
    return hUSBDDeviceDesc;
}

/**
  * @brief  Returns the LangID string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
STATIC uint8_t *USBD_VCP_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
    *length = sizeof(USBD_LangIDDesc);
    return USBD_LangIDDesc;
}

/**
  * @brief  Returns the product string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
STATIC uint8_t *USBD_VCP_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
    if(speed == 0) {
        USBD_GetString((uint8_t *)USBD_PRODUCT_HS_STRING, USBD_StrDesc, length);
    } else {
        USBD_GetString((uint8_t *)USBD_PRODUCT_FS_STRING, USBD_StrDesc, length);
    }
    return USBD_StrDesc;
}

/**
  * @brief  Returns the manufacturer string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
STATIC uint8_t *USBD_VCP_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
    USBD_GetString((uint8_t *)USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
    return USBD_StrDesc;
}

/**
  * @brief  Returns the serial number string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
STATIC uint8_t *USBD_VCP_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
    if(speed == USBD_SPEED_HIGH) {
        USBD_GetString((uint8_t *)USBD_SERIALNUMBER_HS_STRING, USBD_StrDesc, length);
    } else {
        USBD_GetString((uint8_t *)USBD_SERIALNUMBER_FS_STRING, USBD_StrDesc, length);
    }
    return USBD_StrDesc;
}

/**
  * @brief  Returns the configuration string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
STATIC uint8_t *USBD_VCP_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
    if(speed == USBD_SPEED_HIGH) {
        USBD_GetString((uint8_t *)USBD_CONFIGURATION_HS_STRING, USBD_StrDesc, length);
    } else {
        USBD_GetString((uint8_t *)USBD_CONFIGURATION_FS_STRING, USBD_StrDesc, length);
    }
    return USBD_StrDesc;
}

/**
  * @brief  Returns the interface string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
STATIC uint8_t *USBD_VCP_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
    if(speed == 0) {
        USBD_GetString((uint8_t *)USBD_INTERFACE_HS_STRING, USBD_StrDesc, length);
    } else {
        USBD_GetString((uint8_t *)USBD_INTERFACE_FS_STRING, USBD_StrDesc, length);
    }
    return USBD_StrDesc;
}

const USBD_DescriptorsTypeDef VCP_Desc = {
    USBD_VCP_DeviceDescriptor,
    USBD_VCP_LangIDStrDescriptor,
    USBD_VCP_ManufacturerStrDescriptor,
    USBD_VCP_ProductStrDescriptor,
    USBD_VCP_SerialStrDescriptor,
    USBD_VCP_ConfigStrDescriptor,
    USBD_VCP_InterfaceStrDescriptor,
};

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
