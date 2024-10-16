################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TraceLog/TraceLog.c 

OBJS += \
./TraceLog/TraceLog.o 

C_DEPS += \
./TraceLog/TraceLog.d 


# Each subdirectory must supply rules for building sources it contributes
TraceLog/%.o TraceLog/%.su TraceLog/%.cyclo: ../TraceLog/%.c TraceLog/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../TraceLog -I../LoRaWAN -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TraceLog

clean-TraceLog:
	-$(RM) ./TraceLog/TraceLog.cyclo ./TraceLog/TraceLog.d ./TraceLog/TraceLog.o ./TraceLog/TraceLog.su

.PHONY: clean-TraceLog

