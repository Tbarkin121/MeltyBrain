/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Joseph McLaughlin 2017
------------------------------------------------------------------------------*/

#include "include/spi.h"
#include "include/gpio.h"
#include "include/reset_and_clock_control.h"
#include "include/error_categories.h"

//-----------------------------------------------------------------------------
std::error_code SPI::Initialize() const {
    if (initialized_)
        return std::error_code();

    rcc_.InitializeHardware();

    std::error_code error = ConfigurePins(GPIO_AF6_SPI3);
    if (error)
        return error;

    // Chip select is low-enable so disable initially.
    cs_.Set();

    spi_handle_.Instance               = SPI3;
    spi_handle_.Init.Mode              = SPI_MODE_MASTER;
    spi_handle_.Init.Direction         = SPI_DIRECTION_2LINES;
    spi_handle_.Init.DataSize          = SPI_DATASIZE_8BIT;
    spi_handle_.Init.CLKPolarity       = SPI_POLARITY_LOW;
    spi_handle_.Init.CLKPhase          = SPI_PHASE_1EDGE;
    spi_handle_.Init.NSS               = SPI_NSS_SOFT;
    // fAPB1 / BaudRatePrescaler = 42 MHz / 2 = 21 MHz
    spi_handle_.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    spi_handle_.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    spi_handle_.Init.TIMode            = SPI_TIMODE_DISABLE;
    spi_handle_.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    spi_handle_.Init.CRCPolynomial     = 7;

    error = HAL_SPI_Init(&spi_handle_);
    if (error)
        return error;

    initialized_ = true;

    return error;  // no error
}

//-----------------------------------------------------------------------------
std::error_code SPI::ConfigurePins(uint32_t alt_function) const {
    auto error = miso_.Initialize(
        BaseGPIOPin::GetAlternatePushPullConfiguration(alt_function));
    if (error)
        return error;

    error = mosi_.Initialize(
        BaseGPIOPin::GetAlternatePushPullConfiguration(alt_function));
    if (error)
        return error;

    error = sclk_.Initialize(
        BaseGPIOPin::GetAlternatePushPullConfiguration(alt_function));
    if (error)
        return error;

    return cs_.Initialize();
}

//-----------------------------------------------------------------------------
std::error_code SPI::Transmit(boost::asio::const_buffer tx_buffer,
                              uint32_t timeout_ms) const {
    if (!initialized_)
        return SPIError::SPI_UNINITIALIZED;

    uint16_t tx_buffer_size = boost::asio::buffer_size(tx_buffer);

    // Cast the raw tx buffer into uint8_t pointer for the HAL's consumption
    uint8_t* raw_tx_buffer =
        const_cast<uint8_t*>(boost::asio::buffer_cast<const uint8_t*>(tx_buffer));

    return HAL_SPI_Transmit(&spi_handle_, raw_tx_buffer, tx_buffer_size,
                            timeout_ms);
}

//-----------------------------------------------------------------------------
std::error_code SPI::Receive(boost::asio::mutable_buffer rx_buffer,
                             uint32_t timeout_ms) const {
    if (!initialized_)
        return SPIError::SPI_UNINITIALIZED;

    uint16_t rx_buffer_size = boost::asio::buffer_size(rx_buffer);

    // Cast the raw rx buffer into uint8_t pointer for the HAL's consumption
    uint8_t* raw_rx_buffer = boost::asio::buffer_cast<uint8_t*>(rx_buffer);

    return HAL_SPI_Receive(&spi_handle_, raw_rx_buffer, rx_buffer_size,
                           timeout_ms);
}

//-----------------------------------------------------------------------------
std::error_code SPI::SelectSlave() const {
    if (!initialized_)
        return SPIError::SPI_UNINITIALIZED;

    if (!cs_.Query())
        return SPIError::SPI_SLAVE_ALREADY_SELECTED;

    cs_.Clear();

    return std::error_code();
}

//-----------------------------------------------------------------------------
std::error_code SPI::DeselectSlave() const {
    if (!initialized_)
        return SPIError::SPI_UNINITIALIZED;

    if (cs_.Query())
        return SPIError::SPI_SLAVE_ALREADY_DESELECTED;

    cs_.Set();

    return std::error_code();
}
