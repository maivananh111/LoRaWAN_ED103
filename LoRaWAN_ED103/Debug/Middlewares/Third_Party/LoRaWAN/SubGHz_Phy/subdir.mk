################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/radio.c \
../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x.c \
../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x_bsp.c 

OBJS += \
./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/radio.o \
./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x.o \
./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x_bsp.o 

C_DEPS += \
./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/radio.d \
./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x.d \
./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x_bsp.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/%.o Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/%.su Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/%.cyclo: ../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/%.c Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LoRaWAN/App -I../LoRaWAN/Target -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../Middlewares/Third_Party/LoRaWAN/LmHandler -I../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy -I../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages -I../Util/logger -I../Util/mem -I../Util/timer -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-LoRaWAN-2f-SubGHz_Phy

clean-Middlewares-2f-Third_Party-2f-LoRaWAN-2f-SubGHz_Phy:
	-$(RM) ./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/radio.cyclo ./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/radio.d ./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/radio.o ./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/radio.su ./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x.cyclo ./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x.d ./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x.o ./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x.su ./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x_bsp.cyclo ./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x_bsp.d ./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x_bsp.o ./Middlewares/Third_Party/LoRaWAN/SubGHz_Phy/sx126x_bsp.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-LoRaWAN-2f-SubGHz_Phy

