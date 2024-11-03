################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LoRaWAN/LmHandler/packages/FragDecoder.c \
../Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpClockSync.c \
../Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpCompliance.c \
../Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpFragmentation.c \
../Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.c 

OBJS += \
./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/FragDecoder.o \
./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpClockSync.o \
./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpCompliance.o \
./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpFragmentation.o \
./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.o 

C_DEPS += \
./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/FragDecoder.d \
./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpClockSync.d \
./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpCompliance.d \
./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpFragmentation.d \
./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LoRaWAN/LmHandler/packages/%.o Middlewares/Third_Party/LoRaWAN/LmHandler/packages/%.su Middlewares/Third_Party/LoRaWAN/LmHandler/packages/%.cyclo: ../Middlewares/Third_Party/LoRaWAN/LmHandler/packages/%.c Middlewares/Third_Party/LoRaWAN/LmHandler/packages/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -DACTIVE_REGION=LORAMAC_REGION_AS923 -DSOFT_SE -DREGION_AS923 -c -I../Core/Inc -I../Util/logger -I../Util/mem -I../Util/timer -I../Util/nvmm -I../LoRaWAN/Porting -I../LoRaWAN/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy -I../Middlewares/Third_Party/LoRaWAN/Mac/region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/LmHandler/packages -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../Middlewares/Third_Party/LoRaWAN/LmHandler -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-LoRaWAN-2f-LmHandler-2f-packages

clean-Middlewares-2f-Third_Party-2f-LoRaWAN-2f-LmHandler-2f-packages:
	-$(RM) ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/FragDecoder.cyclo ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/FragDecoder.d ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/FragDecoder.o ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/FragDecoder.su ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpClockSync.cyclo ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpClockSync.d ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpClockSync.o ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpClockSync.su ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpCompliance.cyclo ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpCompliance.d ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpCompliance.o ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpCompliance.su ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpFragmentation.cyclo ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpFragmentation.d ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpFragmentation.o ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpFragmentation.su ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.cyclo ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.d ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.o ./Middlewares/Third_Party/LoRaWAN/LmHandler/packages/LmhpRemoteMcastSetup.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-LoRaWAN-2f-LmHandler-2f-packages

