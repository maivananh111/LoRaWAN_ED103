################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/LoRaWAN/crypto/aes.c \
../libraries/LoRaWAN/crypto/cmac.c 

OBJS += \
./libraries/LoRaWAN/crypto/aes.o \
./libraries/LoRaWAN/crypto/cmac.o 

C_DEPS += \
./libraries/LoRaWAN/crypto/aes.d \
./libraries/LoRaWAN/crypto/cmac.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/LoRaWAN/crypto/%.o libraries/LoRaWAN/crypto/%.su libraries/LoRaWAN/crypto/%.cyclo: ../libraries/LoRaWAN/crypto/%.c libraries/LoRaWAN/crypto/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../libraries -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libraries-2f-LoRaWAN-2f-crypto

clean-libraries-2f-LoRaWAN-2f-crypto:
	-$(RM) ./libraries/LoRaWAN/crypto/aes.cyclo ./libraries/LoRaWAN/crypto/aes.d ./libraries/LoRaWAN/crypto/aes.o ./libraries/LoRaWAN/crypto/aes.su ./libraries/LoRaWAN/crypto/cmac.cyclo ./libraries/LoRaWAN/crypto/cmac.d ./libraries/LoRaWAN/crypto/cmac.o ./libraries/LoRaWAN/crypto/cmac.su

.PHONY: clean-libraries-2f-LoRaWAN-2f-crypto

