/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#include "include/uart.h"
#include "include/gpio.h"
#include "include/reset_and_clock_control.h"
#include "include/error_categories.h"

//-----------------------------------------------------------------------------
std::error_code UART::Initialize() const {
    if (initialized_)
        return std::error_code();

    rcc_.InitializeHardware();

    std::error_code error = ConfigurePins();
    if (error)
        return error;

    uart_handle_.Instance = USART3;
    uart_handle_.Init.BaudRate = 115200;
    uart_handle_.Init.WordLength = UART_WORDLENGTH_8B;
    uart_handle_.Init.StopBits = UART_STOPBITS_1;
    uart_handle_.Init.Parity = UART_PARITY_NONE;
    uart_handle_.Init.Mode = UART_MODE_TX_RX;
    uart_handle_.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
    uart_handle_.Init.OverSampling = UART_OVERSAMPLING_16;

    error = HAL_UART_Init(&uart_handle_);
    if (error)
        return error;

    initialized_ = true;

    return error;  // no error
}

std::error_code UART::ConfigurePins() const {
    // Check whether configuration of GPIO pins TX, RX, CTS, RTS is valid for USART3
    if ((tx_.Port() == GPIOB) && (tx_.Pin() == GPIO_PIN_10) &&
        (rx_.Port() == GPIOB) && (rx_.Pin() == GPIO_PIN_11) &&
        (cts_.Port() == GPIOB) && (cts_.Pin() == GPIO_PIN_13) &&
        (rts_.Port() == GPIOB) && (rts_.Pin() == GPIO_PIN_14)) {
        // Pins match alternate configuration GPIO_AF7_USART3
        auto error = tx_.Initialize(
            BaseGPIOPin::GetAlternatePushPullConfiguration(GPIO_AF7_USART3));
        if (error)
            return error;

        error = rx_.Initialize(
            BaseGPIOPin::GetAlternatePushPullConfiguration(GPIO_AF7_USART3));
        if (error)
            return error;

        error = cts_.Initialize(
            BaseGPIOPin::GetAlternatePushPullConfiguration(GPIO_AF7_USART3));
        if (error)
            return error;

        error = rts_.Initialize(
            BaseGPIOPin::GetAlternatePushPullConfiguration(GPIO_AF7_USART3));
        if (error)
            return error;

        return error;
    }

    // No valid pin configuration matched (would have returned early)
    return UARTError::UART_INVALID_PINS;
}

//-----------------------------------------------------------------------------
std::error_code UART::Write(boost::asio::const_buffer buffer, uint32_t timeout) const {
    if (!initialized_)
        return UARTError::UART_UNINITIALIZED;

    uint8_t* raw_buffer = const_cast<uint8_t*>(boost::asio::buffer_cast<const uint8_t*>(buffer));
    std::error_code error = HAL_UART_Transmit(&uart_handle_,
                                                raw_buffer,
                                                boost::asio::buffer_size(buffer),
                                                timeout);
    return error;
}

//-----------------------------------------------------------------------------
std::error_code UART::Read(boost::asio::mutable_buffer buffer, uint32_t timeout) const {
    if (!initialized_)
        return UARTError::UART_UNINITIALIZED;

    std::error_code error = HAL_UART_Receive(&uart_handle_,
                                                boost::asio::buffer_cast<uint8_t*>(buffer),
                                                boost::asio::buffer_size(buffer),
                                                timeout);
    return error;
}
