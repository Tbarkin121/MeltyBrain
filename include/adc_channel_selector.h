/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2016
------------------------------------------------------------------------------*/

#ifndef INCLUDE_ADC_CHANNEL_SELECTOR_H_
#define INCLUDE_ADC_CHANNEL_SELECTOR_H_

#include "include/adc_channel_selector_interface.h"

class AdcChannelSelector : public IAdcChannelSelector {
 public:
    IAdcChannelSelector::ChannelSelectionResult SelectChannel(const IGPIO& gpio) const;
};

#endif  // INCLUDE_ADC_CHANNEL_SELECTOR_H_
