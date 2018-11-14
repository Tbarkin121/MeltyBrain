/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#include "include/rtos_task.h"
#include <cmath>
#include "include/stm32f3xx.h"

//-----------------------------------------------------------------------------
std::error_code RTOSTaskBase::Start() const {
    if (handle_)
        return std::error_code();

    handle_ = xTaskCreateStatic(task_code_static_,
                                name_.c_str(),
                                stack_depth_,
                                const_cast<RTOSTaskBase*>(this),
                                starting_priority_,
                                stack_buffer_ptr_,
                                &control_block_);
    return std::error_code();
}

std::error_code RTOSTaskBase::Suspend() const {
    // Calling vTaskSuspend with a NULL handle will result in the calling
    // task being suspended. Avoid this unintentional behavior.
    if (!handle_)
        return RTOSTaskError::RTOSTASK_NOT_STARTED;

    vTaskSuspend(handle_);
    return std::error_code();
}

std::error_code RTOSTaskBase::Resume() const {
    if (!handle_)
        return RTOSTaskError::RTOSTASK_NOT_STARTED;

    vTaskResume(handle_);
    return std::error_code();
}

std::error_code RTOSTaskBase::SendNotification(uint32_t event_bit) const {
    if (!handle_)
        return RTOSTaskError::RTOSTASK_NOT_STARTED;

    if (event_bit >= MAX_EVENT_BIT)
        return RTOSTaskError::RTOSTASK_INVALID_EVENT_BIT;

    // Per www.freertos.org docs, when eAction is set to `eSetBits`
    // "The notification value of the subject task will be bitwise ORed
    // with ulValue. For example, if ulValue is set to 0x01, then bit 0
    // will get set within the subject task's notification value.
    // Likewise if ulValue is 0x04 then bit 2 will get set in the
    // subject task's notification value. In this way the RTOS task
    // notification mechanism can be used as a light weight alternative
    // to an event group."
    const eNotifyAction ACTION = eSetBits;

    // Per www.freertos.org docs, pdPASS will always be returned when
    // eAction is set to 'eSetBits'.
    if (IsInInterruptHandler()) {
        BaseType_t is_higher_priority_task_woken = pdFALSE;
        xTaskNotifyFromISR(handle_, (1 << event_bit), ACTION,
                            &is_higher_priority_task_woken);
        // Force a context switch if a higher priority task was awakened
        portYIELD_FROM_ISR(is_higher_priority_task_woken);
    } else {
        xTaskNotify(handle_, (1 << event_bit), ACTION);
    }

    return std::error_code();
}

std::error_code RTOSTaskBase::WaitForNotification(float timeout_seconds) const {
    if (!handle_)
        return RTOSTaskError::RTOSTASK_NOT_STARTED;

    if (handle_ != xTaskGetCurrentTaskHandle())
        return RTOSTaskError::RTOSTASK_NOT_RUNNING;

    const uint32_t CLEAR_NO_BITS_ON_ENTRY = 0x0000;
    const uint32_t CLEAR_ALL_BITS_ON_EXIT = 0xFFFF;

    notification_value_ = 0;
    float ticks = round(timeout_seconds * static_cast<float>(configTICK_RATE_HZ));
    BaseType_t is_notification_received =
            xTaskNotifyWait(CLEAR_NO_BITS_ON_ENTRY,
                            CLEAR_ALL_BITS_ON_EXIT,
                            &notification_value_,
                            static_cast<TickType_t>(ticks));

    // Per www.freertos.org docs "Returns pdFALSE if the call to
    // xTaskNotifyWait() timed out before a notification was received."
    if (is_notification_received == pdFALSE)
        return RTOSTaskError::RTOSTASK_TIMED_OUT;

    return std::error_code();
}

bool RTOSTaskBase::IsEventBitPending(uint32_t event_bit) const {
    return (event_bit < MAX_EVENT_BIT)
            ? (((1 << event_bit) & notification_value_) != 0)
            : false;
}

std::error_code RTOSTaskBase::Delay(float seconds) const {
    if (!handle_)
        return RTOSTaskError::RTOSTASK_NOT_STARTED;

    if (handle_ != xTaskGetCurrentTaskHandle())
        return RTOSTaskError::RTOSTASK_NOT_RUNNING;

    float ticks = round(seconds * static_cast<float>(configTICK_RATE_HZ));
    vTaskDelay(static_cast<TickType_t>(ticks));

    return std::error_code();
}

void RTOSTaskBase::Delete() {
    if (handle_) {
        vTaskDelete(handle_);
        handle_ = NULL;
    }
}

void TaskEventHandler::MainLoop(const IRTOSRunningTask& task) {
    while (true) {
        auto error = task.WaitForNotification(notify_wait_timeout_seconds_);

        if (!error) {
            NotificationReceived(task);
        } else if (error == RTOSTaskError::RTOSTASK_TIMED_OUT) {
            NotificationTimeout(task);
        } else if (error == RTOSTaskError::RTOSTASK_EXIT) {
            break;
        } else {
            NotificationError(task, error);
        }
    }
}

std::error_code TaskEventHandler::SetEventBitHandler(
                                        uint32_t event_bit,
                                        std::function<void()> handler) {
    if (event_bit >= event_bit_handler_.size())
        return RTOSTaskError::RTOSTASK_INVALID_EVENT_BIT;

    event_bit_handler_[event_bit] = handler;
    return std::error_code();
}

void TaskEventHandler::NotificationReceived(const IRTOSRunningTask& task) {
    // Invoke any handlers associated with an event bit set in the
    // notification value.
    for (uint32_t event_bit = 0;
            event_bit < event_bit_handler_.size(); event_bit++) {
        if (event_bit_handler_[event_bit]
                && task.IsEventBitPending(event_bit)) {
            event_bit_handler_[event_bit]();
        }
    }
}

