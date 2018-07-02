/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2015
------------------------------------------------------------------------------*/

#ifndef INCLUDE_I2C_H_
#define INCLUDE_I2C_H_

#include "include/i2c_interface.h"
#include "include/stm32f4xx.h"

//-----------------------------------------------------------------------------
enum class I2CError {
    I2C_UNINITIALIZED = 1,
    I2C_INVALID_PINS
};

namespace std {
    template <>
    struct is_error_code_enum<I2CError> : public true_type {};
}

std::error_code make_error_code(I2CError error);
std::error_condition make_error_condition(I2CError error);

//-----------------------------------------------------------------------------
class ResetAndClockControl;
class IGPIO;

//-----------------------------------------------------------------------------
class I2C : public II2C {
 public:
        I2C(const ResetAndClockControl& rcc, const IGPIO& scl, const IGPIO& sda)
                    : rcc_(rcc),  scl_(scl), sda_(sda), initialized_(false) {}
        virtual std::error_code Initialize() const;
        virtual std::error_code Write(uint8_t address,
                                      boost::asio::const_buffer buffer,
                                      uint32_t timeout) const;
        virtual std::error_code Read(uint8_t address,
                                     uint16_t subaddress,
                                     boost::asio::mutable_buffer buffer,
                                     uint32_t timeout) const;
 protected:
        const ResetAndClockControl& rcc_;
        const IGPIO& scl_;
        const IGPIO& sda_;
        mutable bool initialized_;

        mutable I2C_HandleTypeDef i2c_handle_;

        std::error_code ConfigurePins() const;
};

#endif  // INCLUDE_I2C_H_
