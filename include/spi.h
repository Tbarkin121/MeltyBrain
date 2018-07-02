/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#ifndef INCLUDE_SPI_H_
#define INCLUDE_SPI_H_

#include "include/spi_interface.h"
#include "include/stm32f4xx.h"

//-----------------------------------------------------------------------------
class ResetAndClockControl;
class IGPIO;
class IOutputPin;

//-----------------------------------------------------------------------------
class SPI : public ISPI {
 public:
        explicit SPI(const ResetAndClockControl& rcc, const IGPIO& sclk,
                     const IGPIO& miso, const IGPIO& mosi,
                     const IOutputPin& cs) :
            rcc_(rcc),
            sclk_(sclk),
            miso_(miso),
            mosi_(mosi),
            cs_(cs),
            initialized_(false) {}

        std::error_code Initialize() const;

        // Transmit all of the bytes in the tx_buffer to the remote device
        // Ignore the bytes received from the remote device.
        std::error_code Transmit(boost::asio::const_buffer tx_buffer,
                                 uint32_t timeout_ms) const;

        // Transmit all of the bytes in the rx_buffer to the remote device.
        // It is assumed that these are just filler bytes so they can be set
        // to any innocuous value, e.g. 0. The rx_buffer will be returned
        // with all of the bytes received from the remote device.
        std::error_code Receive(boost::asio::mutable_buffer rx_buffer,
                                uint32_t timeout_ms) const;

        std::error_code SelectSlave() const;
        std::error_code DeselectSlave() const;

 protected:
        const ResetAndClockControl& rcc_;
        const IGPIO& sclk_;
        const IGPIO& miso_;
        const IGPIO& mosi_;
        const IOutputPin& cs_;

        mutable bool initialized_;
        mutable SPI_HandleTypeDef spi_handle_;

        std::error_code ConfigurePins(uint32_t alt_function) const;
};

#endif  // INCLUDE_SPI_H_
