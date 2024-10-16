################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LoRaWAN/Utilities/timer/stm32_systime.c \
../LoRaWAN/Utilities/timer/stm32_timer.c \
../LoRaWAN/Utilities/timer/timer_if.c 

OBJS += \
./LoRaWAN/Utilities/timer/stm32_systime.o \
./LoRaWAN/Utilities/timer/stm32_timer.o \
./LoRaWAN/Utilities/timer/timer_if.o 

C_DEPS += \
./LoRaWAN/Utilities/timer/stm32_systime.d \
./LoRaWAN/Utilities/timer/stm32_timer.d \
./LoRaWAN/Utilities/timer/timer_if.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWAN/Utilities/timer/%.o LoRaWAN/Utilities/timer/%.su LoRaWAN/Utilities/timer/%.cyclo: ../LoRaWAN/Utilities/timer/%.c LoRaWAN/Utilities/timer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../LoRaWAN -I../LoRaWAN/LmHandler/packages -I../LoRaWAN/LmHandler -I../LoRaWAN/Crypto -I../LoRaWAN/Mac -I../LoRaWAN/Mac/Region -I../LoRaWAN/Radio -I../LoRaWAN/Utilities/timer -I../LoRaWAN/Utilities/mem -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LoRaWAN-2f-Utilities-2f-timer

clean-LoRaWAN-2f-Utilities-2f-timer:
	-$(RM) ./LoRaWAN/Utilities/timer/stm32_systime.cyclo ./LoRaWAN/Utilities/timer/stm32_systime.d ./LoRaWAN/Utilities/timer/stm32_systime.o ./LoRaWAN/Utilities/timer/stm32_systime.su ./LoRaWAN/Utilities/timer/stm32_timer.cyclo ./LoRaWAN/Utilities/timer/stm32_timer.d ./LoRaWAN/Utilities/timer/stm32_timer.o ./LoRaWAN/Utilities/timer/stm32_timer.su ./LoRaWAN/Utilities/timer/timer_if.cyclo ./LoRaWAN/Utilities/timer/timer_if.d ./LoRaWAN/Utilities/timer/timer_if.o ./LoRaWAN/Utilities/timer/timer_if.su

.PHONY: clean-LoRaWAN-2f-Utilities-2f-timer

