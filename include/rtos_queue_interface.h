/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#ifndef INCLUDE_RTOS_QUEUE_INTERFACE_H_
#define INCLUDE_RTOS_QUEUE_INTERFACE_H_

#include <system_error>  // NOLINT(build/c++11)

//-----------------------------------------------------------------------------
enum class RTOSQueueError {
    RTOSQUEUE_UNINITIALIZED = 1,
    RTOSQUEUE_CREATE_FAILED,
    RTOSQUEUE_IS_FULL,
    RTOSQUEUE_ITEM_NOT_RECEIVED,
};

namespace std {
    template <>
    struct is_error_code_enum<RTOSQueueError> : public true_type {};
}

std::error_code make_error_code(RTOSQueueError error);
std::error_condition make_error_condition(RTOSQueueError error);

//-----------------------------------------------------------------------------
class IRTOSQueue {
 public:
    virtual ~IRTOSQueue() {}
    virtual std::error_code Initialize() const = 0;
 protected:
    virtual std::error_code Send(const void* item, float timeout_seconds) const = 0;
    virtual std::error_code Receive(void* item, float timeout_seconds) const = 0;
};

#endif  // INCLUDE_RTOS_QUEUE_INTERFACE_H_
