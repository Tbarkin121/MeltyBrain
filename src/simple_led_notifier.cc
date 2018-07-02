/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Jason Braley 2017
------------------------------------------------------------------------------*/

#include "include/simple_led_notifier.h"

//-----------------------------------------------------------------------------
std::error_code SimpleLedNotifier::Initialize() const {
    if (initialized_)
        return std::error_code();

    for (auto it = output_pins_.begin(); it != output_pins_.end(); ++it) {
        IOutputPin* pin = *it;
        std::error_code error = pin->Initialize();
        if (error)
            return error;
    }

    initialized_ = true;

    return std::error_code();  // no error
}

//-----------------------------------------------------------------------------
std::error_code SimpleLedNotifier::On() const {
    if (!initialized_)
        return LedNotifierError::LED_NOTIFICATION_UNINITIALIZED;

    for (auto it = output_pins_.begin(); it != output_pins_.end(); ++it) {
        IOutputPin* pin = *it;
        pin->Set();
    }

    return std::error_code();
}

//-----------------------------------------------------------------------------
std::error_code SimpleLedNotifier::Off() const {
    if (!initialized_)
        return LedNotifierError::LED_NOTIFICATION_UNINITIALIZED;

    for (auto it = output_pins_.begin(); it != output_pins_.end(); ++it) {
        IOutputPin* pin = *it;
        pin->Clear();
    }

    return std::error_code();
}

//-----------------------------------------------------------------------------
std::error_code SimpleLedNotifier::Toggle() const {
    if (!initialized_)
        return LedNotifierError::LED_NOTIFICATION_UNINITIALIZED;

    for (auto it = output_pins_.begin(); it != output_pins_.end(); ++it) {
        IOutputPin* pin = *it;
        pin->Toggle();
    }

    return std::error_code();
}

//-----------------------------------------------------------------------------
LedNotifierState SimpleLedNotifier::GetState() const {
    if (!initialized_)
        return LedNotifierState::LED_NOTIFIER_STATE_UNKNOWN;

    if (output_pins_[0]->Query() == true) {
        return LedNotifierState::LED_NOTIFIER_STATE_ON;
    } else {
        return LedNotifierState::LED_NOTIFIER_STATE_OFF;
    }
}
