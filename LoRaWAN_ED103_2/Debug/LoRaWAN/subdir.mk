################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LoRaWAN/utilities.c 

OBJS += \
./LoRaWAN/utilities.o 

C_DEPS += \
./LoRaWAN/utilities.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWAN/%.o LoRaWAN/%.su LoRaWAN/%.cyclo: ../LoRaWAN/%.c LoRaWAN/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../LoRaWAN -I../LoRaWAN/LmHandler/packages -I../LoRaWAN/LmHandler -I../LoRaWAN/Crypto -I../LoRaWAN/Mac -I../LoRaWAN/Mac/Region -I../LoRaWAN/Radio -I../LoRaWAN/Utilities/timer -I../LoRaWAN/Utilities/mem -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LoRaWAN

clean-LoRaWAN:
	-$(RM) ./LoRaWAN/utilities.cyclo ./LoRaWAN/utilities.d ./LoRaWAN/utilities.o ./LoRaWAN/utilities.su

.PHONY: clean-LoRaWAN

