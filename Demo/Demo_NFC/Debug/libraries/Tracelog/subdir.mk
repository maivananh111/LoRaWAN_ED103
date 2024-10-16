################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/Tracelog/TraceLog.c 

OBJS += \
./libraries/Tracelog/TraceLog.o 

C_DEPS += \
./libraries/Tracelog/TraceLog.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/Tracelog/%.o libraries/Tracelog/%.su libraries/Tracelog/%.cyclo: ../libraries/Tracelog/%.c libraries/Tracelog/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../libraries/ndef/Inc/message -I../libraries/ndef/Inc/poller -I../libraries/rfal/Inc -I../libraries/rfal/ST25R3911 -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../libraries -I../libraries/RFAL/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libraries-2f-Tracelog

clean-libraries-2f-Tracelog:
	-$(RM) ./libraries/Tracelog/TraceLog.cyclo ./libraries/Tracelog/TraceLog.d ./libraries/Tracelog/TraceLog.o ./libraries/Tracelog/TraceLog.su

.PHONY: clean-libraries-2f-Tracelog

