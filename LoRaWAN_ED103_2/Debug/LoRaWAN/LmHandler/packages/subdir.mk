################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LoRaWAN/LmHandler/packages/FragDecoder.c \
../LoRaWAN/LmHandler/packages/LmhpClockSync.c \
../LoRaWAN/LmHandler/packages/LmhpCompliance.c \
../LoRaWAN/LmHandler/packages/LmhpFragmentation.c \
../LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.c 

OBJS += \
./LoRaWAN/LmHandler/packages/FragDecoder.o \
./LoRaWAN/LmHandler/packages/LmhpClockSync.o \
./LoRaWAN/LmHandler/packages/LmhpCompliance.o \
./LoRaWAN/LmHandler/packages/LmhpFragmentation.o \
./LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.o 

C_DEPS += \
./LoRaWAN/LmHandler/packages/FragDecoder.d \
./LoRaWAN/LmHandler/packages/LmhpClockSync.d \
./LoRaWAN/LmHandler/packages/LmhpCompliance.d \
./LoRaWAN/LmHandler/packages/LmhpFragmentation.d \
./LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWAN/LmHandler/packages/%.o LoRaWAN/LmHandler/packages/%.su LoRaWAN/LmHandler/packages/%.cyclo: ../LoRaWAN/LmHandler/packages/%.c LoRaWAN/LmHandler/packages/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../LoRaWAN -I../LoRaWAN/LmHandler/packages -I../LoRaWAN/LmHandler -I../LoRaWAN/Crypto -I../LoRaWAN/Mac -I../LoRaWAN/Mac/Region -I../LoRaWAN/Radio -I../LoRaWAN/Utilities/timer -I../LoRaWAN/Utilities/mem -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LoRaWAN-2f-LmHandler-2f-packages

clean-LoRaWAN-2f-LmHandler-2f-packages:
	-$(RM) ./LoRaWAN/LmHandler/packages/FragDecoder.cyclo ./LoRaWAN/LmHandler/packages/FragDecoder.d ./LoRaWAN/LmHandler/packages/FragDecoder.o ./LoRaWAN/LmHandler/packages/FragDecoder.su ./LoRaWAN/LmHandler/packages/LmhpClockSync.cyclo ./LoRaWAN/LmHandler/packages/LmhpClockSync.d ./LoRaWAN/LmHandler/packages/LmhpClockSync.o ./LoRaWAN/LmHandler/packages/LmhpClockSync.su ./LoRaWAN/LmHandler/packages/LmhpCompliance.cyclo ./LoRaWAN/LmHandler/packages/LmhpCompliance.d ./LoRaWAN/LmHandler/packages/LmhpCompliance.o ./LoRaWAN/LmHandler/packages/LmhpCompliance.su ./LoRaWAN/LmHandler/packages/LmhpFragmentation.cyclo ./LoRaWAN/LmHandler/packages/LmhpFragmentation.d ./LoRaWAN/LmHandler/packages/LmhpFragmentation.o ./LoRaWAN/LmHandler/packages/LmhpFragmentation.su ./LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.cyclo ./LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.d ./LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.o ./LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.su

.PHONY: clean-LoRaWAN-2f-LmHandler-2f-packages

