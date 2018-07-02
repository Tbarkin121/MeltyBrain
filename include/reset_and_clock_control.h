/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Jared Kirschner 2015
------------------------------------------------------------------------------*/

#ifndef INCLUDE_RESET_AND_CLOCK_CONTROL_H_
#define INCLUDE_RESET_AND_CLOCK_CONTROL_H_

#include <stdint.h>
#include "include/reset_and_clock_control_interface.h"

class ResetAndClockControl : public IResetAndClockControl {
 public:
        ResetAndClockControl() {}

        void InitializeHardware() const;
        ResetSource GetResetSource() const;
        void ClearResetSource() const;
        static uint32_t APB1TimerFrequencyHz() { return apb1_timer_frequency_hz_; }
        static uint32_t APB2TimerFrequencyHz() { return apb2_timer_frequency_hz_; }
        static void EnableRTCClock();

 protected:
        void SystemClockConfig() const;
        void InitializeOscillators() const;
        void InitializeBusClocks() const;
        void InitializeSysTick() const;
        void EnablePeripheralClocks() const;
        void EnableCRCClock() const;

        static uint32_t apb1_timer_frequency_hz_;
        static uint32_t apb2_timer_frequency_hz_;
        static bool initialized_;
};

#endif  // INCLUDE_RESET_AND_CLOCK_CONTROL_H_
