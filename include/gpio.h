/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2015
------------------------------------------------------------------------------*/

#ifndef INCLUDE_GPIO_H_
#define INCLUDE_GPIO_H_

#include "include/gpio_interface.h"

//-----------------------------------------------------------------------------
class IResetAndClockControl;

//-----------------------------------------------------------------------------
class BaseGPIOPin : public IGPIO {
 public:
        BaseGPIOPin(const IResetAndClockControl& rcc, GPIO_TypeDef* port, uint16_t pin) :
            rcc_(rcc), port_(port), pin_(pin), initialized_(false) {}

        GPIO_TypeDef* Port() const { return port_; }
        uint16_t Pin() const { return pin_; }
        bool Initialized() const { return initialized_; }

        std::error_code Initialize(GPIO_InitTypeDef gpio_config) const;

        static GPIO_InitTypeDef GetAlternateOpenDrainConfiguration(uint32_t alternate);
        static GPIO_InitTypeDef GetAlternatePushPullConfiguration(uint32_t alternate);
        static GPIO_InitTypeDef GetAnalogConfiguration();

 protected:
        const IResetAndClockControl& rcc_;
        GPIO_TypeDef* port_;
        uint16_t pin_;

        mutable bool initialized_;

        mutable GPIO_InitTypeDef gpio_config_;
};

//-----------------------------------------------------------------------------
class OutputPin : public IOutputPin {
 public:
        explicit OutputPin(const IGPIO& gpio, bool pin_state = false)
        : gpio_(gpio), pin_state_(pin_state) {}
        std::error_code Initialize() const;
        void Set() const;
        void Clear() const;
        void Toggle() const;
        bool Query() const;

 protected:
        const IGPIO& gpio_;
        bool pin_state_;
};

//-----------------------------------------------------------------------------
class InputPin : public IInputPin {
 public:
        explicit InputPin(const IGPIO& gpio) : gpio_(gpio) {}
        std::error_code Initialize() const;
        bool Query() const;

        static GPIO_InitTypeDef GetAlternateInputConfiguration(uint8_t alternate);

 protected:
        const IGPIO& gpio_;
};

#endif  // INCLUDE_GPIO_H_