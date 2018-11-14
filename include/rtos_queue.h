/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#ifndef INCLUDE_RTOS_QUEUE_H_
#define INCLUDE_RTOS_QUEUE_H_

#include "include/rtos_queue_interface.h"
#include <memory>
#include "FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOS/Source/include/queue.h"

//-----------------------------------------------------------------------------
class RTOSQueueBase : public IRTOSQueue {
 public:
    RTOSQueueBase(uint32_t max_items, uint32_t item_size,
                  uint8_t* item_buffer)
        : handle_(NULL), max_items_(max_items), item_size_(item_size),
          item_buffer_(item_buffer) {}

    std::error_code Initialize() const;

    ~RTOSQueueBase();

 protected:
    std::error_code Send(const void* item, float timeout_seconds) const;
    std::error_code Receive(void* item, float timeout_seconds) const;

 private:
    mutable StaticQueue_t data_structure_;
    mutable QueueHandle_t handle_;
    uint32_t max_items_;
    uint32_t item_size_;
    uint8_t* item_buffer_;
};

//-----------------------------------------------------------------------------
// A queue of std::shared_ptr<ItemType> objects
template <typename ItemType, int MAX_ITEMS>
class RTOSQueue : public RTOSQueueBase {
 public:
    static_assert(MAX_ITEMS > 0, "MAX_ITEMS must be > 0");

    // structure returned by Receive method
    struct ItemInfo {
        std::shared_ptr<ItemType> item;
        std::error_code error;
    };

    RTOSQueue() : RTOSQueueBase(MAX_ITEMS,
                        sizeof(std::shared_ptr<ItemType>), item_buffer_)
    {}

    std::error_code Send(const std::shared_ptr<ItemType>& item,
                         float timeout_seconds) const {
        // Increment the use count by instantiating a temporary
        // shared_ptr by copy constructor. Use the non-allocating,
        // placement form of "new" to avoid the heap allocation.
        uint8_t mem[sizeof(std::shared_ptr<ItemType>)];
        new(mem) std::shared_ptr<ItemType>(item);

        // Send() will memcpy the shared_ptr from the local 'mem'
        // buffer to the FreeRTOS queue.
        return RTOSQueueBase::Send(mem, timeout_seconds);
    }

    ItemInfo Receive(float timeout_seconds) const {
        // Receive() will memcpy the shared_ptr from the FreeRTOS queue
        // into the local 'mem' buffer.
        uint8_t mem[sizeof(std::shared_ptr<ItemType>)];
        ItemInfo item_info;
        item_info.error = RTOSQueueBase::Receive(mem, timeout_seconds);

        // Call std::move() to instantiate a shared_ptr object from
        // the queue without calling the shared_ptr copy constructor.
        // This avoids incrementing the use count.
        if (!item_info.error) {
            auto temp_item = reinterpret_cast<std::shared_ptr<ItemType> *>(mem);
            item_info.item = std::move(*temp_item);
        }
        return item_info;
    }

 private:
    // storage for the maximum number of items that can be in the queue
    // at any one time
    uint8_t item_buffer_[MAX_ITEMS * sizeof(std::shared_ptr<ItemType>)];
};

#endif  // INCLUDE_RTOS_QUEUE_H_
