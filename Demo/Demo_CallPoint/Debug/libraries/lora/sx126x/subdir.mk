################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/lora/sx126x/sx126x.c \
../libraries/lora/sx126x/sx126x_bsp.c 

OBJS += \
./libraries/lora/sx126x/sx126x.o \
./libraries/lora/sx126x/sx126x_bsp.o 

C_DEPS += \
./libraries/lora/sx126x/sx126x.d \
./libraries/lora/sx126x/sx126x_bsp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/lora/sx126x/%.o libraries/lora/sx126x/%.su libraries/lora/sx126x/%.cyclo: ../libraries/lora/sx126x/%.c libraries/lora/sx126x/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U031xx -D_GNU_SOURCE -c -I../Core/Inc -I../libraries -I../Drivers/STM32U0xx_HAL_Driver/Inc -I../Drivers/STM32U0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-libraries-2f-lora-2f-sx126x

clean-libraries-2f-lora-2f-sx126x:
	-$(RM) ./libraries/lora/sx126x/sx126x.cyclo ./libraries/lora/sx126x/sx126x.d ./libraries/lora/sx126x/sx126x.o ./libraries/lora/sx126x/sx126x.su ./libraries/lora/sx126x/sx126x_bsp.cyclo ./libraries/lora/sx126x/sx126x_bsp.d ./libraries/lora/sx126x/sx126x_bsp.o ./libraries/lora/sx126x/sx126x_bsp.su

.PHONY: clean-libraries-2f-lora-2f-sx126x

