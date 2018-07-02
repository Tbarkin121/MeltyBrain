/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2016
------------------------------------------------------------------------------*/

#ifndef INCLUDE_HARDWARE_H_
#define INCLUDE_HARDWARE_H_

#include "include/hardware_interface.h"

#include "include/gpio_interface.h"
#include "include/reset_and_clock_control_interface.h"

class Hardware : public IHardware {
 public:
        Hardware(const IResetAndClockControl& rcc,
                 const IOutputPin& led1)
                 : rcc_(rcc),
                   led1_(led1) {}

        std::error_code Initialize() const;
        const IResetAndClockControl& ResetAndClockControl() const { return rcc_; }
        const IOutputPin& Led1() const { return led1_; }

 protected:
        const IResetAndClockControl& rcc_;
        const IOutputPin& led1_;
};

#endif  // INCLUDE_HARDWARE_H_
