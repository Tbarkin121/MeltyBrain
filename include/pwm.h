/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Jared Kirschner 2016
------------------------------------------------------------------------------*/

#ifndef INCLUDE_PWM_H_
#define INCLUDE_PWM_H_

#include "include/pwm_interface.h"
#include "include/stm32f4xx.h"



//-----------------------------------------------------------------------------
class ResetAndClockControl;
class IGPIO;

//-----------------------------------------------------------------------------
class PWM : public IPWM {
 public:
        PWM(const ResetAndClockControl& rcc, const IGPIO& pwm_pin) :
            rcc_(rcc), pwm_pin_(pwm_pin), initialized_(false) {}

        std::error_code Initialize(PWMConfig config) const;
        std::error_code Enable() const;
        std::error_code Disable() const;
        std::error_code SetOutput(float output_value) const;
        std::error_code SetCCR(uint32_t ccr) const;


 protected:
        const ResetAndClockControl& rcc_;
        const IGPIO& pwm_pin_;
        mutable bool initialized_;

        mutable PWMConfig config_;
        mutable TIM_HandleTypeDef tim_handle_;
        mutable TIM_OC_InitTypeDef tim_output_capture_config_;
        mutable uint32_t channel_;

        mutable uint32_t timer_period_;
        mutable uint32_t timer_min_ccr_;
        mutable uint32_t timer_mid_ccr_;
        mutable uint32_t timer_max_ccr_;
        mutable uint32_t timer_range_ccr_;
        mutable uint32_t output_range_;

        std::error_code ConfigurePins() const;

};

#endif  // INCLUDE_PWM_H_
