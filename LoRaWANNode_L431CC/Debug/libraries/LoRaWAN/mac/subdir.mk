################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/LoRaWAN/mac/LoRaMac.c \
../libraries/LoRaWAN/mac/LoRaMacCrypto.c \
../libraries/LoRaWAN/mac/LoRaMacHelper.c 

OBJS += \
./libraries/LoRaWAN/mac/LoRaMac.o \
./libraries/LoRaWAN/mac/LoRaMacCrypto.o \
./libraries/LoRaWAN/mac/LoRaMacHelper.o 

C_DEPS += \
./libraries/LoRaWAN/mac/LoRaMac.d \
./libraries/LoRaWAN/mac/LoRaMacCrypto.d \
./libraries/LoRaWAN/mac/LoRaMacHelper.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/LoRaWAN/mac/%.o libraries/LoRaWAN/mac/%.su libraries/LoRaWAN/mac/%.cyclo: ../libraries/LoRaWAN/mac/%.c libraries/LoRaWAN/mac/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../libraries -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libraries-2f-LoRaWAN-2f-mac

clean-libraries-2f-LoRaWAN-2f-mac:
	-$(RM) ./libraries/LoRaWAN/mac/LoRaMac.cyclo ./libraries/LoRaWAN/mac/LoRaMac.d ./libraries/LoRaWAN/mac/LoRaMac.o ./libraries/LoRaWAN/mac/LoRaMac.su ./libraries/LoRaWAN/mac/LoRaMacCrypto.cyclo ./libraries/LoRaWAN/mac/LoRaMacCrypto.d ./libraries/LoRaWAN/mac/LoRaMacCrypto.o ./libraries/LoRaWAN/mac/LoRaMacCrypto.su ./libraries/LoRaWAN/mac/LoRaMacHelper.cyclo ./libraries/LoRaWAN/mac/LoRaMacHelper.d ./libraries/LoRaWAN/mac/LoRaMacHelper.o ./libraries/LoRaWAN/mac/LoRaMacHelper.su

.PHONY: clean-libraries-2f-LoRaWAN-2f-mac

