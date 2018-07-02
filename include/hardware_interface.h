/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2016
------------------------------------------------------------------------------*/

#ifndef INCLUDE_HARDWARE_INTERFACE_H_
#define INCLUDE_HARDWARE_INTERFACE_H_

// We are standardizing on <system_error> for error handling
#include <system_error>  // NOLINT(build/c++11)

class IResetAndClockControl;
class IOutputPin;

class IHardware {
 public:
        virtual ~IHardware() {}
        virtual std::error_code Initialize() const = 0;
        virtual const IResetAndClockControl& ResetAndClockControl() const = 0;
        virtual const IOutputPin& Led1() const = 0;
};

#endif  // INCLUDE_HARDWARE_INTERFACE_H_
