################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LoRaWAN/Crypto/aes.c \
../LoRaWAN/Crypto/cmac.c \
../LoRaWAN/Crypto/soft-se.c 

OBJS += \
./LoRaWAN/Crypto/aes.o \
./LoRaWAN/Crypto/cmac.o \
./LoRaWAN/Crypto/soft-se.o 

C_DEPS += \
./LoRaWAN/Crypto/aes.d \
./LoRaWAN/Crypto/cmac.d \
./LoRaWAN/Crypto/soft-se.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWAN/Crypto/%.o LoRaWAN/Crypto/%.su LoRaWAN/Crypto/%.cyclo: ../LoRaWAN/Crypto/%.c LoRaWAN/Crypto/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../LoRaWAN -I../LoRaWAN/LmHandler/packages -I../LoRaWAN/LmHandler -I../LoRaWAN/Crypto -I../LoRaWAN/Mac -I../LoRaWAN/Mac/Region -I../LoRaWAN/Radio -I../LoRaWAN/Utilities/timer -I../LoRaWAN/Utilities/mem -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LoRaWAN-2f-Crypto

clean-LoRaWAN-2f-Crypto:
	-$(RM) ./LoRaWAN/Crypto/aes.cyclo ./LoRaWAN/Crypto/aes.d ./LoRaWAN/Crypto/aes.o ./LoRaWAN/Crypto/aes.su ./LoRaWAN/Crypto/cmac.cyclo ./LoRaWAN/Crypto/cmac.d ./LoRaWAN/Crypto/cmac.o ./LoRaWAN/Crypto/cmac.su ./LoRaWAN/Crypto/soft-se.cyclo ./LoRaWAN/Crypto/soft-se.d ./LoRaWAN/Crypto/soft-se.o ./LoRaWAN/Crypto/soft-se.su

.PHONY: clean-LoRaWAN-2f-Crypto

