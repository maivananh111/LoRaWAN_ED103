################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LoRaWAN/LoRaWAN/phy/radio.c \
../LoRaWAN/LoRaWAN/phy/sx126x.c \
../LoRaWAN/LoRaWAN/phy/sx126x_bsp.c 

OBJS += \
./LoRaWAN/LoRaWAN/phy/radio.o \
./LoRaWAN/LoRaWAN/phy/sx126x.o \
./LoRaWAN/LoRaWAN/phy/sx126x_bsp.o 

C_DEPS += \
./LoRaWAN/LoRaWAN/phy/radio.d \
./LoRaWAN/LoRaWAN/phy/sx126x.d \
./LoRaWAN/LoRaWAN/phy/sx126x_bsp.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWAN/LoRaWAN/phy/%.o LoRaWAN/LoRaWAN/phy/%.su LoRaWAN/LoRaWAN/phy/%.cyclo: ../LoRaWAN/LoRaWAN/phy/%.c LoRaWAN/LoRaWAN/phy/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../TraceLog -I../LoRaWAN -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LoRaWAN-2f-LoRaWAN-2f-phy

clean-LoRaWAN-2f-LoRaWAN-2f-phy:
	-$(RM) ./LoRaWAN/LoRaWAN/phy/radio.cyclo ./LoRaWAN/LoRaWAN/phy/radio.d ./LoRaWAN/LoRaWAN/phy/radio.o ./LoRaWAN/LoRaWAN/phy/radio.su ./LoRaWAN/LoRaWAN/phy/sx126x.cyclo ./LoRaWAN/LoRaWAN/phy/sx126x.d ./LoRaWAN/LoRaWAN/phy/sx126x.o ./LoRaWAN/LoRaWAN/phy/sx126x.su ./LoRaWAN/LoRaWAN/phy/sx126x_bsp.cyclo ./LoRaWAN/LoRaWAN/phy/sx126x_bsp.d ./LoRaWAN/LoRaWAN/phy/sx126x_bsp.o ./LoRaWAN/LoRaWAN/phy/sx126x_bsp.su

.PHONY: clean-LoRaWAN-2f-LoRaWAN-2f-phy

