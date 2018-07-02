/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Jared Kirschner 2015
------------------------------------------------------------------------------*/

#include "include/reset_and_clock_control.h"
#include "include/stm32f4xx.h"
// #include "diag/Trace.h"

//-----------------------------------------------------------------------------
void ResetAndClockControl::InitializeHardware() const {
    if (initialized_)
        return;

    // Initialise the HAL Library; it must be the first function
    // to be executed before the call of any HAL function.
    HAL_Init();

    // Enable HSE Oscillator and activate PLL with HSE as source
    SystemClockConfig();

    // Call the CSMSIS system clock routine to store the clock frequency
    // in the SystemCoreClock global RAM location.
    SystemCoreClockUpdate();

    initialized_ = true;
}

//-----------------------------------------------------------------------------
void ResetAndClockControl::SystemClockConfig() const {
    // Enable the oscillators, the PLL, and the system tick
    InitializeOscillators();
    InitializeBusClocks();
    InitializeSysTick();
    EnablePeripheralClocks();
    EnableRTCClock();

    // Enable flash prefetch
    if (HAL_GetREVID() == 0x1001) {
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }
}

//-----------------------------------------------------------------------------
void ResetAndClockControl::InitializeOscillators() const {
    // The voltage scaling allows optimizing the power consumption when the device is
    // clocked below the maximum system frequency, to update the voltage scaling value
    // regarding system frequency refer to product datasheet.
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Initialize the HSE (High Speed External Oscillator) and initialize the PLLs
    // The board uses an 8MHz crystal

    // PLLN must be between 50 and 432
    // The VCO output frequency should be between 100MHz and 432MHz

    // PLLN = 336
    // PLLM = 8
    // fVCO = fPLLinput * PLLN/PLLM
    // fVCO = 8MHz * 336/8
    // fVCO = 336MHz

    // The maximum frequency for the system clock is 180MHz

    // PLLP = 2
    // fPLLoutput = fVCO / PLLP
    // fPLLoutput = 336MHz / 2
    // fPLLoutput = 168MHz

    // PLLQ = 7
    // fUSB_SDIO_RNG = fVCO / PLLQ
    // fUSB_SDIO_RNG = 336MHz / 7
    // fUSB_SDIO_RNG = 48MHz

    // This system doesn't use USB, SDIO, or RNG.  These clocks will
    // not be enabled.  However, should they need to be, the clock should
    // be 48 MHz to operate correctly

    RCC_OscInitTypeDef RCC_OscInitStruct;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
}

//-----------------------------------------------------------------------------
void ResetAndClockControl::InitializeBusClocks() const {
    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    // APB High Speed Prescaler (APB2)
    // APB2CLKDivider = 2
    // The frequency must not exceed 90MHz
    // APB2 = fPLLoutput / APB2CLKDivider
    // APB2 = 168MHz / 2
    // APB2 = 84MHz peripherals / 168 MHz timers (x2 multiplier on timers)

    // APB Low Speed Prescaler (APB1)
    // APB1CLKDivider = 4
    // The frequency must not exceed 45MHz
    // APB1 = fPLLoutput / APB1CLKDivider
    // APB1 = 168MHz / 4
    // APB1 = 42MHz peripherals / 84 MHz timers (x2 multiplier on timers)

    // AHB Prescaler
    // AHBCLKDivider = 1
    // The frequency must not exceed 180MHz
    // AHB = fPLLoutput / AHBCLKDivider
    // AHB = 168MHz / 1
    // AHB = 168MHz

    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    RCC_ClkInitStruct.ClockType =
                   (RCC_CLOCKTYPE_SYSCLK |
                    RCC_CLOCKTYPE_HCLK |
                    RCC_CLOCKTYPE_PCLK1 |
                    RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

    // Set the appropriate constants here, based on the above calculations.
    // We are doing this manually as we are not using a calculator function
    // or macro above, so ensure this matches the comments if it ever changes
    apb1_timer_frequency_hz_ = 42000000;  // 42 MHz
    apb2_timer_frequency_hz_ = 84000000;  // 84 MHz
}

//-----------------------------------------------------------------------------
void ResetAndClockControl::InitializeSysTick() const {
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

//-----------------------------------------------------------------------------
void ResetAndClockControl::EnablePeripheralClocks() const {
    __ADC1_CLK_ENABLE();
    __HAL_RCC_CRC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();  // PWM
    __HAL_RCC_GPIOB_CLK_ENABLE();  // contactor enable; I2C;
    __HAL_RCC_GPIOC_CLK_ENABLE();  // Motor Control En/Dis
    __HAL_RCC_GPIOD_CLK_ENABLE();  // Encoder
    __HAL_RCC_GPIOE_CLK_ENABLE();  // Capacitor Bank Enable; motor enable
    // __HAL_RCC_GPIOF_CLK_ENABLE();  // Buttons; watchdog
    // __HAL_RCC_GPIOG_CLK_ENABLE();  // Buttons; watchdog
    // __HAL_RCC_GPIOI_CLK_ENABLE();  // charger status line
    __HAL_RCC_I2C1_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();   // PWM
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();   // Encoder
    __HAL_RCC_TIM5_CLK_ENABLE();
    // __HAL_RCC_TIM8_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    // __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_USART6_CLK_ENABLE();
    __HAL_RCC_SPI3_CLK_ENABLE();
    __HAL_RCC_MCO1_CONFIG(RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}

//-----------------------------------------------------------------------------
void ResetAndClockControl::EnableRTCClock() {
    // After reset, the backup domain (RTC registers, RTC backup data
    // registers and backup SRAM) is protected against possible unwanted write
    // accesses.
    // To enable access to the RTC Domain and RTC registers, proceed as follows:
    // Enable the Power Controller (PWR) APB1 interface clock using the
    // __HAL_RCC_PWR_CLK_ENABLE() function.
    // Enable access to RTC domain using the HAL_PWR_EnableBkUpAccess() function.
    // Select the RTC clock source using the __HAL_RCC_RTC_CONFIG() function.
    //  (Low Speed Oscillator)
    // Enable RTC Clock using the __HAL_RCC_RTC_ENABLE() function.
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
    __HAL_RCC_RTC_ENABLE();
}

//-----------------------------------------------------------------------------
IResetAndClockControl::ResetSource ResetAndClockControl::GetResetSource() const {
    // Power on reset
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
        return ResetSource::PowerOnReset;

    // Brown out reset
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST))
        return ResetSource::BrownOutReset;

    // Independent watchdog reset
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
        return ResetSource::IndependentWatchdogReset;

    // NRST pin reset
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
        return ResetSource::PinReset;

    // Software reset
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
        return ResetSource::SoftwareReset;

    // Windowed watchdog reset
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
        return ResetSource::WindowedWatchdogReset;

    // Low power reset
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
        return ResetSource::LowPowerReset;

    return ResetSource::Unknown;
}

//-----------------------------------------------------------------------------
void ResetAndClockControl::ClearResetSource() const {
    __HAL_RCC_CLEAR_RESET_FLAGS();
}

//-----------------------------------------------------------------------------
uint32_t ResetAndClockControl::apb1_timer_frequency_hz_;
uint32_t ResetAndClockControl::apb2_timer_frequency_hz_;
bool ResetAndClockControl::initialized_;
