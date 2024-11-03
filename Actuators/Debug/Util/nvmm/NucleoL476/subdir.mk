################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Util/nvmm/NucleoL476/eeprom-board.c 

OBJS += \
./Util/nvmm/NucleoL476/eeprom-board.o 

C_DEPS += \
./Util/nvmm/NucleoL476/eeprom-board.d 


# Each subdirectory must supply rules for building sources it contributes
Util/nvmm/NucleoL476/%.o Util/nvmm/NucleoL476/%.su Util/nvmm/NucleoL476/%.cyclo: ../Util/nvmm/NucleoL476/%.c Util/nvmm/NucleoL476/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -DACTIVE_REGION=LORAMAC_REGION_AS923 -DSOFT_SE -DREGION_AS923 -c -I../Core/Inc -I../Util/logger -I../Util/mem -I../Util/timer -I../Util/nvmm -I../LoRaWAN/Porting -I../LoRaWAN/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy -I../Middlewares/Third_Party/LoRaWAN/Mac/region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/LmHandler/packages -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../Middlewares/Third_Party/LoRaWAN/LmHandler -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Util-2f-nvmm-2f-NucleoL476

clean-Util-2f-nvmm-2f-NucleoL476:
	-$(RM) ./Util/nvmm/NucleoL476/eeprom-board.cyclo ./Util/nvmm/NucleoL476/eeprom-board.d ./Util/nvmm/NucleoL476/eeprom-board.o ./Util/nvmm/NucleoL476/eeprom-board.su

.PHONY: clean-Util-2f-nvmm-2f-NucleoL476

