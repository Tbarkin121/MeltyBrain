/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2016
------------------------------------------------------------------------------*/

#include "include/timer.h"
#include <limits>
#include "include/reset_and_clock_control.h"
#include "include/error_categories.h"

//-----------------------------------------------------------------------------
Timer::InterruptHandler TimerInterruptHandler[static_cast<int>(GpTimerId::MaxGpTimer) + 1];

//-----------------------------------------------------------------------------
Timer::Timer(const IResetAndClockControl& rcc, GpTimerId id, uint32_t frequency_hz) :
            rcc_(rcc),
            gp_timer_id_(id),
            frequency_hz_(frequency_hz),
            action_(nullptr),
            initialized_(false),
            started_(false) {
    TimerInterruptHandler[static_cast<int>(gp_timer_id_)] = std::bind(&Timer::Handler, this);
}

//-----------------------------------------------------------------------------
std::error_code Timer::Initialize() const {
    if (initialized_)
        return std::error_code();

    rcc_.InitializeHardware();
    initialized_ = true;

    handle_.State = HAL_TIM_STATE_RESET;

    return std::error_code();
}


//-----------------------------------------------------------------------------
std::error_code Timer::Periodic(float period_seconds) const {
    if (!initialized_)
        return TimerError::TIMER_UNINITIALIZED;

    auto timebase_hz = ResetAndClockControl::APB1FrequencyHz();

    switch (gp_timer_id_) {
        case GpTimerId::GpTimer2:
            handle_.Instance = TIM2;
            break;

        case GpTimerId::GpTimer3:
            handle_.Instance = TIM3;
            break;

        case GpTimerId::GpTimer4:
            handle_.Instance = TIM4;
            break;

        case GpTimerId::GpTimer5:
            handle_.Instance = TIM5;
            break;

        case GpTimerId::GpTimer8:
            handle_.Instance = TIM8;
            timebase_hz = ResetAndClockControl::APB2FrequencyHz();
            break;

        default:
            return TimerError::TIMER_BAD_ARGUMENT;
    }

    // Ensure the frequency selected doesn't overflow the prescaler
    float prescaler_raw = (timebase_hz * 2 / frequency_hz_) - 1;
    if (prescaler_raw > std::numeric_limits<uint16_t>::max())
        return TimerError::TIMER_PRESCALER_OVERFLOW;

    // Ensure the period selected is greater than a clock period
    float period_f = period_seconds / (1.0f / static_cast<float>(frequency_hz_));
    if (period_f < 1.0f)
        return TimerError::TIMER_PERIOD_UNDERFLOW;

    uint32_t period = static_cast<uint32_t>(period_f) - 1;

    handle_.Init.Period = period;
    handle_.Init.Prescaler = static_cast<uint16_t>(prescaler_raw);
    handle_.Init.ClockDivision = 0;
    handle_.Init.CounterMode = TIM_COUNTERMODE_UP;
    handle_.Init.RepetitionCounter = 0;

    // Initialize the peripheral
    std::error_code error = HAL_TIM_Base_Init(&handle_);
    if (error)
        return error;

    // Configure clock to signal trigger event on update (i.e. reload of the counter)
    TIM_MasterConfigTypeDef master_config;
    master_config.MasterOutputTrigger = TIM_TRGO_UPDATE;
    master_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    error = HAL_TIMEx_MasterConfigSynchronization(&handle_, &master_config);
    if (error)
        return error;

    if (action_ == nullptr)
        error = HAL_TIM_Base_Start(&handle_);
    else
        error = HAL_TIM_Base_Start_IT(&handle_);
    if (error)
        return error;

    started_ = true;

    return std::error_code();
}

//-----------------------------------------------------------------------------
std::error_code Timer::Periodic(float period_seconds, InterruptHandler action) const {
    if (action == nullptr)
        return TimerError::TIMER_NULLPTR_ACTION;

    action_ = action;

    switch (gp_timer_id_) {
        case GpTimerId::GpTimer2:
            HAL_NVIC_SetPriority(TIM2_IRQn, 4, 0);
            HAL_NVIC_EnableIRQ(TIM2_IRQn);
            break;

        case GpTimerId::GpTimer3:
            HAL_NVIC_SetPriority(TIM3_IRQn, 4, 0);
            HAL_NVIC_EnableIRQ(TIM3_IRQn);
            break;

        case GpTimerId::GpTimer4:
            HAL_NVIC_SetPriority(TIM4_IRQn, 4, 0);
            HAL_NVIC_EnableIRQ(TIM4_IRQn);
            break;

        case GpTimerId::GpTimer5:
            HAL_NVIC_SetPriority(TIM5_IRQn, 4, 0);
            HAL_NVIC_EnableIRQ(TIM5_IRQn);
            break;

        case GpTimerId::GpTimer8:
            HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 4, 0);
            HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
            break;

        default:
            return TimerError::TIMER_BAD_ARGUMENT;
    }

    return Periodic(period_seconds);
}

//-----------------------------------------------------------------------------
std::error_code Timer::Stop() const {
    if (!initialized_)
        return TimerError::TIMER_UNINITIALIZED;

    // Stop the timer interrupts
    if (started_) {
        started_ = false;
        if (action_ == nullptr)
            return HAL_TIM_Base_Stop(&handle_);
        else
            return HAL_TIM_Base_Stop_IT(&handle_);
    }

    return std::error_code();
}

//-----------------------------------------------------------------------------
void Timer::Handler() const {
    if (__HAL_TIM_GET_FLAG(&handle_, TIM_FLAG_UPDATE) != RESET) {
        if (__HAL_TIM_GET_IT_SOURCE(&handle_, TIM_IT_UPDATE) !=RESET) {
            __HAL_TIM_CLEAR_IT(&handle_, TIM_IT_UPDATE);
            if (action_ != nullptr)
                action_();
        }
    }
}

//-----------------------------------------------------------------------------
extern "C" void TIM2_IRQHandler(void) {
    TimerInterruptHandler[static_cast<int>(GpTimerId::GpTimer2)]();
}

//-----------------------------------------------------------------------------
extern "C" void TIM3_IRQHandler(void) {
    TimerInterruptHandler[static_cast<int>(GpTimerId::GpTimer3)]();
}

//-----------------------------------------------------------------------------
extern "C" void TIM4_IRQHandler(void) {
    TimerInterruptHandler[static_cast<int>(GpTimerId::GpTimer4)]();
}

//-----------------------------------------------------------------------------
extern "C" void TIM5_IRQHandler(void) {
    TimerInterruptHandler[static_cast<int>(GpTimerId::GpTimer5)]();
}

//-----------------------------------------------------------------------------
extern "C" void TIM8_UP_TIM13_IRQHandler(void) {
    TimerInterruptHandler[static_cast<int>(GpTimerId::GpTimer8)]();
}
