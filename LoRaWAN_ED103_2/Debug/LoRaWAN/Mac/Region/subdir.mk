################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LoRaWAN/Mac/Region/Region.c \
../LoRaWAN/Mac/Region/RegionAS923.c \
../LoRaWAN/Mac/Region/RegionAU915.c \
../LoRaWAN/Mac/Region/RegionCN470.c \
../LoRaWAN/Mac/Region/RegionCN779.c \
../LoRaWAN/Mac/Region/RegionCommon.c \
../LoRaWAN/Mac/Region/RegionEU433.c \
../LoRaWAN/Mac/Region/RegionEU868.c \
../LoRaWAN/Mac/Region/RegionIN865.c \
../LoRaWAN/Mac/Region/RegionKR920.c \
../LoRaWAN/Mac/Region/RegionRU864.c \
../LoRaWAN/Mac/Region/RegionUS915.c 

OBJS += \
./LoRaWAN/Mac/Region/Region.o \
./LoRaWAN/Mac/Region/RegionAS923.o \
./LoRaWAN/Mac/Region/RegionAU915.o \
./LoRaWAN/Mac/Region/RegionCN470.o \
./LoRaWAN/Mac/Region/RegionCN779.o \
./LoRaWAN/Mac/Region/RegionCommon.o \
./LoRaWAN/Mac/Region/RegionEU433.o \
./LoRaWAN/Mac/Region/RegionEU868.o \
./LoRaWAN/Mac/Region/RegionIN865.o \
./LoRaWAN/Mac/Region/RegionKR920.o \
./LoRaWAN/Mac/Region/RegionRU864.o \
./LoRaWAN/Mac/Region/RegionUS915.o 

C_DEPS += \
./LoRaWAN/Mac/Region/Region.d \
./LoRaWAN/Mac/Region/RegionAS923.d \
./LoRaWAN/Mac/Region/RegionAU915.d \
./LoRaWAN/Mac/Region/RegionCN470.d \
./LoRaWAN/Mac/Region/RegionCN779.d \
./LoRaWAN/Mac/Region/RegionCommon.d \
./LoRaWAN/Mac/Region/RegionEU433.d \
./LoRaWAN/Mac/Region/RegionEU868.d \
./LoRaWAN/Mac/Region/RegionIN865.d \
./LoRaWAN/Mac/Region/RegionKR920.d \
./LoRaWAN/Mac/Region/RegionRU864.d \
./LoRaWAN/Mac/Region/RegionUS915.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWAN/Mac/Region/%.o LoRaWAN/Mac/Region/%.su LoRaWAN/Mac/Region/%.cyclo: ../LoRaWAN/Mac/Region/%.c LoRaWAN/Mac/Region/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../LoRaWAN -I../LoRaWAN/LmHandler/packages -I../LoRaWAN/LmHandler -I../LoRaWAN/Crypto -I../LoRaWAN/Mac -I../LoRaWAN/Mac/Region -I../LoRaWAN/Radio -I../LoRaWAN/Utilities/timer -I../LoRaWAN/Utilities/mem -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LoRaWAN-2f-Mac-2f-Region

clean-LoRaWAN-2f-Mac-2f-Region:
	-$(RM) ./LoRaWAN/Mac/Region/Region.cyclo ./LoRaWAN/Mac/Region/Region.d ./LoRaWAN/Mac/Region/Region.o ./LoRaWAN/Mac/Region/Region.su ./LoRaWAN/Mac/Region/RegionAS923.cyclo ./LoRaWAN/Mac/Region/RegionAS923.d ./LoRaWAN/Mac/Region/RegionAS923.o ./LoRaWAN/Mac/Region/RegionAS923.su ./LoRaWAN/Mac/Region/RegionAU915.cyclo ./LoRaWAN/Mac/Region/RegionAU915.d ./LoRaWAN/Mac/Region/RegionAU915.o ./LoRaWAN/Mac/Region/RegionAU915.su ./LoRaWAN/Mac/Region/RegionCN470.cyclo ./LoRaWAN/Mac/Region/RegionCN470.d ./LoRaWAN/Mac/Region/RegionCN470.o ./LoRaWAN/Mac/Region/RegionCN470.su ./LoRaWAN/Mac/Region/RegionCN779.cyclo ./LoRaWAN/Mac/Region/RegionCN779.d ./LoRaWAN/Mac/Region/RegionCN779.o ./LoRaWAN/Mac/Region/RegionCN779.su ./LoRaWAN/Mac/Region/RegionCommon.cyclo ./LoRaWAN/Mac/Region/RegionCommon.d ./LoRaWAN/Mac/Region/RegionCommon.o ./LoRaWAN/Mac/Region/RegionCommon.su ./LoRaWAN/Mac/Region/RegionEU433.cyclo ./LoRaWAN/Mac/Region/RegionEU433.d ./LoRaWAN/Mac/Region/RegionEU433.o ./LoRaWAN/Mac/Region/RegionEU433.su ./LoRaWAN/Mac/Region/RegionEU868.cyclo ./LoRaWAN/Mac/Region/RegionEU868.d ./LoRaWAN/Mac/Region/RegionEU868.o ./LoRaWAN/Mac/Region/RegionEU868.su ./LoRaWAN/Mac/Region/RegionIN865.cyclo ./LoRaWAN/Mac/Region/RegionIN865.d ./LoRaWAN/Mac/Region/RegionIN865.o ./LoRaWAN/Mac/Region/RegionIN865.su ./LoRaWAN/Mac/Region/RegionKR920.cyclo ./LoRaWAN/Mac/Region/RegionKR920.d ./LoRaWAN/Mac/Region/RegionKR920.o ./LoRaWAN/Mac/Region/RegionKR920.su ./LoRaWAN/Mac/Region/RegionRU864.cyclo ./LoRaWAN/Mac/Region/RegionRU864.d ./LoRaWAN/Mac/Region/RegionRU864.o ./LoRaWAN/Mac/Region/RegionRU864.su ./LoRaWAN/Mac/Region/RegionUS915.cyclo ./LoRaWAN/Mac/Region/RegionUS915.d ./LoRaWAN/Mac/Region/RegionUS915.o ./LoRaWAN/Mac/Region/RegionUS915.su

.PHONY: clean-LoRaWAN-2f-Mac-2f-Region

