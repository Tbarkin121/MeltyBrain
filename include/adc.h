/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2016
------------------------------------------------------------------------------*/

#ifndef INCLUDE_ADC_H_
#define INCLUDE_ADC_H_

#include "include/adc_interface.h"
#include "include/adc_channel_selector_interface.h"
#include "include/stm32f4xx.h"

//-----------------------------------------------------------------------------
enum class AdcError {
    ADC_UNINITIALIZED = 1,
    ADC_INVALID_PINS,
    ADC_NOT_STARTED,
    ADC_DEPRECATED
};

namespace std {
    template <>
    struct is_error_code_enum<AdcError> : public true_type {};
}

std::error_code make_error_code(AdcError error);
std::error_condition make_error_condition(AdcError error);

//-----------------------------------------------------------------------------
class IResetAndClockControl;
class IGPIO;

//-----------------------------------------------------------------------------
class Adc : public IAdc {
 public:
    // Single-sample mode is currently only mode, as ConfigureAdc
    // must be called for each read
    Adc(const IResetAndClockControl& rcc,
        const IGPIO& adc_base_pin,
        const IAdcChannelSelector& channel_validator,
        const quantity<electric_potential, float> voltage_reference) :
        rcc_(rcc),
        adc_base_pin_(adc_base_pin),
        single_sample_mode_(true),
        initialized_(false),
        adc_active_(false),
        channel_validator_(channel_validator),
        voltage_reference_(voltage_reference) {}

    quantity<electric_potential, float> GetVRef() const { return voltage_reference_; }
    AdcReading Read() const;
    std::error_code Initialize() const;
    std::error_code Start() const;
    std::error_code Stop() const;

 protected:
    const IResetAndClockControl& rcc_;
    const IGPIO& adc_base_pin_;
    const bool single_sample_mode_;
    mutable bool initialized_;
    mutable bool adc_active_;
    const IAdcChannelSelector& channel_validator_;
    const quantity<electric_potential, float> voltage_reference_;

    mutable ADC_HandleTypeDef adc_handle_;
    mutable ADC_ChannelConfTypeDef adc_channel_config_;

    AdcReading FormErrorResult(std::error_code error) const;
    virtual std::error_code ConfigurePins() const;
    std::error_code ConfigureAdc() const;
};

#endif  // INCLUDE_ADC_H_
