/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Jared Kirschner 2016
------------------------------------------------------------------------------*/

#include "include/pwm.h"
#include "include/gpio.h"
#include "include/reset_and_clock_control.h"
#include "include/error_categories.h"

//-----------------------------------------------------------------------------
std::error_code PWM::Initialize(PWMConfig config) const {
    if (initialized_)
        return std::error_code();

    rcc_.InitializeHardware();

    auto error = ConfigurePins();
    if (error)
        return error;

    /*
    TIM1 is on APB2
        TIM1CLK = 2 * APB2 clock
        APB2 clock = HCLK / 2
        => TIM1CLK = HCLK = SystemCoreClock = 168MHz ??

    To get the TIM1 counter clock at 56 MHz,
    the prescaler is computed as follows:
        Prescaler = (TIM1CLK / TIM1 counter clock) - 1
        Prescaler = ((SystemCoreClock) / 56 MHz) - 1
                  = 3 - 1 = 2

    To get TIM1 output clock at 1 KHz, the period (ARR) is computed as follows:
        ARR = (TIM1 counter clock / TIM1 output clock) - 1
            = (56 MHz / 1 KHz) - 1 = 56000 - 1
            = 55999

    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%

    The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100

    ----------------------------------------------------------------------- */

    /* Initialize TIMx peripheral as follow:
       + Prescaler = 2
       + Period = 55999
       + ClockDivision = 0
       + Counter direction = Up
    */
    uint32_t prescaler = 0;
    output_range_ = config.max_output_value - config.min_output_value;

    timer_period_ = static_cast<uint32_t>(
        (SystemCoreClock / (prescaler + 1) / config.frequency_hz) - 1);
    timer_mid_ccr_ = (timer_period_ + 1) / 2;
    timer_min_ccr_ = static_cast<uint32_t>(
        static_cast<float>(timer_period_) * config.min_duty_cycle);
    timer_max_ccr_ = static_cast<uint32_t>(
        static_cast<float>(timer_period_) * config.max_duty_cycle);
    timer_range_ccr_ = timer_max_ccr_ - timer_min_ccr_;


    tim_handle_.State = HAL_TIM_STATE_RESET;

    tim_handle_.Instance = TIM4;

    tim_handle_.Init.Prescaler = prescaler;
    tim_handle_.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim_handle_.Init.Period = 60000;
    tim_handle_.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    
    tim_handle_.Init.RepetitionCounter = 0;

    error = HAL_TIM_PWM_Init(&tim_handle_);
    if (error)
        return error;

    tim_output_capture_config_.OCMode       = TIM_OCMODE_PWM1;
    tim_output_capture_config_.Pulse        = 30000;
    tim_output_capture_config_.OCPolarity   = TIM_OCPOLARITY_HIGH;
    tim_output_capture_config_.OCFastMode   = TIM_OCFAST_DISABLE;
    
    // tim_output_capture_config_.OCIdleState  = TIM_OCIDLESTATE_RESET;
    // tim_output_capture_config_.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    // tim_output_capture_config_.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    

    error = HAL_TIM_PWM_ConfigChannel(
        &tim_handle_, &tim_output_capture_config_, channel_);
    if (error)
        return error;

    config_ = config;

    initialized_ = true;

    return error;
}


std::error_code PWM::Enable() const {
    return HAL_TIM_PWM_Start(&tim_handle_, channel_);
}

std::error_code PWM::Disable() const {
    return HAL_TIM_PWM_Stop(&tim_handle_, channel_);
}

std::error_code PWM::ConfigurePins() const {
    // The PWM instance is fixed to TIM1.
    if (pwm_pin_.Port() == GPIOA) {
        // GPIOA 8 - 11 valid
        switch (pwm_pin_.Pin()) {
            case GPIO_PIN_8:
                channel_ = TIM_CHANNEL_1;
                break;
            case GPIO_PIN_9:
                channel_ = TIM_CHANNEL_2;
                break;
            case GPIO_PIN_10:
                channel_ = TIM_CHANNEL_3;
                break;
            case GPIO_PIN_11:
                channel_ = TIM_CHANNEL_4;
                break;
            default:
                return PWMError::INVALID_PINS;
        }

        // Pin was determined to be valid. Configure for PWM output.
        return pwm_pin_.Initialize(
            BaseGPIOPin::GetAlternatePushPullConfiguration(GPIO_AF1_TIM1));
    }

    if (pwm_pin_.Port() == GPIOD) {
        // GPIOA 12 - 15 valid
        switch (pwm_pin_.Pin()) {
            case GPIO_PIN_12:
                channel_ = TIM_CHANNEL_1;
                break;
            case GPIO_PIN_13:
                channel_ = TIM_CHANNEL_2;
                break;
            case GPIO_PIN_14:
                channel_ = TIM_CHANNEL_3;
                break;
            case GPIO_PIN_15:
                channel_ = TIM_CHANNEL_4;
                break;
            default:
                return PWMError::INVALID_PINS;
        }

        // Pin was determined to be valid. Configure for PWM output.
        return pwm_pin_.Initialize( BaseGPIOPin::GetAlternatePushPullConfiguration(GPIO_AF2_TIM4) );
    }

    // No valid pin configuration matched (would have returned early)
    return PWMError::INVALID_PINS;
}

std::error_code PWM::SetOutput(float output_value) const {
    if (!initialized_)
        return PWMError::UNINITIALIZED;

    if (output_value < config_.min_output_value)
        output_value = config_.min_output_value;
    else if (output_value > config_.max_output_value)
        output_value = config_.max_output_value;

    // See what percent it is within range
    float range_percent = (output_value - config_.min_output_value) / output_range_;
    uint32_t ccr = timer_min_ccr_ + static_cast<uint32_t>(
        static_cast<float>(timer_range_ccr_) * range_percent);

        // Shameful hack because storing *ccr_register_ in ConfigurePins()
        // fails exactly every other time you execute for some god forsaken
        // reason.
        switch (pwm_pin_.Pin()) {
            case GPIO_PIN_12:
                tim_handle_.Instance->CCR1 = ccr;
                break;
            case GPIO_PIN_13:
                tim_handle_.Instance->CCR2 = ccr;
                break;
            case GPIO_PIN_14:
                tim_handle_.Instance->CCR3 = ccr;
                break;
            case GPIO_PIN_15:
                tim_handle_.Instance->CCR4 = ccr;
                break;
            default:
                return PWMError::INVALID_PINS;
        }

    // Call to HAL_TIM_PWM_Start not needed again; register can be modified directly
    return std::error_code();
}

std::error_code PWM::SetCCR(uint32_t ccr) const {
    if (!initialized_){
        return PWMError::UNINITIALIZED;
    }
        // Shameful hack because storing *ccr_register_ in ConfigurePins()
        // fails exactly every other time you execute for some god forsaken
        // reason.
        switch (pwm_pin_.Pin()) {
            case GPIO_PIN_12:
                tim_handle_.Instance->CCR1 = ccr;
                break;
            case GPIO_PIN_13:
                tim_handle_.Instance->CCR2 = ccr;
                break;
            case GPIO_PIN_14:
                tim_handle_.Instance->CCR3 = ccr;
                break;
            case GPIO_PIN_15:
                tim_handle_.Instance->CCR4 = ccr;
                break;
            default:
                return PWMError::INVALID_PINS;
        }

    // Call to HAL_TIM_PWM_Start not needed again; register can be modified directly
    return std::error_code();
}