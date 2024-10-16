################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/utiltimer/drv_utiltimer.c \
../libraries/utiltimer/utiltimer.c 

OBJS += \
./libraries/utiltimer/drv_utiltimer.o \
./libraries/utiltimer/utiltimer.o 

C_DEPS += \
./libraries/utiltimer/drv_utiltimer.d \
./libraries/utiltimer/utiltimer.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/utiltimer/%.o libraries/utiltimer/%.su libraries/utiltimer/%.cyclo: ../libraries/utiltimer/%.c libraries/utiltimer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../libraries -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libraries-2f-utiltimer

clean-libraries-2f-utiltimer:
	-$(RM) ./libraries/utiltimer/drv_utiltimer.cyclo ./libraries/utiltimer/drv_utiltimer.d ./libraries/utiltimer/drv_utiltimer.o ./libraries/utiltimer/drv_utiltimer.su ./libraries/utiltimer/utiltimer.cyclo ./libraries/utiltimer/utiltimer.d ./libraries/utiltimer/utiltimer.o ./libraries/utiltimer/utiltimer.su

.PHONY: clean-libraries-2f-utiltimer

