################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/RFAL/st25r3911/st25r3911.c \
../libraries/RFAL/st25r3911/st25r3911_com.c \
../libraries/RFAL/st25r3911/st25r3911_interrupt.c 

OBJS += \
./libraries/RFAL/st25r3911/st25r3911.o \
./libraries/RFAL/st25r3911/st25r3911_com.o \
./libraries/RFAL/st25r3911/st25r3911_interrupt.o 

C_DEPS += \
./libraries/RFAL/st25r3911/st25r3911.d \
./libraries/RFAL/st25r3911/st25r3911_com.d \
./libraries/RFAL/st25r3911/st25r3911_interrupt.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/RFAL/st25r3911/%.o libraries/RFAL/st25r3911/%.su libraries/RFAL/st25r3911/%.cyclo: ../libraries/RFAL/st25r3911/%.c libraries/RFAL/st25r3911/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../libraries/ndef/Inc/message -I../libraries/ndef/Inc/poller -I../libraries/RFAL/st25r3911 -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../libraries -I../libraries/RFAL/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libraries-2f-RFAL-2f-st25r3911

clean-libraries-2f-RFAL-2f-st25r3911:
	-$(RM) ./libraries/RFAL/st25r3911/st25r3911.cyclo ./libraries/RFAL/st25r3911/st25r3911.d ./libraries/RFAL/st25r3911/st25r3911.o ./libraries/RFAL/st25r3911/st25r3911.su ./libraries/RFAL/st25r3911/st25r3911_com.cyclo ./libraries/RFAL/st25r3911/st25r3911_com.d ./libraries/RFAL/st25r3911/st25r3911_com.o ./libraries/RFAL/st25r3911/st25r3911_com.su ./libraries/RFAL/st25r3911/st25r3911_interrupt.cyclo ./libraries/RFAL/st25r3911/st25r3911_interrupt.d ./libraries/RFAL/st25r3911/st25r3911_interrupt.o ./libraries/RFAL/st25r3911/st25r3911_interrupt.su

.PHONY: clean-libraries-2f-RFAL-2f-st25r3911

