/* 
 * Copyright (c) Microsoft
 * Copyright (c) 2024 Eclipse Foundation
 * 
 *  This program and the accompanying materials are made available 
 *  under the terms of the MIT license which is available at
 *  https://opensource.org/license/mit.
 * 
 *  SPDX-License-Identifier: MIT
 * 
 *  Contributors: 
 *     Microsoft         - Initial version
 *     Frédéric Desbiens - 2024 version.
 */

#ifndef _THREADX_PNP_INFO_H
#define _THREADX_PNP_INFO_H

#define DEVICE_INFO_COMPONENT_NAME "deviceInformation"

// Device Info property names
#define DEVICE_INFO_MANUFACTURER_PROPERTY_NAME           "manufacturer"
#define DEVICE_INFO_MODEL_PROPERTY_NAME                  "model"
#define DEVICE_INFO_SW_VERSION_PROPERTY_NAME             "swVersion"
#define DEVICE_INFO_OS_NAME_PROPERTY_NAME                "osName"
#define DEVICE_INFO_PROCESSOR_ARCHITECTURE_PROPERTY_NAME "processorArchitecture"
#define DEVICE_INFO_PROCESSOR_MANUFACTURER_PROPERTY_NAME "processorManufacturer"
#define DEVICE_INFO_TOTAL_STORAGE_PROPERTY_NAME          "totalStorage"
#define DEVICE_INFO_TOTAL_MEMORY_PROPERTY_NAME           "totalMemory"

// Device Info property values
#define DEVICE_INFO_MANUFACTURER_PROPERTY_VALUE           "MXCHIP"
#define DEVICE_INFO_MODEL_PROPERTY_VALUE                  "AZ3166"
#define DEVICE_INFO_SW_VERSION_PROPERTY_VALUE             "6.1.4"
#define DEVICE_INFO_OS_NAME_PROPERTY_VALUE                "Eclipse ThreadX"
#define DEVICE_INFO_PROCESSOR_ARCHITECTURE_PROPERTY_VALUE "Arm Cortex M4"
#define DEVICE_INFO_PROCESSOR_MANUFACTURER_PROPERTY_VALUE "STMicroelectronics"
#define DEVICE_INFO_TOTAL_STORAGE_PROPERTY_VALUE          1024
#define DEVICE_INFO_TOTAL_MEMORY_PROPERTY_VALUE           128

#endif
