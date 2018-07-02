/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2015
------------------------------------------------------------------------------*/

#ifndef INCLUDE_I2C_INTERFACE_H_
#define INCLUDE_I2C_INTERFACE_H_

// We are standardizing on <system_error> for error handling
#include <system_error>  // NOLINT(build/c++11)

#include "boost/asio/buffer.hpp"

class II2C {
 public:
        virtual ~II2C() {}
        virtual std::error_code Initialize() const = 0;
        virtual std::error_code Write(uint8_t address,
                                      boost::asio::const_buffer buffer,
                                      uint32_t timeout) const = 0;
        virtual std::error_code Read(uint8_t address,
                                     uint16_t subaddress,
                                     boost::asio::mutable_buffer buffer,
                                     uint32_t timeout) const = 0;
};

#endif  // INCLUDE_I2C_INTERFACE_H_
