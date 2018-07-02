/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2015
------------------------------------------------------------------------------*/

#include "include/i2c.h"
#include "include/gpio.h"
#include "include/reset_and_clock_control.h"
#include "include/error_categories.h"

//-----------------------------------------------------------------------------
std::error_code I2C::Initialize() const {
    if (initialized_)
        return std::error_code();

    rcc_.InitializeHardware();

    std::error_code error = ConfigurePins();
    if (error)
        return error;

    // If the I2C bus in the system ever expands to communicate with more devices
    // then these settings should be perhaps passed in.  However, in this system,
    // there is no need for variation in the bus setup.
    // 100kHz
    // 50% duty cycle
    // Master mode
    i2c_handle_.Instance = I2C1;
    i2c_handle_.State = HAL_I2C_STATE_RESET;
    i2c_handle_.Init.ClockSpeed = 100000;
    i2c_handle_.Init.DutyCycle = I2C_DUTYCYCLE_2;
    i2c_handle_.Init.OwnAddress1 = 0x00;
    i2c_handle_.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c_handle_.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c_handle_.Init.OwnAddress2 = 0x00;
    i2c_handle_.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    i2c_handle_.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    error = HAL_I2C_Init(&i2c_handle_);
    if (error)
        return error;

    initialized_ = true;

    return error;  // no error
}

std::error_code I2C::ConfigurePins() const {
    // The I2C instance is fixed to I2C1.
    // Only alternate GPIO_AF4_I2C1 is configured (SCL: PB8; SDA: PB9)

    // Check whether configuration of GPIO pins SCL and SDA is valid for I2C
    if ((scl_.Port() == GPIOB) && (scl_.Pin() == GPIO_PIN_8)) {
        if ((sda_.Port() == GPIOB) && (sda_.Pin() == GPIO_PIN_9)) {
            // Pins match alternate configuration GPIO_AF4_I2C1
            auto error = scl_.Initialize(
                BaseGPIOPin::GetAlternateOpenDrainConfiguration(GPIO_AF4_I2C1));
            if (error)
                return error;

            error = sda_.Initialize(
                BaseGPIOPin::GetAlternateOpenDrainConfiguration(GPIO_AF4_I2C1));
            return error;
        }
    }

    // No valid pin configuration matched (would have returned early)
    return I2CError::I2C_INVALID_PINS;
}

//-----------------------------------------------------------------------------
std::error_code I2C::Write(uint8_t address,
                           boost::asio::const_buffer buffer, uint32_t timeout) const {
    if (!initialized_)
        return I2CError::I2C_UNINITIALIZED;

    uint8_t* raw_buffer = const_cast<uint8_t*>(boost::asio::buffer_cast<const uint8_t*>(buffer));
    std::error_code error = HAL_I2C_Master_Transmit(&i2c_handle_,
                                                       address,
                                                       raw_buffer,
                                                       boost::asio::buffer_size(buffer),
                                                       timeout);

    return error;
}

//-----------------------------------------------------------------------------
std::error_code I2C::Read(uint8_t address, uint16_t subaddress,
                          boost::asio::mutable_buffer buffer, uint32_t timeout) const {
    if (!initialized_)
        return I2CError::I2C_UNINITIALIZED;

    std::error_code error = HAL_I2C_Mem_Read(&i2c_handle_,
                                                address,
                                                subaddress,
                                                I2C_MEMADD_SIZE_8BIT,
                                                boost::asio::buffer_cast<uint8_t*>(buffer),
                                                boost::asio::buffer_size(buffer),
                                                timeout);

    return error;
}
