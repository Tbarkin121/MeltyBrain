/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Jared Kirschner 2016
------------------------------------------------------------------------------*/

#ifndef INCLUDE_PWM_INTERFACE_H_
#define INCLUDE_PWM_INTERFACE_H_

#include <system_error>  // NOLINT(build/c++11)

//-----------------------------------------------------------------------------
enum class PWMError {
    UNINITIALIZED = 1,
    INVALID_PINS
};

namespace std {
    template <>
    struct is_error_code_enum<PWMError> : public true_type {};
}

std::error_code make_error_code(PWMError error);
std::error_condition make_error_condition(PWMError error);

//-----------------------------------------------------------------------------
class IPWM {
 public:
        struct PWMConfig {
            uint32_t frequency_hz;
            float min_duty_cycle;
            float max_duty_cycle;
            float min_output_value;
            float max_output_value;
        };

        virtual ~IPWM() {}

        virtual std::error_code Initialize(PWMConfig config) const = 0;
        virtual std::error_code Enable() const = 0;
        virtual std::error_code Disable() const = 0;
        virtual std::error_code SetOutput(float output_value) const = 0;
        virtual std::error_code SetCCR(uint32_t ccr) const = 0;
};

#endif  // INCLUDE_PWM_INTERFACE_H_
