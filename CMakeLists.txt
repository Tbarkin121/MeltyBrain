#*********************************************************************
# Copyright © Tyler Barkin – All Rights Reserved
# Proprietary and Confidential
# Copying of this file or use of code contained in this file via any medium is strictly
# prohibited unless prior written permission is obtained from Tyler Barkin, Inc.
# Created by Tyler Barkin, November 2017
#*********************************************************************/

cmake_minimum_required(VERSION 2.8)

enable_language(ASM)
project(MeltyBrain)


# Set the output folder where your program will be created
#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(TOOLS_PATH /opt/tools)
set(SCRIPTS_PATH ${CMAKE_SOURCE_DIR}/tools)

set(STM_LIB_SOURCES
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_crc.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_iwdg.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rtc.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rtc_ex.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/CMSIS/Device/ST/STM32F3xx/Source/Templates/system_stm32f3xx.c
        
        )

set(STM_STARTUP_SOURCES
        ${CMAKE_SOURCE_DIR}/src/startup_stm32f334x8.s
        )

set(FreeRTOS_LIB_SOURCES
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Middlewares/Third_Party/FreeRTOS/Source/list.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Middlewares/Third_Party/FreeRTOS/Source/queue.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Middlewares/Third_Party/FreeRTOS/Source/timers.c
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
        )

set(PROJECT_SOURCES
        # ${CMAKE_SOURCE_DIR}/src/system.cc
        ${CMAKE_SOURCE_DIR}/src/freertos_tasks.c
        )

set(PROJECT_LOW_LEVEL_SOURCES

        ${CMAKE_SOURCE_DIR}/src/stm32f3xx_it.c
        ${CMAKE_SOURCE_DIR}/src/gpio.cc
        ${CMAKE_SOURCE_DIR}/src/reset_and_clock_control.cc
        # ${CMAKE_SOURCE_DIR}/src/rtos_task.cc
        # ${CMAKE_SOURCE_DIR}/src/rtos_queue.cc
        
        )

set(MAIN_SOURCES
        ${CMAKE_SOURCE_DIR}/src/main.cc
        ${CMAKE_SOURCE_DIR}/src/stm32f3xx_it.c
        )

set(PROJECT_LIB_SOURCES
        ${CMAKE_SOURCE_DIR}/src/hardware.cc
        )

include_directories(
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/STM32F3xx_HAL_Driver/Inc
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/CMSIS/Device/ST/STM32F3xx/Include
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Drivers/CMSIS/Include

        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Middlewares/Third_Party
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Middlewares/Third_Party/FreeRTOS/Source/include
        ${TOOLS_PATH}/STM32Cube_FW_F3_V1.9.0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F

        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}

)



set(CMAKE_C_COMPILER ${TOOLS_PATH}/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER ${TOOLS_PATH}/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-as)
set(GCOV ${TOOLS_PATH}/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-gcov)
# Processor flags
set(GCC_PROCESSOR_FLAGS "-mcpu=cortex-m4 -mthumb")
#  -mcpu=cortex-m4                      Processor type is ARM Cortex M4
#  -mthumb                              Generate code using Thumb mode

# Dialect flags
set(GCC_DIALECT_FLAGS "-fmessage-length=0 -fsigned-char")
#  -fmessage-length=0                   Each error message is not line wrapped
#  -fsigned-char                        'char' type is signed by default

# C Standard and Warnings
set(GCC_C_FLAGS "-std=gnu11")
#  -std=gnu11                           Allow the gnu extensions with C11

# C++ Software Version macro
    if(DEFINED ENV{BUILD_NUMBER})
       set(SW_VERSION_MACRO "--sw_version=$ENV{BUILD_NUMBER}")
    endif()

# C++ Option flags
set(GCC_CPP_FLAGS "-fabi-version=0 -fno-exceptions -fno-use-cxa-atexit -fno-threadsafe-statics -std=gnu++11")
#  -fabi-version=0                      Use ABI version 0
#  -fno-exceptions                      Don't use C++ exception handlers
#  -fno-use-cxa-atexit                  Register destructors with atexit function
#  -fno-threadsafe-statics              Don't use extra code to make static initialization threadsafe
#  -fstack-check                        Enable stack checking runtime
#  -std=gnu++11                         Allow the gnu extensions with C++11

# Optimization flags
set(GCC_OPTIMIZATION_FLAGS "-ffunction-sections -fdata-sections -fno-move-loop-invariants -Og")
#  -ffunction-sections                  Put each function into own section (for linker optimization)
#  -fdata-sections                      Put each data item into own section (for linker optimization)
#  -Og                                  Enable optimizations that do not interfere with debugging
#  -fno-move-loop-invariants            Required for GCC prior to 4.9 with -Og to make debugging stepping easier to follow

# Warnings flags
set(GCC_WARNING_FLAGS "-Wall -Wextra -Werror -Wno-unused-parameter")
#  -Wall                                Enable warnings about questionable constructions
#  -Wextra                              Enable more warnings not covered by Wall
#  -Werror                              Make all warnings into errors
#  -Wno-unused-parameter                Ignore unused parameter warnings

# Debug flags
set(GCC_DEBUG_FLAGS "-g3")
#  -g3                                  Produces debugging information

# STM32F Specific definitions
set(STM_SPECIFIC_DEFINES "-DSTM32F334x8 -DUSE_HAL_DRIVER -DHSE_VALUE=8000000")
#  -DSTM32F334x8                        Defines the processor to be STM32F334x8
#  -DUSE_HAL_DRIVER                     Use the provided hardware abstraction layer drivers instead of direct register access
#  -DHSE_VALUE=8000000                  Set crystal frequency to 8MHz

# Boost specific definitions
set(BOOST_SPECIFIC_DEFINES "-DBOOST_DISABLE_ASSERTS")
#  -DBOOST_DISABLE_ASSERTS              BOOST_ASSERT() calls are pre-processed out

# STM32F Specific definitions for debug
set(STM_SPECIFIC_DEBUG_DEFINES "-DDEBUG -DTRACE -DUSE_FULL_ASSERT -DOS_USE_SEMIHOSTING -DOS_USE_TRACE_SEMIHOSTING_DEBUG")
#  -DDEBUG                              Defines the DEBUG symbol
#  -DTRACE                              Use the trace channel defined in diag/trace.h
#  -DUSE_FULL_ASSERT                    Causes ST assert macros to be enabled
#  -DOS_USE_SEMIHOSTING                 Enable semihosting for communication
#  -DOS_USE_TRACE_SEMIHOSTING_DEBUG     Use semihosting for the trace channel

# http://stm32f4-discovery.com/2015/01/properly-set-clock-speed-stm32f4xx-devices/
# https://feabhas.gitbooks.io/stm32f4-cmsis/content/compiler_flags.html

# Linker flags -nostartfiles
set(GCC_LINK_FLAGS "-u _printf_float -Xlinker --gc-sections -L${CMAKE_SOURCE_DIR}/ldscripts --specs=nano.specs --specs=nosys.specs")
#  -u _printf_float                     Linker flag for flaoting point printf and sprintf
#  -nostartfiles                        Don't use system startup files
#  -Xlinker                             Options to follow:
#  --gc-sections                        Optimize the sections; needs to be used with ffunction-sections and fdata-section
#  -L${CMAKE_SOURCE_DIR}/ldscripts      Location of the linker scripts
#  --specs=nano.specs                   Use the newlib-nano derivative
#  --specs=nosys.specs                  Use libnosys, stubs for file i/o and thread fns


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_PROCESSOR_FLAGS} ${GCC_DIALECT_FLAGS} ${GCC_OPTIMIZATION_FLAGS} ${GCC_C_FLAGS} ${GCC_WARNING_FLAGS} ${GCC_DEBUG_FLAGS} ${STM_SPECIFIC_DEFINES}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_PROCESSOR_FLAGS} ${GCC_DIALECT_FLAGS} ${GCC_OPTIMIZATION_FLAGS} ${GCC_WARNING_FLAGS} ${GCC_DEBUG_FLAGS} ${STM_SPECIFIC_DEFINES} ${GCC_CPP_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_PROCESSOR_FLAGS} ${GCC_DIALECT_FLAGS} ${GCC_OPTIMIZATION_FLAGS} ${GCC_WARNING_FLAGS} ${GCC_DEBUG_FLAGS} ${GCC_LINK_FLAGS}")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

set(GCC_TARGET_LINK_SCRIPTS "-T STM32F334K6Tx_FLASH.ld")
set(GCC_TARGET_FREERTOS_LINK_SCRIPTS "-T STM32F334K6Tx_FLASH.ld")
# For main build
add_library(STM ${STM_LIB_SOURCES})
add_library(STARTUP ${STM_STARTUP_SOURCES})
add_library(PROJECT ${PROJECT_LOW_LEVEL_SOURCES} ${PROJECT_LIB_SOURCES})
add_library(FreeRTOS ${FreeRTOS_LIB_SOURCES})

set_target_properties(FreeRTOS PROPERTIES COMPILE_FLAGS "-mfpu=fpv4-sp-d16 -mfloat-abi=hard -DxPortSysTickHandler=SysTick_Handler")
set_target_properties(PROJECT PROPERTIES COMPILE_FLAGS "${BOOST_SPECIFIC_DEFINES}")


# Application builds
set(MAIN_BUILD ${CMAKE_PROJECT_NAME})
add_executable(${MAIN_BUILD} ${PROJECT_SOURCES} ${MAIN_SOURCES})
target_link_libraries(${MAIN_BUILD} STARTUP)
target_link_libraries(${MAIN_BUILD} FreeRTOS)
target_link_libraries(${MAIN_BUILD} PROJECT)
target_link_libraries(${MAIN_BUILD} STM)
#set_target_properties(${MAIN_BUILD} PROPERTIES LINK_FLAGS "${GCC_TARGET_LINK_SCRIPTS} -fprofile-arcs -Wl,-Map,${MAIN_BUILD}.map")
set_target_properties(${MAIN_BUILD} PROPERTIES LINK_FLAGS "${GCC_TARGET_FREERTOS_LINK_SCRIPTS} -fprofile-arcs -Wl,-Map,${MAIN_BUILD}.map")
set_target_properties(${MAIN_BUILD} PROPERTIES COMPILE_FLAGS "-mfpu=fpv4-sp-d16")
set(OPENOCD_COMMAND openocd)
add_custom_command(
    OUTPUT ${OPENOCD_COMMAND}
    COMMAND ${SCRIPTS_PATH}/loadsw.sh -b ${PROJECT_BINARY_DIR}/${MAIN_BUILD}
    DEPENDS ${MAIN_BUILD}
    COMMENT "Running application"
    )
set_source_files_properties(${OPENOCD_COMMAND} SYMBOLIC 1)
add_custom_target(app DEPENDS ${OPENOCD_COMMAND})


