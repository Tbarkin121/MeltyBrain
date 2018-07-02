/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Mike Attili 2016
------------------------------------------------------------------------------*/

#ifndef INCLUDE_ERROR_CATEGORIES_H_
#define INCLUDE_ERROR_CATEGORIES_H_

#include <system_error>  // NOLINT(build/c++11)
#include "include/stm32f4xx.h"

enum class HardwareFault {
    HARDWARE_FAULT_WRONG_IMAGE = 1,
    HARDWARE_FAULT_WRONG_CONSTANTS
};

namespace std {
    template <>
    struct is_error_code_enum<HardwareFault> : public true_type {};
}

std::error_code make_error_code(HardwareFault error);
std::error_condition make_error_condition(HardwareFault error);

namespace std {
    template <>
    struct is_error_code_enum<HAL_StatusTypeDef> : public true_type {};
}

std::error_code make_error_code(HAL_StatusTypeDef error);
std::error_condition make_error_condition(HAL_StatusTypeDef error);

#endif  // INCLUDE_ERROR_CATEGORIES_H_
