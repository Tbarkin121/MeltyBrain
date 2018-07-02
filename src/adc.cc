/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2016
------------------------------------------------------------------------------*/

#include "include/adc.h"
#include "include/gpio.h"
#include "include/reset_and_clock_control.h"
#include "include/error_categories.h"

//-----------------------------------------------------------------------------
std::error_code Adc::Initialize() const {
    if (initialized_)
        return std::error_code();

    rcc_.InitializeHardware();

    auto error = ConfigurePins();
    if (error)
        return error;

    // Set up to do single shot ADC conversion.
    //
    // 12 bit resolution - ADC_RESOLUTION_12B (==MAX and slowest)
    //                     resolution can go as low as 6 bit
    // ADC_EOC_SINGLE_CONV - 1 shot end of conversion
    // No external trigger nor DMA
    //
    adc_handle_.Instance = ADC1;
    adc_handle_.State = HAL_ADC_STATE_RESET;

    adc_handle_.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    adc_handle_.Init.Resolution = ADC_RESOLUTION_12B;
    adc_handle_.Init.ScanConvMode = DISABLE;
    adc_handle_.Init.ContinuousConvMode = ENABLE;
    adc_handle_.Init.DiscontinuousConvMode = DISABLE;
    adc_handle_.Init.NbrOfDiscConversion = 0;
    adc_handle_.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc_handle_.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
    adc_handle_.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_handle_.Init.NbrOfConversion = 1;
    adc_handle_.Init.DMAContinuousRequests = DISABLE;
    adc_handle_.Init.EOCSelection = (single_sample_mode_) ? ADC_EOC_SINGLE_CONV : ADC_EOC_SEQ_CONV;

    // adc_channel_config_.Channel set in Adc::ConfigurePins()
    adc_channel_config_.Rank = 1;
    adc_channel_config_.SamplingTime = ADC_SAMPLETIME_144CYCLES;
    adc_channel_config_.Offset = 0;

    error = ConfigureAdc();
    if (error)
        return error;

    initialized_ = true;

    return std::error_code();
}

std::error_code Adc::ConfigurePins() const {
    IAdcChannelSelector::ChannelSelectionResult result = channel_validator_.
        SelectChannel(adc_base_pin_);
    if (result.error)
        return result.error;

    adc_channel_config_.Channel = result.value;

    // Pin was determined to be valid. Configure for analog input.
    return adc_base_pin_.Initialize(
            BaseGPIOPin::GetAnalogConfiguration());
}

std::error_code Adc::ConfigureAdc() const {
    std::error_code error = HAL_ADC_Init(&adc_handle_);
    if (error)
        return error;

    return HAL_ADC_ConfigChannel(&adc_handle_, &adc_channel_config_);
}

//-----------------------------------------------------------------------------
Adc::AdcReading Adc::Read() const {
    if (!initialized_)
        return FormErrorResult(AdcError::ADC_UNINITIALIZED);

    // As multiple instances of Adc share ADC1, the Adc
    // must be reconfigured between each read
    auto error = ConfigureAdc();
    if (error)
        return FormErrorResult(error);

    if ((!single_sample_mode_) && (!adc_active_))
        return FormErrorResult(AdcError::ADC_NOT_STARTED);

    if (single_sample_mode_)
        Start();

    error = HAL_ADC_PollForConversion(&adc_handle_, 500);
    uint32_t value = HAL_ADC_GetValue(&adc_handle_);

    if (single_sample_mode_)
        Stop();

    if (error)
        return FormErrorResult(error);

    uint16_t masked_value = static_cast<uint16_t>(value & 0xfff);  // 12 bit conversion resolution
    AdcReading result = { .raw_value = masked_value, .error = std::error_code() };
    return result;
}

//-----------------------------------------------------------------------------
std::error_code Adc::Start() const {
    if (!adc_active_) {
        std::error_code error = HAL_ADC_Start(&adc_handle_);

        if (error)
            return error;

        adc_active_ = true;
    }

    return std::error_code();
}

//-----------------------------------------------------------------------------
std::error_code Adc::Stop() const {
    if (adc_active_) {
        std::error_code error = HAL_ADC_Stop(&adc_handle_);

        if (error)
            return error;

        adc_active_ = false;
    }

    return std::error_code();
}

//-----------------------------------------------------------------------------
Adc::AdcReading Adc::FormErrorResult(std::error_code error) const {
    AdcReading result = { .raw_value = 0x0000, .error = error};
    return result;
}
