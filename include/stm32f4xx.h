/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#ifndef INCLUDE_STM32F4XX_H_
#define INCLUDE_STM32F4XX_H_

#include "STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h"

#define IsInInterruptHandler()  (__get_IPSR() != 0)

#define AreInterruptsEnabled()  (__get_PRIMASK() == 0)

#endif  // INCLUDE_STM32F4XX_H_
