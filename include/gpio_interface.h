/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2015
------------------------------------------------------------------------------*/

#ifndef INCLUDE_GPIO_INTERFACE_H_
#define INCLUDE_GPIO_INTERFACE_H_

// We are standardizing on <system_error> for error handling
#include <system_error>  // NOLINT(build/c++11)
#include "include/stm32f4xx.h"

class IGPIO {
 public:
        virtual ~IGPIO() {}

        virtual std::error_code Initialize(GPIO_InitTypeDef gpio_config) const = 0;
        virtual GPIO_TypeDef* Port() const = 0;
        virtual uint16_t Pin() const = 0;
        virtual bool Initialized() const = 0;
};

class IInputPin {
 public:
        virtual ~IInputPin() {}
        virtual std::error_code Initialize() const = 0;
        virtual bool Query() const = 0;
};

class IOutputPin {
 public:
        virtual ~IOutputPin() {}
        virtual std::error_code Initialize() const = 0;
        virtual void Set() const = 0;
        virtual void Clear() const = 0;
        virtual void Toggle() const = 0;
        virtual bool Query() const = 0;
};

#endif  // INCLUDE_GPIO_INTERFACE_H_
