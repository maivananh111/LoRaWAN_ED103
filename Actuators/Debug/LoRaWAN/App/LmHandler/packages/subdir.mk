################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LoRaWAN/App/LmHandler/packages/FragDecoder.c \
../LoRaWAN/App/LmHandler/packages/LmhpClockSync.c \
../LoRaWAN/App/LmHandler/packages/LmhpCompliance.c \
../LoRaWAN/App/LmHandler/packages/LmhpFragmentation.c \
../LoRaWAN/App/LmHandler/packages/LmhpRemoteMcastSetup.c 

OBJS += \
./LoRaWAN/App/LmHandler/packages/FragDecoder.o \
./LoRaWAN/App/LmHandler/packages/LmhpClockSync.o \
./LoRaWAN/App/LmHandler/packages/LmhpCompliance.o \
./LoRaWAN/App/LmHandler/packages/LmhpFragmentation.o \
./LoRaWAN/App/LmHandler/packages/LmhpRemoteMcastSetup.o 

C_DEPS += \
./LoRaWAN/App/LmHandler/packages/FragDecoder.d \
./LoRaWAN/App/LmHandler/packages/LmhpClockSync.d \
./LoRaWAN/App/LmHandler/packages/LmhpCompliance.d \
./LoRaWAN/App/LmHandler/packages/LmhpFragmentation.d \
./LoRaWAN/App/LmHandler/packages/LmhpRemoteMcastSetup.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWAN/App/LmHandler/packages/%.o LoRaWAN/App/LmHandler/packages/%.su LoRaWAN/App/LmHandler/packages/%.cyclo: ../LoRaWAN/App/LmHandler/packages/%.c LoRaWAN/App/LmHandler/packages/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -DACTIVE_REGION=LORAMAC_REGION_AS923 -DSOFT_SE -DREGION_AS923 -c -I../Core/Inc -I../Util/logger -I../Util/mem -I../Util/timer -I../Util/nvmm -I../LoRaWAN/Porting -I../LoRaWAN/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy -I../Middlewares/Third_Party/LoRaWAN/Mac/region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../LoRaWAN/App/LmHandler -I../LoRaWAN/App/LmHandler/packages -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LoRaWAN-2f-App-2f-LmHandler-2f-packages

clean-LoRaWAN-2f-App-2f-LmHandler-2f-packages:
	-$(RM) ./LoRaWAN/App/LmHandler/packages/FragDecoder.cyclo ./LoRaWAN/App/LmHandler/packages/FragDecoder.d ./LoRaWAN/App/LmHandler/packages/FragDecoder.o ./LoRaWAN/App/LmHandler/packages/FragDecoder.su ./LoRaWAN/App/LmHandler/packages/LmhpClockSync.cyclo ./LoRaWAN/App/LmHandler/packages/LmhpClockSync.d ./LoRaWAN/App/LmHandler/packages/LmhpClockSync.o ./LoRaWAN/App/LmHandler/packages/LmhpClockSync.su ./LoRaWAN/App/LmHandler/packages/LmhpCompliance.cyclo ./LoRaWAN/App/LmHandler/packages/LmhpCompliance.d ./LoRaWAN/App/LmHandler/packages/LmhpCompliance.o ./LoRaWAN/App/LmHandler/packages/LmhpCompliance.su ./LoRaWAN/App/LmHandler/packages/LmhpFragmentation.cyclo ./LoRaWAN/App/LmHandler/packages/LmhpFragmentation.d ./LoRaWAN/App/LmHandler/packages/LmhpFragmentation.o ./LoRaWAN/App/LmHandler/packages/LmhpFragmentation.su ./LoRaWAN/App/LmHandler/packages/LmhpRemoteMcastSetup.cyclo ./LoRaWAN/App/LmHandler/packages/LmhpRemoteMcastSetup.d ./LoRaWAN/App/LmHandler/packages/LmhpRemoteMcastSetup.o ./LoRaWAN/App/LmHandler/packages/LmhpRemoteMcastSetup.su

.PHONY: clean-LoRaWAN-2f-App-2f-LmHandler-2f-packages

