################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Util/logger/logger.c 

OBJS += \
./Util/logger/logger.o 

C_DEPS += \
./Util/logger/logger.d 


# Each subdirectory must supply rules for building sources it contributes
Util/logger/%.o Util/logger/%.su Util/logger/%.cyclo: ../Util/logger/%.c Util/logger/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LoRaWAN/App -I../LoRaWAN/Target -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../Middlewares/Third_Party/LoRaWAN/LmHandler -I../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy -I../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages -I../Util/logger -I../Util/mem -I../Util/timer -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Util-2f-logger

clean-Util-2f-logger:
	-$(RM) ./Util/logger/logger.cyclo ./Util/logger/logger.d ./Util/logger/logger.o ./Util/logger/logger.su

.PHONY: clean-Util-2f-logger

