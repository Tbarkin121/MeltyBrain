/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2015
------------------------------------------------------------------------------*/

#include "include/gpio.h"
#include "include/reset_and_clock_control.h"

//-----------------------------------------------------------------------------
std::error_code BaseGPIOPin::Initialize(GPIO_InitTypeDef gpio_config) const {
    if (initialized_)
        return std::error_code();

    rcc_.InitializeHardware();

    gpio_config_ = gpio_config;

    gpio_config_.Pin = pin_;

    HAL_GPIO_Init(port_, &gpio_config_);

    initialized_ = true;

    return std::error_code();
}

//-----------------------------------------------------------------------------
// Used for such alternate functions as: I2C
GPIO_InitTypeDef BaseGPIOPin::GetAlternateOpenDrainConfiguration(uint32_t alternate) {
    GPIO_InitTypeDef gpio_config;

    gpio_config.Mode = GPIO_MODE_AF_OD;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Speed = GPIO_SPEED_FAST;
    gpio_config.Alternate = alternate;

    return gpio_config;
}

//-----------------------------------------------------------------------------
// Used for such alternate functions as: Encoder, PWM
GPIO_InitTypeDef BaseGPIOPin::GetAlternatePushPullConfiguration(uint32_t alternate) {
    GPIO_InitTypeDef gpio_config;

    gpio_config.Mode = GPIO_MODE_AF_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Speed = GPIO_SPEED_HIGH;
    gpio_config.Alternate = alternate;

    return gpio_config;
}

//-----------------------------------------------------------------------------
GPIO_InitTypeDef BaseGPIOPin::GetAnalogConfiguration() {
    GPIO_InitTypeDef gpio_config;

    gpio_config.Mode = GPIO_MODE_ANALOG;
    gpio_config.Pull = GPIO_PULLUP;

    return gpio_config;
}

//-----------------------------------------------------------------------------
std::error_code OutputPin::Initialize() const {
    GPIO_InitTypeDef gpio_config;

    gpio_config.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_config.Pull = GPIO_NOPULL;
    gpio_config.Speed = GPIO_SPEED_HIGH;

    if ( pin_state_ ) {
        HAL_GPIO_WritePin(gpio_.Port(), gpio_.Pin(), GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(gpio_.Port(), gpio_.Pin(), GPIO_PIN_RESET);
    }
    return gpio_.Initialize(gpio_config);
}

//-----------------------------------------------------------------------------
void OutputPin::Set() const {
    // Protect if not initialized -- currently fails silently
    if (!gpio_.Initialized())
        return;

    HAL_GPIO_WritePin(gpio_.Port(), gpio_.Pin(), GPIO_PIN_SET);
}

//-----------------------------------------------------------------------------
void OutputPin::Clear() const {
    // Protect if not initialized -- currently fails silently
    if (!gpio_.Initialized())
        return;
    HAL_GPIO_WritePin(gpio_.Port(), gpio_.Pin(), GPIO_PIN_RESET);
}

//-----------------------------------------------------------------------------
void OutputPin::Toggle() const {
    // Protect if not initialized -- currently fails silently
    if (!gpio_.Initialized())
        return;

    HAL_GPIO_TogglePin(gpio_.Port(), gpio_.Pin());
}

//-----------------------------------------------------------------------------
bool OutputPin::Query() const {
    // Protect if not initialized -- currently fails silently
    if (!gpio_.Initialized())
        return false;

    return static_cast<bool>(HAL_GPIO_ReadPin(gpio_.Port(), gpio_.Pin()));
}

//-----------------------------------------------------------------------------
std::error_code InputPin::Initialize() const {
    GPIO_InitTypeDef gpio_config;

    gpio_config.Mode = GPIO_MODE_INPUT;
    gpio_config.Pull = GPIO_NOPULL;
    gpio_config.Speed = GPIO_SPEED_HIGH;

    return gpio_.Initialize(gpio_config);
}

//-----------------------------------------------------------------------------
bool InputPin::Query() const {
    // Protect if not initialized -- currently fails silently
    if (!gpio_.Initialized())
        return false;

    return static_cast<bool>(HAL_GPIO_ReadPin(gpio_.Port(), gpio_.Pin()));
}
