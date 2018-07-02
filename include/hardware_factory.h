/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Jason Braley 2017
------------------------------------------------------------------------------*/

#ifndef INCLUDE_HARDWARE_FACTORY_H_
#define INCLUDE_HARDWARE_FACTORY_H_

#include "include/hardware.h"
#include "include/reset_and_clock_control.h"
#include "include/gpio.h"


class HardwareFactory {
 public:
    HardwareFactory() :
        led1_base_pin_(rcc_, GPIOB, GPIO_PIN_1),

        led1_pin_(led1_base_pin_),
        
        hw_(rcc_,
        led1_pin_) {}

    IHardware& GetHardware() { return hw_; }

 // private:
    // System Level
    ResetAndClockControl rcc_;

    // LEDs
    BaseGPIOPin led1_base_pin_;

    OutputPin led1_pin_;

    Hardware hw_;
};

#endif  // INCLUDE_HARDWARE_FACTORY_H_
