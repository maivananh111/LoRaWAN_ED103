################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LoRaWAN/App/LmHandler/CayenneLpp.c \
../LoRaWAN/App/LmHandler/LmHandler.c \
../LoRaWAN/App/LmHandler/LmHandlerMsgDisplay.c \
../LoRaWAN/App/LmHandler/NvmDataMgmt.c 

OBJS += \
./LoRaWAN/App/LmHandler/CayenneLpp.o \
./LoRaWAN/App/LmHandler/LmHandler.o \
./LoRaWAN/App/LmHandler/LmHandlerMsgDisplay.o \
./LoRaWAN/App/LmHandler/NvmDataMgmt.o 

C_DEPS += \
./LoRaWAN/App/LmHandler/CayenneLpp.d \
./LoRaWAN/App/LmHandler/LmHandler.d \
./LoRaWAN/App/LmHandler/LmHandlerMsgDisplay.d \
./LoRaWAN/App/LmHandler/NvmDataMgmt.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWAN/App/LmHandler/%.o LoRaWAN/App/LmHandler/%.su LoRaWAN/App/LmHandler/%.cyclo: ../LoRaWAN/App/LmHandler/%.c LoRaWAN/App/LmHandler/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -DACTIVE_REGION=LORAMAC_REGION_AS923 -DSOFT_SE -DREGION_AS923 -c -I../Core/Inc -I../Util/logger -I../Util/mem -I../Util/timer -I../Util/nvmm -I../LoRaWAN/Porting -I../LoRaWAN/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy -I../Middlewares/Third_Party/LoRaWAN/Mac/region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../LoRaWAN/App/LmHandler -I../LoRaWAN/App/LmHandler/packages -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LoRaWAN-2f-App-2f-LmHandler

clean-LoRaWAN-2f-App-2f-LmHandler:
	-$(RM) ./LoRaWAN/App/LmHandler/CayenneLpp.cyclo ./LoRaWAN/App/LmHandler/CayenneLpp.d ./LoRaWAN/App/LmHandler/CayenneLpp.o ./LoRaWAN/App/LmHandler/CayenneLpp.su ./LoRaWAN/App/LmHandler/LmHandler.cyclo ./LoRaWAN/App/LmHandler/LmHandler.d ./LoRaWAN/App/LmHandler/LmHandler.o ./LoRaWAN/App/LmHandler/LmHandler.su ./LoRaWAN/App/LmHandler/LmHandlerMsgDisplay.cyclo ./LoRaWAN/App/LmHandler/LmHandlerMsgDisplay.d ./LoRaWAN/App/LmHandler/LmHandlerMsgDisplay.o ./LoRaWAN/App/LmHandler/LmHandlerMsgDisplay.su ./LoRaWAN/App/LmHandler/NvmDataMgmt.cyclo ./LoRaWAN/App/LmHandler/NvmDataMgmt.d ./LoRaWAN/App/LmHandler/NvmDataMgmt.o ./LoRaWAN/App/LmHandler/NvmDataMgmt.su

.PHONY: clean-LoRaWAN-2f-App-2f-LmHandler

