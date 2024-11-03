################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LoRaWAN/Crypto/aes.c \
../Middlewares/Third_Party/LoRaWAN/Crypto/cmac.c \
../Middlewares/Third_Party/LoRaWAN/Crypto/soft-se-hal.c \
../Middlewares/Third_Party/LoRaWAN/Crypto/soft-se.c 

OBJS += \
./Middlewares/Third_Party/LoRaWAN/Crypto/aes.o \
./Middlewares/Third_Party/LoRaWAN/Crypto/cmac.o \
./Middlewares/Third_Party/LoRaWAN/Crypto/soft-se-hal.o \
./Middlewares/Third_Party/LoRaWAN/Crypto/soft-se.o 

C_DEPS += \
./Middlewares/Third_Party/LoRaWAN/Crypto/aes.d \
./Middlewares/Third_Party/LoRaWAN/Crypto/cmac.d \
./Middlewares/Third_Party/LoRaWAN/Crypto/soft-se-hal.d \
./Middlewares/Third_Party/LoRaWAN/Crypto/soft-se.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LoRaWAN/Crypto/%.o Middlewares/Third_Party/LoRaWAN/Crypto/%.su Middlewares/Third_Party/LoRaWAN/Crypto/%.cyclo: ../Middlewares/Third_Party/LoRaWAN/Crypto/%.c Middlewares/Third_Party/LoRaWAN/Crypto/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -DACTIVE_REGION=LORAMAC_REGION_AS923 -DSOFT_SE -DREGION_AS923 -c -I../Core/Inc -I../Util/logger -I../Util/mem -I../Util/timer -I../Util/nvmm -I../LoRaWAN/Porting -I../LoRaWAN/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy -I../Middlewares/Third_Party/LoRaWAN/Mac/region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../LoRaWAN/App/LmHandler -I../LoRaWAN/App/LmHandler/packages -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-LoRaWAN-2f-Crypto

clean-Middlewares-2f-Third_Party-2f-LoRaWAN-2f-Crypto:
	-$(RM) ./Middlewares/Third_Party/LoRaWAN/Crypto/aes.cyclo ./Middlewares/Third_Party/LoRaWAN/Crypto/aes.d ./Middlewares/Third_Party/LoRaWAN/Crypto/aes.o ./Middlewares/Third_Party/LoRaWAN/Crypto/aes.su ./Middlewares/Third_Party/LoRaWAN/Crypto/cmac.cyclo ./Middlewares/Third_Party/LoRaWAN/Crypto/cmac.d ./Middlewares/Third_Party/LoRaWAN/Crypto/cmac.o ./Middlewares/Third_Party/LoRaWAN/Crypto/cmac.su ./Middlewares/Third_Party/LoRaWAN/Crypto/soft-se-hal.cyclo ./Middlewares/Third_Party/LoRaWAN/Crypto/soft-se-hal.d ./Middlewares/Third_Party/LoRaWAN/Crypto/soft-se-hal.o ./Middlewares/Third_Party/LoRaWAN/Crypto/soft-se-hal.su ./Middlewares/Third_Party/LoRaWAN/Crypto/soft-se.cyclo ./Middlewares/Third_Party/LoRaWAN/Crypto/soft-se.d ./Middlewares/Third_Party/LoRaWAN/Crypto/soft-se.o ./Middlewares/Third_Party/LoRaWAN/Crypto/soft-se.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-LoRaWAN-2f-Crypto

