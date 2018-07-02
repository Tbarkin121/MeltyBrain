/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Mike Attili 2016
------------------------------------------------------------------------------*/

#include "include/error_categories.h"
//#include "include/timer.h"
//#include "include/i2c.h"
//#include "include/adc.h"
//#include "include/system_tick.h"
#include "include/led_notifier_interface.h"
//#include "include/rtc_interface.h"
#include "include/pwm_interface.h"
//#include "include/uart_interface.h"
//#include "include/spi_interface.h"
//#include "include/rtos_task_interface.h"
//#include "include/rtos_queue_interface.h"

//-----------------------------------------------------------------------------

class HalCategory : public std::error_category {
 public:
    static const std::error_category& Instance() {
        static HalCategory instance;
        return instance;
    }

 private:
    const char* name() const noexcept { return "HAL Error"; }
    std::string message(int error) const { return "N/A"; }
};

std::error_code make_error_code(HAL_StatusTypeDef error) {
    return std::error_code(static_cast<int>(error), HalCategory::Instance());
}
std::error_condition make_error_condition(HAL_StatusTypeDef error) {
    return std::error_condition(static_cast<int>(error), HalCategory::Instance());
}

//-----------------------------------------------------------------------------
/*
class ResetAndClockControlCategory : public std::error_category {
 public:
    static const std::error_category& Instance() {
        static ResetAndClockControlCategory instance;
        return instance;
    }

 private:
    const char* name() const noexcept { return "Reset and Clock Control Error"; }
    std::string message(int error) const { return "N/A"; }
};

std::error_code make_error_code(ResetAndClockControlError error) {
    return std::error_code(static_cast<int>(error), ResetAndClockControlCategory::Instance());
}
std::error_condition make_error_condition(ResetAndClockControlError error) {
    return std::error_condition(static_cast<int>(error),
        ResetAndClockControlCategory::Instance());
}
*/
//-----------------------------------------------------------------------------
class LedNotifierCategory : public std::error_category {
 public:
    static const std::error_category& Instance() {
        static LedNotifierCategory instance;
        return instance;
    }

 private:
    const char* name() const noexcept { return "Led Notification Error"; }
    std::string message(int error) const { return "N/A"; }
};

std::error_code make_error_code(LedNotifierError error) {
    return std::error_code(static_cast<int>(error), LedNotifierCategory::Instance());
}
std::error_condition make_error_condition(LedNotifierError error) {
    return std::error_condition(static_cast<int>(error), LedNotifierCategory::Instance());
}

//-----------------------------------------------------------------------------

class PWMCategory : public std::error_category {
 public:
    static const std::error_category& Instance() {
        static PWMCategory instance;
        return instance;
    }

    const char* name() const noexcept { return "PWM Error"; }
    std::string message(int error) const { return "N/A"; }
};

std::error_code make_error_code(PWMError error) {
    return std::error_code(static_cast<int>(error), PWMCategory::Instance());
}
std::error_condition make_error_condition(PWMError error) {
    return std::error_condition(static_cast<int>(error), PWMCategory::Instance());
}

//-----------------------------------------------------------------------------