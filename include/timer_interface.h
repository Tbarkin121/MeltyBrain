/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2015
------------------------------------------------------------------------------*/

#ifndef INCLUDE_TIMER_INTERFACE_H_
#define INCLUDE_TIMER_INTERFACE_H_

// We are standardizing on <system_error> for error handling
#include <system_error>  // NOLINT(build/c++11)
#include <functional>

enum class TimerError {
    TIMER_UNINITIALIZED = 1,
    TIMER_PRESCALER_OVERFLOW,
    TIMER_PERIOD_UNDERFLOW,
    TIMER_NULLPTR_ACTION,
    TIMER_BAD_ARGUMENT
};

namespace std {
    template <>
    struct is_error_code_enum<TimerError> : public true_type {};
}

std::error_code make_error_code(TimerError error);
std::error_condition make_error_condition(TimerError error);

class ITimer {
 public:
        typedef std::function<void(void)> InterruptHandler;
        virtual ~ITimer() {}

        virtual std::error_code Initialize() const = 0;
        virtual std::error_code Periodic(float timeout_seconds) const = 0;
        virtual std::error_code Periodic(float timeout_seconds,
                                         InterruptHandler action) const = 0;
        virtual std::error_code Stop() const = 0;
};

#endif  // INCLUDE_TIMER_INTERFACE_H_
