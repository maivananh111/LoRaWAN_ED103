################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Util/nvmm/eeprom.c \
../Util/nvmm/nvmm.c 

OBJS += \
./Util/nvmm/eeprom.o \
./Util/nvmm/nvmm.o 

C_DEPS += \
./Util/nvmm/eeprom.d \
./Util/nvmm/nvmm.d 


# Each subdirectory must supply rules for building sources it contributes
Util/nvmm/%.o Util/nvmm/%.su Util/nvmm/%.cyclo: ../Util/nvmm/%.c Util/nvmm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -DACTIVE_REGION=LORAMAC_REGION_AS923 -DSOFT_SE -DREGION_AS923 -c -I../Core/Inc -I../Util/logger -I../Util/mem -I../Util/timer -I../Util/nvmm -I../LoRaWAN/Porting -I../LoRaWAN/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy -I../Middlewares/Third_Party/LoRaWAN/Mac/region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/LmHandler/packages -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../Middlewares/Third_Party/LoRaWAN/LmHandler -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Util-2f-nvmm

clean-Util-2f-nvmm:
	-$(RM) ./Util/nvmm/eeprom.cyclo ./Util/nvmm/eeprom.d ./Util/nvmm/eeprom.o ./Util/nvmm/eeprom.su ./Util/nvmm/nvmm.cyclo ./Util/nvmm/nvmm.d ./Util/nvmm/nvmm.o ./Util/nvmm/nvmm.su

.PHONY: clean-Util-2f-nvmm

