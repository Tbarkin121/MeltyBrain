/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#include "include/rtos_queue.h"
#include <cmath>
#include "include/stm32f4xx.h"

//-----------------------------------------------------------------------------
std::error_code RTOSQueueBase::Initialize() const {
    if (handle_)
        return std::error_code();

    handle_ = xQueueCreateStatic(max_items_, item_size_, item_buffer_,
                                 &data_structure_);

    // www.freertos.org says if the queue is created successfully then a
    // handle to the created queue is returned. If the pointer to the queue
    // data structure is NULL then NULL is returned.
    if (!handle_)
        return RTOSQueueError::RTOSQUEUE_CREATE_FAILED;

    return std::error_code();
}

std::error_code RTOSQueueBase::Send(const void* item, float timeout_seconds) const {
    if (!handle_)
        return RTOSQueueError::RTOSQUEUE_UNINITIALIZED;

    BaseType_t status;
    float ticks = round(timeout_seconds * static_cast<float>(configTICK_RATE_HZ));

    if (IsInInterruptHandler()) {
        BaseType_t is_higher_priority_task_woken = pdFALSE;
        status = xQueueSendFromISR(handle_, item, &is_higher_priority_task_woken);
        // Force a context switch if a higher priority task was awakened
        portYIELD_FROM_ISR(is_higher_priority_task_woken);
    } else {
        status = xQueueSend(handle_, item, static_cast<TickType_t>(ticks));
    }

    // www.freertos.org says xQueueSend returns pdTRUE if the item was
    // successfully posted, otherwise errQUEUE_FULL.
    if (status == errQUEUE_FULL)
        return RTOSQueueError::RTOSQUEUE_IS_FULL;

    return std::error_code();
}

std::error_code RTOSQueueBase::Receive(void* item, float timeout_seconds) const {
    if (!handle_)
        return RTOSQueueError::RTOSQUEUE_UNINITIALIZED;

    float ticks = round(timeout_seconds
                            * static_cast<float>(configTICK_RATE_HZ));
    BaseType_t status =
        xQueueReceive(handle_, item, static_cast<TickType_t>(ticks));

    // www.freertos.org says xQueueReceive returns pdTRUE if an item was
    // successfully received from the queue, otherwise pdFALSE.
    if (status == pdFALSE)
        return RTOSQueueError::RTOSQUEUE_ITEM_NOT_RECEIVED;

    return std::error_code();
}

RTOSQueueBase::~RTOSQueueBase() {
    if (handle_)
        vQueueDelete(handle_);
}
