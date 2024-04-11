cmake_minimum_required(VERSION 3.22...3.28)
project(STM32F446RE_CubeMX_CMake_Demo VERSION 1.0 LANGUAGES ASM C CXX)

# Use the arm-none-eabi toolchain
# set(CMAKE_TOOLCHAIN_FILE arm-none-eabi.cmake)
include(arm-none-eabi.cmake)

# Set CMake C/C++/ASM standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Will not generate compile_commands otherwise

set(STM32_CUBEMX_GENERATED_FILES
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c
  # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c

  Inc/main.hpp
  # Inc/stm32f4xx_hal_conf.h
  # Inc/stm32f4xx_it.h

  Src/main.cpp
  # Src/stm32f4xx_hal_msp.c
  # Src/stm32f4xx_it.c
  Src/system_stm32f4xx.c

  startup_stm32f446xx.s
)

set(EXECUTABLE ${PROJECT_NAME}.o)
add_executable(${EXECUTABLE} ${STM32_CUBEMX_GENERATED_FILES})
target_compile_definitions(${EXECUTABLE} PRIVATE
  # -DUSE_HAL_DRIVER
  -DSTM32F446xx
)
target_include_directories(${EXECUTABLE} PRIVATE
  Inc
  # Drivers/STM32F4xx_HAL_Driver/Inc
  Drivers/CMSIS/Device/ST/STM32F4xx/Include
  Drivers/CMSIS/Include
)
target_compile_options(${EXECUTABLE} PRIVATE
  -mcpu=cortex-m4
  -mthumb
  -mfpu=fpv4-sp-d16
  -mfloat-abi=hard

  -fdata-sections
  -ffunction-sections

  -Wall
  -Wextra
  -pedantic
)

target_link_options(${EXECUTABLE} PRIVATE
  -T${CMAKE_SOURCE_DIR}/STM32F446RETx_FLASH.ld
  -mcpu=cortex-m4
  -mthumb
  -mfpu=fpv4-sp-d16
  -mfloat-abi=hard
  -specs=nano.specs
  -lc
  -lm
  -lnosys
  -Wl,-Map=${PROJECT_NAME}.map,--cref
  -Wl,--gc-sections
  -Wl,--print-memory-usage
)

add_custom_command(TARGET ${EXECUTABLE}
  POST_BUILD
  COMMAND arm-none-eabi-size ${EXECUTABLE}
)

add_custom_command(TARGET ${EXECUTABLE}
  POST_BUILD
  COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
  COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
)

add_custom_target(flash DEPENDS ${PROJECT_NAME}.bin)
add_custom_command(TARGET flash
  USES_TERMINAL
  COMMAND st-flash --reset write ${PROJECT_NAME}.bin 0x8000000
)

add_custom_target(debug DEPENDS ${PROJECT_NAME}.bin
  COMMAND arm-none-eabi-gdb -tui -command ${CMAKE_SOURCE_DIR}/remote.gdbconf ${PROJECT_NAME}.o 
)

add_custom_target(debug-server DEPENDS ${PROJECT_NAME}.bin)
add_custom_command(TARGET debug-server
  # COMMAND st-util
  COMMAND gnome-terminal -- st-util
)

add_custom_target(erase)
add_custom_command(TARGET erase
  COMMAND st-flash erase
)
