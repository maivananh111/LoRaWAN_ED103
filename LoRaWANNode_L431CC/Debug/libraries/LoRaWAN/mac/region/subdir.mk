################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/LoRaWAN/mac/region/Region.c \
../libraries/LoRaWAN/mac/region/RegionAS923.c \
../libraries/LoRaWAN/mac/region/RegionAU915.c \
../libraries/LoRaWAN/mac/region/RegionCN470.c \
../libraries/LoRaWAN/mac/region/RegionCN779.c \
../libraries/LoRaWAN/mac/region/RegionCommon.c \
../libraries/LoRaWAN/mac/region/RegionEU433.c \
../libraries/LoRaWAN/mac/region/RegionEU868.c \
../libraries/LoRaWAN/mac/region/RegionIN865.c \
../libraries/LoRaWAN/mac/region/RegionKR920.c \
../libraries/LoRaWAN/mac/region/RegionRU864.c \
../libraries/LoRaWAN/mac/region/RegionUS915.c 

OBJS += \
./libraries/LoRaWAN/mac/region/Region.o \
./libraries/LoRaWAN/mac/region/RegionAS923.o \
./libraries/LoRaWAN/mac/region/RegionAU915.o \
./libraries/LoRaWAN/mac/region/RegionCN470.o \
./libraries/LoRaWAN/mac/region/RegionCN779.o \
./libraries/LoRaWAN/mac/region/RegionCommon.o \
./libraries/LoRaWAN/mac/region/RegionEU433.o \
./libraries/LoRaWAN/mac/region/RegionEU868.o \
./libraries/LoRaWAN/mac/region/RegionIN865.o \
./libraries/LoRaWAN/mac/region/RegionKR920.o \
./libraries/LoRaWAN/mac/region/RegionRU864.o \
./libraries/LoRaWAN/mac/region/RegionUS915.o 

C_DEPS += \
./libraries/LoRaWAN/mac/region/Region.d \
./libraries/LoRaWAN/mac/region/RegionAS923.d \
./libraries/LoRaWAN/mac/region/RegionAU915.d \
./libraries/LoRaWAN/mac/region/RegionCN470.d \
./libraries/LoRaWAN/mac/region/RegionCN779.d \
./libraries/LoRaWAN/mac/region/RegionCommon.d \
./libraries/LoRaWAN/mac/region/RegionEU433.d \
./libraries/LoRaWAN/mac/region/RegionEU868.d \
./libraries/LoRaWAN/mac/region/RegionIN865.d \
./libraries/LoRaWAN/mac/region/RegionKR920.d \
./libraries/LoRaWAN/mac/region/RegionRU864.d \
./libraries/LoRaWAN/mac/region/RegionUS915.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/LoRaWAN/mac/region/%.o libraries/LoRaWAN/mac/region/%.su libraries/LoRaWAN/mac/region/%.cyclo: ../libraries/LoRaWAN/mac/region/%.c libraries/LoRaWAN/mac/region/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../libraries -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libraries-2f-LoRaWAN-2f-mac-2f-region

clean-libraries-2f-LoRaWAN-2f-mac-2f-region:
	-$(RM) ./libraries/LoRaWAN/mac/region/Region.cyclo ./libraries/LoRaWAN/mac/region/Region.d ./libraries/LoRaWAN/mac/region/Region.o ./libraries/LoRaWAN/mac/region/Region.su ./libraries/LoRaWAN/mac/region/RegionAS923.cyclo ./libraries/LoRaWAN/mac/region/RegionAS923.d ./libraries/LoRaWAN/mac/region/RegionAS923.o ./libraries/LoRaWAN/mac/region/RegionAS923.su ./libraries/LoRaWAN/mac/region/RegionAU915.cyclo ./libraries/LoRaWAN/mac/region/RegionAU915.d ./libraries/LoRaWAN/mac/region/RegionAU915.o ./libraries/LoRaWAN/mac/region/RegionAU915.su ./libraries/LoRaWAN/mac/region/RegionCN470.cyclo ./libraries/LoRaWAN/mac/region/RegionCN470.d ./libraries/LoRaWAN/mac/region/RegionCN470.o ./libraries/LoRaWAN/mac/region/RegionCN470.su ./libraries/LoRaWAN/mac/region/RegionCN779.cyclo ./libraries/LoRaWAN/mac/region/RegionCN779.d ./libraries/LoRaWAN/mac/region/RegionCN779.o ./libraries/LoRaWAN/mac/region/RegionCN779.su ./libraries/LoRaWAN/mac/region/RegionCommon.cyclo ./libraries/LoRaWAN/mac/region/RegionCommon.d ./libraries/LoRaWAN/mac/region/RegionCommon.o ./libraries/LoRaWAN/mac/region/RegionCommon.su ./libraries/LoRaWAN/mac/region/RegionEU433.cyclo ./libraries/LoRaWAN/mac/region/RegionEU433.d ./libraries/LoRaWAN/mac/region/RegionEU433.o ./libraries/LoRaWAN/mac/region/RegionEU433.su ./libraries/LoRaWAN/mac/region/RegionEU868.cyclo ./libraries/LoRaWAN/mac/region/RegionEU868.d ./libraries/LoRaWAN/mac/region/RegionEU868.o ./libraries/LoRaWAN/mac/region/RegionEU868.su ./libraries/LoRaWAN/mac/region/RegionIN865.cyclo ./libraries/LoRaWAN/mac/region/RegionIN865.d ./libraries/LoRaWAN/mac/region/RegionIN865.o ./libraries/LoRaWAN/mac/region/RegionIN865.su ./libraries/LoRaWAN/mac/region/RegionKR920.cyclo ./libraries/LoRaWAN/mac/region/RegionKR920.d ./libraries/LoRaWAN/mac/region/RegionKR920.o ./libraries/LoRaWAN/mac/region/RegionKR920.su ./libraries/LoRaWAN/mac/region/RegionRU864.cyclo ./libraries/LoRaWAN/mac/region/RegionRU864.d ./libraries/LoRaWAN/mac/region/RegionRU864.o ./libraries/LoRaWAN/mac/region/RegionRU864.su ./libraries/LoRaWAN/mac/region/RegionUS915.cyclo ./libraries/LoRaWAN/mac/region/RegionUS915.d ./libraries/LoRaWAN/mac/region/RegionUS915.o ./libraries/LoRaWAN/mac/region/RegionUS915.su

.PHONY: clean-libraries-2f-LoRaWAN-2f-mac-2f-region

