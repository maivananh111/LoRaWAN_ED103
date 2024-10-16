################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/RFAL/source/rfal_analogConfig.c \
../libraries/RFAL/source/rfal_crc.c \
../libraries/RFAL/source/rfal_dpo.c \
../libraries/RFAL/source/rfal_iso15693_2.c \
../libraries/RFAL/source/rfal_isoDep.c \
../libraries/RFAL/source/rfal_nfc.c \
../libraries/RFAL/source/rfal_nfcDep.c \
../libraries/RFAL/source/rfal_nfca.c \
../libraries/RFAL/source/rfal_nfcb.c \
../libraries/RFAL/source/rfal_nfcf.c \
../libraries/RFAL/source/rfal_nfcv.c \
../libraries/RFAL/source/rfal_rfst25r3911.c \
../libraries/RFAL/source/rfal_st25tb.c \
../libraries/RFAL/source/rfal_st25xv.c \
../libraries/RFAL/source/rfal_t1t.c \
../libraries/RFAL/source/rfal_t2t.c \
../libraries/RFAL/source/rfal_t4t.c 

OBJS += \
./libraries/RFAL/source/rfal_analogConfig.o \
./libraries/RFAL/source/rfal_crc.o \
./libraries/RFAL/source/rfal_dpo.o \
./libraries/RFAL/source/rfal_iso15693_2.o \
./libraries/RFAL/source/rfal_isoDep.o \
./libraries/RFAL/source/rfal_nfc.o \
./libraries/RFAL/source/rfal_nfcDep.o \
./libraries/RFAL/source/rfal_nfca.o \
./libraries/RFAL/source/rfal_nfcb.o \
./libraries/RFAL/source/rfal_nfcf.o \
./libraries/RFAL/source/rfal_nfcv.o \
./libraries/RFAL/source/rfal_rfst25r3911.o \
./libraries/RFAL/source/rfal_st25tb.o \
./libraries/RFAL/source/rfal_st25xv.o \
./libraries/RFAL/source/rfal_t1t.o \
./libraries/RFAL/source/rfal_t2t.o \
./libraries/RFAL/source/rfal_t4t.o 

C_DEPS += \
./libraries/RFAL/source/rfal_analogConfig.d \
./libraries/RFAL/source/rfal_crc.d \
./libraries/RFAL/source/rfal_dpo.d \
./libraries/RFAL/source/rfal_iso15693_2.d \
./libraries/RFAL/source/rfal_isoDep.d \
./libraries/RFAL/source/rfal_nfc.d \
./libraries/RFAL/source/rfal_nfcDep.d \
./libraries/RFAL/source/rfal_nfca.d \
./libraries/RFAL/source/rfal_nfcb.d \
./libraries/RFAL/source/rfal_nfcf.d \
./libraries/RFAL/source/rfal_nfcv.d \
./libraries/RFAL/source/rfal_rfst25r3911.d \
./libraries/RFAL/source/rfal_st25tb.d \
./libraries/RFAL/source/rfal_st25xv.d \
./libraries/RFAL/source/rfal_t1t.d \
./libraries/RFAL/source/rfal_t2t.d \
./libraries/RFAL/source/rfal_t4t.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/RFAL/source/%.o libraries/RFAL/source/%.su libraries/RFAL/source/%.cyclo: ../libraries/RFAL/source/%.c libraries/RFAL/source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -D_GNU_SOURCE -c -I../Core/Inc -I../libraries/ndef/Inc/message -I../libraries/ndef/Inc/poller -I../libraries/RFAL/st25r3911 -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../libraries -I../libraries/RFAL/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libraries-2f-RFAL-2f-source

clean-libraries-2f-RFAL-2f-source:
	-$(RM) ./libraries/RFAL/source/rfal_analogConfig.cyclo ./libraries/RFAL/source/rfal_analogConfig.d ./libraries/RFAL/source/rfal_analogConfig.o ./libraries/RFAL/source/rfal_analogConfig.su ./libraries/RFAL/source/rfal_crc.cyclo ./libraries/RFAL/source/rfal_crc.d ./libraries/RFAL/source/rfal_crc.o ./libraries/RFAL/source/rfal_crc.su ./libraries/RFAL/source/rfal_dpo.cyclo ./libraries/RFAL/source/rfal_dpo.d ./libraries/RFAL/source/rfal_dpo.o ./libraries/RFAL/source/rfal_dpo.su ./libraries/RFAL/source/rfal_iso15693_2.cyclo ./libraries/RFAL/source/rfal_iso15693_2.d ./libraries/RFAL/source/rfal_iso15693_2.o ./libraries/RFAL/source/rfal_iso15693_2.su ./libraries/RFAL/source/rfal_isoDep.cyclo ./libraries/RFAL/source/rfal_isoDep.d ./libraries/RFAL/source/rfal_isoDep.o ./libraries/RFAL/source/rfal_isoDep.su ./libraries/RFAL/source/rfal_nfc.cyclo ./libraries/RFAL/source/rfal_nfc.d ./libraries/RFAL/source/rfal_nfc.o ./libraries/RFAL/source/rfal_nfc.su ./libraries/RFAL/source/rfal_nfcDep.cyclo ./libraries/RFAL/source/rfal_nfcDep.d ./libraries/RFAL/source/rfal_nfcDep.o ./libraries/RFAL/source/rfal_nfcDep.su ./libraries/RFAL/source/rfal_nfca.cyclo ./libraries/RFAL/source/rfal_nfca.d ./libraries/RFAL/source/rfal_nfca.o ./libraries/RFAL/source/rfal_nfca.su ./libraries/RFAL/source/rfal_nfcb.cyclo ./libraries/RFAL/source/rfal_nfcb.d ./libraries/RFAL/source/rfal_nfcb.o ./libraries/RFAL/source/rfal_nfcb.su ./libraries/RFAL/source/rfal_nfcf.cyclo ./libraries/RFAL/source/rfal_nfcf.d ./libraries/RFAL/source/rfal_nfcf.o ./libraries/RFAL/source/rfal_nfcf.su ./libraries/RFAL/source/rfal_nfcv.cyclo ./libraries/RFAL/source/rfal_nfcv.d ./libraries/RFAL/source/rfal_nfcv.o ./libraries/RFAL/source/rfal_nfcv.su ./libraries/RFAL/source/rfal_rfst25r3911.cyclo ./libraries/RFAL/source/rfal_rfst25r3911.d ./libraries/RFAL/source/rfal_rfst25r3911.o ./libraries/RFAL/source/rfal_rfst25r3911.su ./libraries/RFAL/source/rfal_st25tb.cyclo ./libraries/RFAL/source/rfal_st25tb.d ./libraries/RFAL/source/rfal_st25tb.o ./libraries/RFAL/source/rfal_st25tb.su ./libraries/RFAL/source/rfal_st25xv.cyclo ./libraries/RFAL/source/rfal_st25xv.d ./libraries/RFAL/source/rfal_st25xv.o ./libraries/RFAL/source/rfal_st25xv.su ./libraries/RFAL/source/rfal_t1t.cyclo ./libraries/RFAL/source/rfal_t1t.d ./libraries/RFAL/source/rfal_t1t.o ./libraries/RFAL/source/rfal_t1t.su ./libraries/RFAL/source/rfal_t2t.cyclo ./libraries/RFAL/source/rfal_t2t.d ./libraries/RFAL/source/rfal_t2t.o ./libraries/RFAL/source/rfal_t2t.su ./libraries/RFAL/source/rfal_t4t.cyclo ./libraries/RFAL/source/rfal_t4t.d ./libraries/RFAL/source/rfal_t4t.o ./libraries/RFAL/source/rfal_t4t.su

.PHONY: clean-libraries-2f-RFAL-2f-source

