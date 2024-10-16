################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Util/timer/stm32_systime.c \
../Util/timer/stm32_timer.c \
../Util/timer/timer_if.c 

OBJS += \
./Util/timer/stm32_systime.o \
./Util/timer/stm32_timer.o \
./Util/timer/timer_if.o 

C_DEPS += \
./Util/timer/stm32_systime.d \
./Util/timer/stm32_timer.d \
./Util/timer/timer_if.d 


# Each subdirectory must supply rules for building sources it contributes
Util/timer/%.o Util/timer/%.su Util/timer/%.cyclo: ../Util/timer/%.c Util/timer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LoRaWAN/App -I../LoRaWAN/Target -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../Middlewares/Third_Party/LoRaWAN/LmHandler -I../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy -I../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages -I../Util/logger -I../Util/mem -I../Util/timer -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Util-2f-timer

clean-Util-2f-timer:
	-$(RM) ./Util/timer/stm32_systime.cyclo ./Util/timer/stm32_systime.d ./Util/timer/stm32_systime.o ./Util/timer/stm32_systime.su ./Util/timer/stm32_timer.cyclo ./Util/timer/stm32_timer.d ./Util/timer/stm32_timer.o ./Util/timer/stm32_timer.su ./Util/timer/timer_if.cyclo ./Util/timer/timer_if.d ./Util/timer/timer_if.o ./Util/timer/timer_if.su

.PHONY: clean-Util-2f-timer

