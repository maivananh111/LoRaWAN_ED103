################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/LoRaWAN/phy/radio.c \
../libraries/LoRaWAN/phy/sx126x.c \
../libraries/LoRaWAN/phy/sx126x_bsp.c 

OBJS += \
./libraries/LoRaWAN/phy/radio.o \
./libraries/LoRaWAN/phy/sx126x.o \
./libraries/LoRaWAN/phy/sx126x_bsp.o 

C_DEPS += \
./libraries/LoRaWAN/phy/radio.d \
./libraries/LoRaWAN/phy/sx126x.d \
./libraries/LoRaWAN/phy/sx126x_bsp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/LoRaWAN/phy/%.o libraries/LoRaWAN/phy/%.su libraries/LoRaWAN/phy/%.cyclo: ../libraries/LoRaWAN/phy/%.c libraries/LoRaWAN/phy/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../libraries -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libraries-2f-LoRaWAN-2f-phy

clean-libraries-2f-LoRaWAN-2f-phy:
	-$(RM) ./libraries/LoRaWAN/phy/radio.cyclo ./libraries/LoRaWAN/phy/radio.d ./libraries/LoRaWAN/phy/radio.o ./libraries/LoRaWAN/phy/radio.su ./libraries/LoRaWAN/phy/sx126x.cyclo ./libraries/LoRaWAN/phy/sx126x.d ./libraries/LoRaWAN/phy/sx126x.o ./libraries/LoRaWAN/phy/sx126x.su ./libraries/LoRaWAN/phy/sx126x_bsp.cyclo ./libraries/LoRaWAN/phy/sx126x_bsp.d ./libraries/LoRaWAN/phy/sx126x_bsp.o ./libraries/LoRaWAN/phy/sx126x_bsp.su

.PHONY: clean-libraries-2f-LoRaWAN-2f-phy

