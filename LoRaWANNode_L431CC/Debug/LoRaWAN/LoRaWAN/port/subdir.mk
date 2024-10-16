################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LoRaWAN/LoRaWAN/port/port.c \
../LoRaWAN/LoRaWAN/port/timer.c \
../LoRaWAN/LoRaWAN/port/timertick.c 

OBJS += \
./LoRaWAN/LoRaWAN/port/port.o \
./LoRaWAN/LoRaWAN/port/timer.o \
./LoRaWAN/LoRaWAN/port/timertick.o 

C_DEPS += \
./LoRaWAN/LoRaWAN/port/port.d \
./LoRaWAN/LoRaWAN/port/timer.d \
./LoRaWAN/LoRaWAN/port/timertick.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWAN/LoRaWAN/port/%.o LoRaWAN/LoRaWAN/port/%.su LoRaWAN/LoRaWAN/port/%.cyclo: ../LoRaWAN/LoRaWAN/port/%.c LoRaWAN/LoRaWAN/port/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../TraceLog -I../LoRaWAN -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LoRaWAN-2f-LoRaWAN-2f-port

clean-LoRaWAN-2f-LoRaWAN-2f-port:
	-$(RM) ./LoRaWAN/LoRaWAN/port/port.cyclo ./LoRaWAN/LoRaWAN/port/port.d ./LoRaWAN/LoRaWAN/port/port.o ./LoRaWAN/LoRaWAN/port/port.su ./LoRaWAN/LoRaWAN/port/timer.cyclo ./LoRaWAN/LoRaWAN/port/timer.d ./LoRaWAN/LoRaWAN/port/timer.o ./LoRaWAN/LoRaWAN/port/timer.su ./LoRaWAN/LoRaWAN/port/timertick.cyclo ./LoRaWAN/LoRaWAN/port/timertick.d ./LoRaWAN/LoRaWAN/port/timertick.o ./LoRaWAN/LoRaWAN/port/timertick.su

.PHONY: clean-LoRaWAN-2f-LoRaWAN-2f-port

