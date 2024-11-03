################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LoRaWAN/App/app_msgstructs.c \
../LoRaWAN/App/lora_apps.c 

OBJS += \
./LoRaWAN/App/app_msgstructs.o \
./LoRaWAN/App/lora_apps.o 

C_DEPS += \
./LoRaWAN/App/app_msgstructs.d \
./LoRaWAN/App/lora_apps.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWAN/App/%.o LoRaWAN/App/%.su LoRaWAN/App/%.cyclo: ../LoRaWAN/App/%.c LoRaWAN/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -DACTIVE_REGION=LORAMAC_REGION_AS923 -DSOFT_SE -DREGION_AS923 -c -I../Core/Inc -I../Util/logger -I../Util/mem -I../Util/timer -I../Util/nvmm -I../LoRaWAN/Porting -I../LoRaWAN/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy -I../Middlewares/Third_Party/LoRaWAN/Mac/region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../LoRaWAN/App/LmHandler -I../LoRaWAN/App/LmHandler/packages -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LoRaWAN-2f-App

clean-LoRaWAN-2f-App:
	-$(RM) ./LoRaWAN/App/app_msgstructs.cyclo ./LoRaWAN/App/app_msgstructs.d ./LoRaWAN/App/app_msgstructs.o ./LoRaWAN/App/app_msgstructs.su ./LoRaWAN/App/lora_apps.cyclo ./LoRaWAN/App/lora_apps.d ./LoRaWAN/App/lora_apps.o ./LoRaWAN/App/lora_apps.su

.PHONY: clean-LoRaWAN-2f-App

