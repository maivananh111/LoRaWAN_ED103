################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Util/timer/rtc-board.c \
../Util/timer/systime.c \
../Util/timer/timer.c 

OBJS += \
./Util/timer/rtc-board.o \
./Util/timer/systime.o \
./Util/timer/timer.o 

C_DEPS += \
./Util/timer/rtc-board.d \
./Util/timer/systime.d \
./Util/timer/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Util/timer/%.o Util/timer/%.su Util/timer/%.cyclo: ../Util/timer/%.c Util/timer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -DACTIVE_REGION=LORAMAC_REGION_AS923 -DSOFT_SE -DREGION_AS923 -c -I../Core/Inc -I../Util/logger -I../Util/mem -I../Util/timer -I../Util/nvmm -I../LoRaWAN/Porting -I../LoRaWAN/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/LoRaWAN/Utilities -I../Middlewares/Third_Party/LoRaWAN/SubGHz_Phy -I../Middlewares/Third_Party/LoRaWAN/Mac/region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../LoRaWAN/App/LmHandler -I../LoRaWAN/App/LmHandler/packages -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Util-2f-timer

clean-Util-2f-timer:
	-$(RM) ./Util/timer/rtc-board.cyclo ./Util/timer/rtc-board.d ./Util/timer/rtc-board.o ./Util/timer/rtc-board.su ./Util/timer/systime.cyclo ./Util/timer/systime.d ./Util/timer/systime.o ./Util/timer/systime.su ./Util/timer/timer.cyclo ./Util/timer/timer.d ./Util/timer/timer.o ./Util/timer/timer.su

.PHONY: clean-Util-2f-timer

