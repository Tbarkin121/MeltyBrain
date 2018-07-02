/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2015
------------------------------------------------------------------------------*/

#ifndef INCLUDE_TIMER_H_
#define INCLUDE_TIMER_H_

// We are standardizing on <system_error> for error handling
#include <system_error>  // NOLINT(build/c++11)
#include "include/timer_interface.h"
#include "include/gp_timer_id.h"
#include "include/stm32f4xx.h"

//-----------------------------------------------------------------------------
class IResetAndClockControl;

//-----------------------------------------------------------------------------
class Timer : public ITimer {
 public:
        Timer(const IResetAndClockControl& rcc, GpTimerId id, uint32_t frequency_hz);
        ~Timer() { Stop(); }
        std::error_code Initialize() const;
        std::error_code Periodic(float period_seconds) const;
        std::error_code Periodic(float period_seconds, InterruptHandler action) const;
        std::error_code Stop() const;

 protected:
        void Handler() const;

        const IResetAndClockControl& rcc_;
        GpTimerId gp_timer_id_;
        uint32_t frequency_hz_;

        mutable InterruptHandler action_ = nullptr;
        mutable bool initialized_;

        mutable TIM_HandleTypeDef handle_;
        mutable bool started_;
};

#endif  // INCLUDE_TIMER_H_
