################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Application/cJSON/cJSON.c \
../Core/Application/cJSON/cJSON_Utils.c 

C_DEPS += \
./Core/Application/cJSON/cJSON.d \
./Core/Application/cJSON/cJSON_Utils.d 

OBJS += \
./Core/Application/cJSON/cJSON.o \
./Core/Application/cJSON/cJSON_Utils.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Application/cJSON/%.o Core/Application/cJSON/%.su Core/Application/cJSON/%.cyclo: ../Core/Application/cJSON/%.c Core/Application/cJSON/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -D_GNU_SOURCE -c -I../Core/Inc -I../Core/Application -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I../LoRaWAN/App -I../LoRaWAN/Target -I../Utilities/trace/adv_trace -I../Utilities/misc -I../Utilities/timer -I../Utilities/lpm/tiny_lpm -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/LmHandler -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/SubGHz_Phy -I../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Application-2f-cJSON

clean-Core-2f-Application-2f-cJSON:
	-$(RM) ./Core/Application/cJSON/cJSON.cyclo ./Core/Application/cJSON/cJSON.d ./Core/Application/cJSON/cJSON.o ./Core/Application/cJSON/cJSON.su ./Core/Application/cJSON/cJSON_Utils.cyclo ./Core/Application/cJSON/cJSON_Utils.d ./Core/Application/cJSON/cJSON_Utils.o ./Core/Application/cJSON/cJSON_Utils.su

.PHONY: clean-Core-2f-Application-2f-cJSON

