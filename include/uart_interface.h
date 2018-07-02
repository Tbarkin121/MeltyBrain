/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#ifndef INCLUDE_UART_INTERFACE_H_
#define INCLUDE_UART_INTERFACE_H_

// We are standardizing on <system_error> for error handling
#include <system_error>  // NOLINT(build/c++11)

#include "boost/asio/buffer.hpp"

//-----------------------------------------------------------------------------
enum class UARTError {
    UART_UNINITIALIZED = 1,
    UART_INVALID_PINS
};

namespace std {
    template <>
    struct is_error_code_enum<UARTError> : public true_type {};
}

std::error_code make_error_code(UARTError error);
std::error_condition make_error_condition(UARTError error);

//-----------------------------------------------------------------------------
class IUART {
 public:
        virtual ~IUART() {}
        virtual std::error_code Initialize() const = 0;
        virtual std::error_code Write(boost::asio::const_buffer buffer,
                                        uint32_t timeout) const = 0;
        virtual std::error_code Read(boost::asio::mutable_buffer buffer,
                                        uint32_t timeout) const = 0;
};

#endif  // INCLUDE_UART_INTERFACE_H_
