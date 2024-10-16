################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../LoRaWAN/App/lora_apps.cpp 

C_SRCS += \
../LoRaWAN/App/CayenneLpp.c \
../LoRaWAN/App/app_lorawan.c \
../LoRaWAN/App/lora_app.c \
../LoRaWAN/App/lora_info.c 

C_DEPS += \
./LoRaWAN/App/CayenneLpp.d \
./LoRaWAN/App/app_lorawan.d \
./LoRaWAN/App/lora_app.d \
./LoRaWAN/App/lora_info.d 

OBJS += \
./LoRaWAN/App/CayenneLpp.o \
./LoRaWAN/App/app_lorawan.o \
./LoRaWAN/App/lora_app.o \
./LoRaWAN/App/lora_apps.o \
./LoRaWAN/App/lora_info.o 

CPP_DEPS += \
./LoRaWAN/App/lora_apps.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWAN/App/%.o LoRaWAN/App/%.su LoRaWAN/App/%.cyclo: ../LoRaWAN/App/%.c LoRaWAN/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -D_GNU_SOURCE -c -I../Core/Inc -I../Core/Application -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I../LoRaWAN/App -I../LoRaWAN/Target -I../Utilities/trace/adv_trace -I../Utilities/misc -I../Utilities/timer -I../Utilities/lpm/tiny_lpm -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/LmHandler -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/SubGHz_Phy -I../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
LoRaWAN/App/%.o LoRaWAN/App/%.su LoRaWAN/App/%.cyclo: ../LoRaWAN/App/%.cpp LoRaWAN/App/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -D_GNU_SOURCE -c -I../Core/Inc -I../Core/Application -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I../LoRaWAN/App -I../LoRaWAN/Target -I../Utilities/trace/adv_trace -I../Utilities/misc -I../Utilities/timer -I../Utilities/lpm/tiny_lpm -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/LmHandler -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/SubGHz_Phy -I../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-LoRaWAN-2f-App

clean-LoRaWAN-2f-App:
	-$(RM) ./LoRaWAN/App/CayenneLpp.cyclo ./LoRaWAN/App/CayenneLpp.d ./LoRaWAN/App/CayenneLpp.o ./LoRaWAN/App/CayenneLpp.su ./LoRaWAN/App/app_lorawan.cyclo ./LoRaWAN/App/app_lorawan.d ./LoRaWAN/App/app_lorawan.o ./LoRaWAN/App/app_lorawan.su ./LoRaWAN/App/lora_app.cyclo ./LoRaWAN/App/lora_app.d ./LoRaWAN/App/lora_app.o ./LoRaWAN/App/lora_app.su ./LoRaWAN/App/lora_apps.cyclo ./LoRaWAN/App/lora_apps.d ./LoRaWAN/App/lora_apps.o ./LoRaWAN/App/lora_apps.su ./LoRaWAN/App/lora_info.cyclo ./LoRaWAN/App/lora_info.d ./LoRaWAN/App/lora_info.o ./LoRaWAN/App/lora_info.su

.PHONY: clean-LoRaWAN-2f-App

