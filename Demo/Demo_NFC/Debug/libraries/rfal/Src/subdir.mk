################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/rfal/Src/rfal_analogConfig.c \
../libraries/rfal/Src/rfal_crc.c \
../libraries/rfal/Src/rfal_dpo.c \
../libraries/rfal/Src/rfal_iso15693_2.c \
../libraries/rfal/Src/rfal_isoDep.c \
../libraries/rfal/Src/rfal_nfc.c \
../libraries/rfal/Src/rfal_nfcDep.c \
../libraries/rfal/Src/rfal_nfca.c \
../libraries/rfal/Src/rfal_nfcb.c \
../libraries/rfal/Src/rfal_nfcf.c \
../libraries/rfal/Src/rfal_nfcv.c \
../libraries/rfal/Src/rfal_rfst25r3911.c \
../libraries/rfal/Src/rfal_st25tb.c \
../libraries/rfal/Src/rfal_st25xv.c \
../libraries/rfal/Src/rfal_t1t.c \
../libraries/rfal/Src/rfal_t2t.c \
../libraries/rfal/Src/rfal_t4t.c 

OBJS += \
./libraries/rfal/Src/rfal_analogConfig.o \
./libraries/rfal/Src/rfal_crc.o \
./libraries/rfal/Src/rfal_dpo.o \
./libraries/rfal/Src/rfal_iso15693_2.o \
./libraries/rfal/Src/rfal_isoDep.o \
./libraries/rfal/Src/rfal_nfc.o \
./libraries/rfal/Src/rfal_nfcDep.o \
./libraries/rfal/Src/rfal_nfca.o \
./libraries/rfal/Src/rfal_nfcb.o \
./libraries/rfal/Src/rfal_nfcf.o \
./libraries/rfal/Src/rfal_nfcv.o \
./libraries/rfal/Src/rfal_rfst25r3911.o \
./libraries/rfal/Src/rfal_st25tb.o \
./libraries/rfal/Src/rfal_st25xv.o \
./libraries/rfal/Src/rfal_t1t.o \
./libraries/rfal/Src/rfal_t2t.o \
./libraries/rfal/Src/rfal_t4t.o 

C_DEPS += \
./libraries/rfal/Src/rfal_analogConfig.d \
./libraries/rfal/Src/rfal_crc.d \
./libraries/rfal/Src/rfal_dpo.d \
./libraries/rfal/Src/rfal_iso15693_2.d \
./libraries/rfal/Src/rfal_isoDep.d \
./libraries/rfal/Src/rfal_nfc.d \
./libraries/rfal/Src/rfal_nfcDep.d \
./libraries/rfal/Src/rfal_nfca.d \
./libraries/rfal/Src/rfal_nfcb.d \
./libraries/rfal/Src/rfal_nfcf.d \
./libraries/rfal/Src/rfal_nfcv.d \
./libraries/rfal/Src/rfal_rfst25r3911.d \
./libraries/rfal/Src/rfal_st25tb.d \
./libraries/rfal/Src/rfal_st25xv.d \
./libraries/rfal/Src/rfal_t1t.d \
./libraries/rfal/Src/rfal_t2t.d \
./libraries/rfal/Src/rfal_t4t.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/rfal/Src/%.o libraries/rfal/Src/%.su libraries/rfal/Src/%.cyclo: ../libraries/rfal/Src/%.c libraries/rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../libraries/ndef/Inc/message -I../libraries/ndef/Inc/poller -I../libraries/rfal/Inc -I../libraries/rfal/ST25R3911 -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../libraries -I../libraries/RFAL/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libraries-2f-rfal-2f-Src

clean-libraries-2f-rfal-2f-Src:
	-$(RM) ./libraries/rfal/Src/rfal_analogConfig.cyclo ./libraries/rfal/Src/rfal_analogConfig.d ./libraries/rfal/Src/rfal_analogConfig.o ./libraries/rfal/Src/rfal_analogConfig.su ./libraries/rfal/Src/rfal_crc.cyclo ./libraries/rfal/Src/rfal_crc.d ./libraries/rfal/Src/rfal_crc.o ./libraries/rfal/Src/rfal_crc.su ./libraries/rfal/Src/rfal_dpo.cyclo ./libraries/rfal/Src/rfal_dpo.d ./libraries/rfal/Src/rfal_dpo.o ./libraries/rfal/Src/rfal_dpo.su ./libraries/rfal/Src/rfal_iso15693_2.cyclo ./libraries/rfal/Src/rfal_iso15693_2.d ./libraries/rfal/Src/rfal_iso15693_2.o ./libraries/rfal/Src/rfal_iso15693_2.su ./libraries/rfal/Src/rfal_isoDep.cyclo ./libraries/rfal/Src/rfal_isoDep.d ./libraries/rfal/Src/rfal_isoDep.o ./libraries/rfal/Src/rfal_isoDep.su ./libraries/rfal/Src/rfal_nfc.cyclo ./libraries/rfal/Src/rfal_nfc.d ./libraries/rfal/Src/rfal_nfc.o ./libraries/rfal/Src/rfal_nfc.su ./libraries/rfal/Src/rfal_nfcDep.cyclo ./libraries/rfal/Src/rfal_nfcDep.d ./libraries/rfal/Src/rfal_nfcDep.o ./libraries/rfal/Src/rfal_nfcDep.su ./libraries/rfal/Src/rfal_nfca.cyclo ./libraries/rfal/Src/rfal_nfca.d ./libraries/rfal/Src/rfal_nfca.o ./libraries/rfal/Src/rfal_nfca.su ./libraries/rfal/Src/rfal_nfcb.cyclo ./libraries/rfal/Src/rfal_nfcb.d ./libraries/rfal/Src/rfal_nfcb.o ./libraries/rfal/Src/rfal_nfcb.su ./libraries/rfal/Src/rfal_nfcf.cyclo ./libraries/rfal/Src/rfal_nfcf.d ./libraries/rfal/Src/rfal_nfcf.o ./libraries/rfal/Src/rfal_nfcf.su ./libraries/rfal/Src/rfal_nfcv.cyclo ./libraries/rfal/Src/rfal_nfcv.d ./libraries/rfal/Src/rfal_nfcv.o ./libraries/rfal/Src/rfal_nfcv.su ./libraries/rfal/Src/rfal_rfst25r3911.cyclo ./libraries/rfal/Src/rfal_rfst25r3911.d ./libraries/rfal/Src/rfal_rfst25r3911.o ./libraries/rfal/Src/rfal_rfst25r3911.su ./libraries/rfal/Src/rfal_st25tb.cyclo ./libraries/rfal/Src/rfal_st25tb.d ./libraries/rfal/Src/rfal_st25tb.o ./libraries/rfal/Src/rfal_st25tb.su ./libraries/rfal/Src/rfal_st25xv.cyclo ./libraries/rfal/Src/rfal_st25xv.d ./libraries/rfal/Src/rfal_st25xv.o ./libraries/rfal/Src/rfal_st25xv.su ./libraries/rfal/Src/rfal_t1t.cyclo ./libraries/rfal/Src/rfal_t1t.d ./libraries/rfal/Src/rfal_t1t.o ./libraries/rfal/Src/rfal_t1t.su ./libraries/rfal/Src/rfal_t2t.cyclo ./libraries/rfal/Src/rfal_t2t.d ./libraries/rfal/Src/rfal_t2t.o ./libraries/rfal/Src/rfal_t2t.su ./libraries/rfal/Src/rfal_t4t.cyclo ./libraries/rfal/Src/rfal_t4t.d ./libraries/rfal/Src/rfal_t4t.o ./libraries/rfal/Src/rfal_t4t.su

.PHONY: clean-libraries-2f-rfal-2f-Src

