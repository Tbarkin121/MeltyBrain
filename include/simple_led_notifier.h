/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Jason Braley 2017
------------------------------------------------------------------------------*/
#ifndef INCLUDE_SIMPLE_LED_NOTIFIER_H_
#define INCLUDE_SIMPLE_LED_NOTIFIER_H_

#include <vector>
#include "include/led_notifier_interface.h"
#include "include/gpio_interface.h"

//-----------------------------------------------------------------------------
class SimpleLedNotifier : public ILedNotifier {
 public:
        explicit SimpleLedNotifier(std::initializer_list<IOutputPin*> output_pins) :
            output_pins_(output_pins),
            initialized_(false) {}

        std::error_code Initialize() const;
        std::error_code On() const;
        std::error_code Off() const;
        std::error_code Toggle() const;
        LedNotifierState GetState() const;

 protected:
        mutable std::vector<IOutputPin*> output_pins_;
        mutable bool initialized_;
};
#endif  // INCLUDE_SIMPLE_LED_NOTIFIER_H_
