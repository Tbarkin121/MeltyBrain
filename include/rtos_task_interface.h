/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#ifndef INCLUDE_RTOS_TASK_INTERFACE_H_
#define INCLUDE_RTOS_TASK_INTERFACE_H_

#include <system_error>  // NOLINT(build/c++11)

//-----------------------------------------------------------------------------
enum class RTOSTaskError {
    RTOSTASK_NOT_STARTED = 1,
    RTOSTASK_NOT_RUNNING = 2,
    RTOSTASK_TIMED_OUT = 3,
    RTOSTASK_INVALID_EVENT_BIT = 4,
    RTOSTASK_EXIT = 5,
};

namespace std {
    template <>
    struct is_error_code_enum<RTOSTaskError> : public true_type {};
}

std::error_code make_error_code(RTOSTaskError error);
std::error_condition make_error_condition(RTOSTaskError error);

//-----------------------------------------------------------------------------
class IRTOSTask {
 public:
    virtual ~IRTOSTask() {}
    virtual std::error_code Start() const = 0;
    virtual std::error_code Suspend() const = 0;
    virtual std::error_code Resume() const = 0;
};

class IRTOSRunningTask {
 public:
    virtual std::error_code Delay(float seconds) const = 0;
    virtual std::error_code WaitForNotification(float timeout_seconds) const = 0;
    virtual bool IsEventBitPending(uint32_t event_bit) const = 0;
};

class IRTOSNotifyTask {
 public:
    virtual std::error_code SendNotification(uint32_t event_bit) const = 0;
};

#endif  // INCLUDE_RTOS_TASK_INTERFACE_H_
