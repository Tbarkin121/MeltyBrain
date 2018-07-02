/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#ifndef INCLUDE_SPI_INTERFACE_H_
#define INCLUDE_SPI_INTERFACE_H_

// We are standardizing on <system_error> for error handling
#include <system_error>  // NOLINT(build/c++11)

#include "boost/asio/buffer.hpp"

//-----------------------------------------------------------------------------
enum class SPIError {
    SPI_UNINITIALIZED = 1,
    SPI_SLAVE_ALREADY_SELECTED,
    SPI_SLAVE_ALREADY_DESELECTED
};

namespace std {
    template <>
    struct is_error_code_enum<SPIError> : public true_type {};
}

std::error_code make_error_code(SPIError error);
std::error_condition make_error_condition(SPIError error);

//-----------------------------------------------------------------------------
class ISPI {
 public:
        virtual ~ISPI() {}
        virtual std::error_code Initialize() const = 0;
        virtual std::error_code Transmit(
                                    boost::asio::const_buffer tx_buffer,
                                    uint32_t timeout_ms) const = 0;
        virtual std::error_code Receive(
                                    boost::asio::mutable_buffer rx_buffer,
                                    uint32_t timeout_ms) const = 0;
        virtual std::error_code SelectSlave() const = 0;
        virtual std::error_code DeselectSlave() const = 0;
};

#endif  // INCLUDE_SPI_INTERFACE_H_
