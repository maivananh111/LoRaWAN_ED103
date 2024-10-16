################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LoRaWAN/Mac/LoRaMac.c \
../LoRaWAN/Mac/LoRaMacAdr.c \
../LoRaWAN/Mac/LoRaMacClassB.c \
../LoRaWAN/Mac/LoRaMacCommands.c \
../LoRaWAN/Mac/LoRaMacConfirmQueue.c \
../LoRaWAN/Mac/LoRaMacCrypto.c \
../LoRaWAN/Mac/LoRaMacParser.c \
../LoRaWAN/Mac/LoRaMacSerializer.c 

OBJS += \
./LoRaWAN/Mac/LoRaMac.o \
./LoRaWAN/Mac/LoRaMacAdr.o \
./LoRaWAN/Mac/LoRaMacClassB.o \
./LoRaWAN/Mac/LoRaMacCommands.o \
./LoRaWAN/Mac/LoRaMacConfirmQueue.o \
./LoRaWAN/Mac/LoRaMacCrypto.o \
./LoRaWAN/Mac/LoRaMacParser.o \
./LoRaWAN/Mac/LoRaMacSerializer.o 

C_DEPS += \
./LoRaWAN/Mac/LoRaMac.d \
./LoRaWAN/Mac/LoRaMacAdr.d \
./LoRaWAN/Mac/LoRaMacClassB.d \
./LoRaWAN/Mac/LoRaMacCommands.d \
./LoRaWAN/Mac/LoRaMacConfirmQueue.d \
./LoRaWAN/Mac/LoRaMacCrypto.d \
./LoRaWAN/Mac/LoRaMacParser.d \
./LoRaWAN/Mac/LoRaMacSerializer.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWAN/Mac/%.o LoRaWAN/Mac/%.su LoRaWAN/Mac/%.cyclo: ../LoRaWAN/Mac/%.c LoRaWAN/Mac/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../LoRaWAN -I../LoRaWAN/LmHandler/packages -I../LoRaWAN/LmHandler -I../LoRaWAN/Crypto -I../LoRaWAN/Mac -I../LoRaWAN/Mac/Region -I../LoRaWAN/Radio -I../LoRaWAN/Utilities/timer -I../LoRaWAN/Utilities/mem -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LoRaWAN-2f-Mac

clean-LoRaWAN-2f-Mac:
	-$(RM) ./LoRaWAN/Mac/LoRaMac.cyclo ./LoRaWAN/Mac/LoRaMac.d ./LoRaWAN/Mac/LoRaMac.o ./LoRaWAN/Mac/LoRaMac.su ./LoRaWAN/Mac/LoRaMacAdr.cyclo ./LoRaWAN/Mac/LoRaMacAdr.d ./LoRaWAN/Mac/LoRaMacAdr.o ./LoRaWAN/Mac/LoRaMacAdr.su ./LoRaWAN/Mac/LoRaMacClassB.cyclo ./LoRaWAN/Mac/LoRaMacClassB.d ./LoRaWAN/Mac/LoRaMacClassB.o ./LoRaWAN/Mac/LoRaMacClassB.su ./LoRaWAN/Mac/LoRaMacCommands.cyclo ./LoRaWAN/Mac/LoRaMacCommands.d ./LoRaWAN/Mac/LoRaMacCommands.o ./LoRaWAN/Mac/LoRaMacCommands.su ./LoRaWAN/Mac/LoRaMacConfirmQueue.cyclo ./LoRaWAN/Mac/LoRaMacConfirmQueue.d ./LoRaWAN/Mac/LoRaMacConfirmQueue.o ./LoRaWAN/Mac/LoRaMacConfirmQueue.su ./LoRaWAN/Mac/LoRaMacCrypto.cyclo ./LoRaWAN/Mac/LoRaMacCrypto.d ./LoRaWAN/Mac/LoRaMacCrypto.o ./LoRaWAN/Mac/LoRaMacCrypto.su ./LoRaWAN/Mac/LoRaMacParser.cyclo ./LoRaWAN/Mac/LoRaMacParser.d ./LoRaWAN/Mac/LoRaMacParser.o ./LoRaWAN/Mac/LoRaMacParser.su ./LoRaWAN/Mac/LoRaMacSerializer.cyclo ./LoRaWAN/Mac/LoRaMacSerializer.d ./LoRaWAN/Mac/LoRaMacSerializer.o ./LoRaWAN/Mac/LoRaMacSerializer.su

.PHONY: clean-LoRaWAN-2f-Mac

