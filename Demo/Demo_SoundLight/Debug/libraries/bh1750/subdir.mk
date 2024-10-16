################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/bh1750/bh1750.c 

OBJS += \
./libraries/bh1750/bh1750.o 

C_DEPS += \
./libraries/bh1750/bh1750.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/bh1750/%.o libraries/bh1750/%.su libraries/bh1750/%.cyclo: ../libraries/bh1750/%.c libraries/bh1750/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U031xx -c -I../Core/Inc -I../libraries -I../Drivers/STM32U0xx_HAL_Driver/Inc -I../Drivers/STM32U0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-libraries-2f-bh1750

clean-libraries-2f-bh1750:
	-$(RM) ./libraries/bh1750/bh1750.cyclo ./libraries/bh1750/bh1750.d ./libraries/bh1750/bh1750.o ./libraries/bh1750/bh1750.su

.PHONY: clean-libraries-2f-bh1750

