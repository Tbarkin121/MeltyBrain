/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#ifndef INCLUDE_UART_H_
#define INCLUDE_UART_H_

#include "include/uart_interface.h"
#include "include/stm32f4xx.h"

//-----------------------------------------------------------------------------
class ResetAndClockControl;
class IGPIO;

//-----------------------------------------------------------------------------
class UART : public IUART {
 public:
        UART(const ResetAndClockControl& rcc, const IGPIO& tx,
             const IGPIO& rx, const IGPIO& cts, const IGPIO& rts) :
             rcc_(rcc), tx_(tx), rx_(rx), cts_(cts), rts_(rts),
             initialized_(false) {}
        virtual std::error_code Initialize() const;
        virtual std::error_code Write(boost::asio::const_buffer buffer,
                                      uint32_t timeout) const;
        virtual std::error_code Read(boost::asio::mutable_buffer buffer,
                                     uint32_t timeout) const;
 protected:
        const ResetAndClockControl& rcc_;
        const IGPIO& tx_;
        const IGPIO& rx_;
        const IGPIO& cts_;
        const IGPIO& rts_;
        mutable bool initialized_;

        mutable UART_HandleTypeDef uart_handle_;

        std::error_code ConfigurePins() const;
};

#endif  // INCLUDE_UART_H_
