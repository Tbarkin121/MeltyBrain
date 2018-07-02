/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2016
------------------------------------------------------------------------------*/

#ifndef INCLUDE_RESET_AND_CLOCK_CONTROL_INTERFACE_H_
#define INCLUDE_RESET_AND_CLOCK_CONTROL_INTERFACE_H_

#include <system_error>  // NOLINT(build/c++11)

//-----------------------------------------------------------------------------
enum class ResetAndClockControlError {
    UNINITIALIZED = 1,
    UNEXPECTED_RESET_REASON
};

namespace std {
template <>
struct is_error_code_enum<ResetAndClockControlError> : public true_type {};
}

std::error_code make_error_code(ResetAndClockControlError error);
std::error_condition make_error_condition(ResetAndClockControlError error);

//-----------------------------------------------------------------------------
class IResetAndClockControl {
 public:
        virtual ~IResetAndClockControl() {}
        virtual void InitializeHardware() const = 0;
        enum ResetSource {
            PowerOnReset,
            BrownOutReset,
            IndependentWatchdogReset,
            PinReset,
            SoftwareReset,
            WindowedWatchdogReset,
            LowPowerReset,
            Unknown
        };
        virtual ResetSource GetResetSource() const = 0;
        virtual void ClearResetSource() const = 0;
};

#endif  // INCLUDE_RESET_AND_CLOCK_CONTROL_INTERFACE_H_
