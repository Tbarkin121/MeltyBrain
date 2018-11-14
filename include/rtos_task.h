/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#ifndef INCLUDE_RTOS_TASK_H_
#define INCLUDE_RTOS_TASK_H_

#include "include/rtos_task_interface.h"
#include <string>
#include <functional>
#include "FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOS/Source/include/task.h"

//-----------------------------------------------------------------------------
class RTOSTaskBase : public IRTOSTask,
                     public IRTOSRunningTask,
                     protected IRTOSNotifyTask {
 public:
    RTOSTaskBase(const std::string& name,
                 int starting_priority,
                 TaskFunction_t task_code_static,
                 uint32_t stack_depth,
                 StackType_t* stack_buffer_ptr)
                 : name_(name),
                   starting_priority_(starting_priority),
                   task_code_static_(task_code_static),
                   stack_depth_(stack_depth),
                   stack_buffer_ptr_(stack_buffer_ptr),
                   handle_(NULL),
                   notification_value_(0) {}

    std::error_code Start() const;

    std::error_code Suspend() const;

    std::error_code Resume() const;

    bool IsActive() const {return (handle_ != NULL);}

    ~RTOSTaskBase() {Delete();}

 protected:
    constexpr static uint32_t MAX_EVENT_BIT = 32;
    const std::string name_;
    UBaseType_t starting_priority_;
    TaskFunction_t task_code_static_;
    uint32_t stack_depth_;
    StackType_t* stack_buffer_ptr_;
    mutable TaskHandle_t handle_;
    mutable StaticTask_t control_block_;
    mutable uint32_t notification_value_;

    // NOTE: This method is valid only when this is the running task. An error
    // code of RTOSTASK_NOT_RUNNING will be returned otherwise
    std::error_code Delay(float seconds) const;

    // Send a notification intended for this task. Specify the
    // event bit position (0-31).
    std::error_code SendNotification(uint32_t event_bit) const;

    // Receive a notification intended for this task. Block for up to
    // timeout_seconds waiting.
    //
    // NOTE: This method is valid only when this is the running task. An error
    // code of RTOSTASK_NOT_RUNNING will be returned otherwise.
    //
    // NOTE: Upon timeout, an error code of RTOSTASK_TIMED_OUT will be
    // returned and the notification_value will be invalid.
    std::error_code WaitForNotification(float timeout_seconds) const;

    // Check if an event bit position is active after WaitForNotification
    // succeeds.
    bool IsEventBitPending(uint32_t event_bit) const;

 private:
    // Make sure this class is not copyable and not assignable
    RTOSTaskBase(const RTOSTaskBase&) = delete;
    RTOSTaskBase& operator=(const RTOSTaskBase&) = delete;

    // Remove this task from the list of running tasks.
    void Delete();
};

//-----------------------------------------------------------------------------
// NOTE: This class hierarchy avoids the code duplication that would result
// if the RTOSTaskBase methods were included in this RTOSTask template class.
template <typename TaskType, int STACK_BYTES>
class RTOSTask : public RTOSTaskBase {
 public:
    RTOSTask(const std::string& name, int starting_priority,
             std::function<void(const TaskType&)> handler)
        : RTOSTaskBase(name, starting_priority,
                        &RTOSTask::TaskCodeStatic,
                        STACK_DEPTH, stack_buffer_),
        handler_(handler) {}

 private:
    constexpr static uint32_t STACK_DEPTH = (STACK_BYTES / sizeof(StackType_t));

    std::function<void(const TaskType&)> handler_;
    StackType_t stack_buffer_[STACK_DEPTH];

    // The "Task Function" i.e. main entry point of the task
    static void TaskCodeStatic(void *parameters) {
        auto ptr = static_cast<RTOSTaskBase*>(parameters);
        TaskType* task = dynamic_cast<TaskType *>(ptr);
        task->handler_(*task);
    }
};

// Provides generic MainLoop for an RTOS task with simple IPC using
// the 32-bit event value passed in direct-to-task notifications.
// Classes that derive from EventHandler may assign their own meaning to
// each notification bit and set event bit handler methods to handle
// incoming events.
class TaskEventHandler {
 public:
    explicit TaskEventHandler(float notify_wait_timeout_seconds = 0.001f)
            : notify_wait_timeout_seconds_(notify_wait_timeout_seconds)
    {}
    virtual void MainLoop(const IRTOSRunningTask& task);

 protected:
    float notify_wait_timeout_seconds_;
    constexpr static uint32_t MAX_EVENT_BIT = 32;
    std::array<std::function<void()>, MAX_EVENT_BIT> event_bit_handler_;

    std::error_code SetEventBitHandler(uint32_t event_bit,
                                       std::function<void()> handler);

    virtual void NotificationReceived(const IRTOSRunningTask& task);
    virtual void NotificationTimeout(const IRTOSRunningTask& task) {}
    virtual void NotificationError(const IRTOSRunningTask& task,
                                   std::error_code error) {}

 private:
    // Make sure this class is not copyable and not assignable
    TaskEventHandler(const TaskEventHandler&) = delete;
    TaskEventHandler& operator=(const TaskEventHandler&) = delete;
};

#endif  // INCLUDE_RTOS_TASK_H_
