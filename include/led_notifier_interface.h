/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Jason Braley 2017
------------------------------------------------------------------------------*/
#ifndef INCLUDE_LED_NOTIFIER_INTERFACE_H_
#define INCLUDE_LED_NOTIFIER_INTERFACE_H_

#include <system_error>  // NOLINT(build/c++11)

//-----------------------------------------------------------------------------
enum class LedNotifierError {
    LED_NOTIFICATION_UNINITIALIZED = 1,
};

namespace std {
    template <>
    struct is_error_code_enum<LedNotifierError> : public true_type {};
}

std::error_code make_error_code(LedNotifierError error);
std::error_condition make_error_condition(LedNotifierError error);

//-----------------------------------------------------------------------------
enum class LedNotifierState {
    LED_NOTIFIER_STATE_UNKNOWN,
    LED_NOTIFIER_STATE_ON,
    LED_NOTIFIER_STATE_OFF
};

class ILedNotifier {
 public:
    virtual ~ILedNotifier() {}
    virtual std::error_code Initialize() const = 0;
    virtual std::error_code On() const = 0;
    virtual std::error_code Off() const = 0;
    virtual std::error_code Toggle() const = 0;
    virtual LedNotifierState GetState() const = 0;
};

#endif  // INCLUDE_LED_NOTIFIER_INTERFACE_H_
