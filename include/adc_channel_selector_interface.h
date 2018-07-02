/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Jason Braley 2017
------------------------------------------------------------------------------*/

#ifndef INCLUDE_ADC_CHANNEL_SELECTOR_INTERFACE_H_
#define INCLUDE_ADC_CHANNEL_SELECTOR_INTERFACE_H_

#include "include/gpio_interface.h"

enum class AdcChannelSelectorError {
    ADC_INVALID_PINS = 1,
    ADC_UNINITIALIZED
};

namespace std {
template <>
struct is_error_code_enum<AdcChannelSelectorError> : public true_type {};
}

std::error_code make_error_code(AdcChannelSelectorError error);
std::error_condition make_error_condition(AdcChannelSelectorError error);

class IAdcChannelSelector {
 public:
    struct ChannelSelectionResult {
        uint32_t value;
        std::error_code error;
    };
    virtual ~IAdcChannelSelector() {}
    virtual ChannelSelectionResult SelectChannel(const IGPIO& gpio) const = 0;
};

#endif  // INCLUDE_ADC_CHANNEL_SELECTOR_INTERFACE_H_
