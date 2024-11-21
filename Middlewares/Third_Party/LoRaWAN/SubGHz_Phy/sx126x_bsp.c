/*!
 * \file      sx1262mbxdas-board.c
 *
 * \brief     Target board SX1262MBXDAS shield driver implementation
 *
 * \remark    This target board is only available with the SX126xDVK1xAS
 *            development kit.
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
#include <stdlib.h>
#include "utilities.h"
#include "radio.h"
#include "sx126x_bsp.h"
#include "main.h"
#include "spi.h"



#define RADIO_PHY_HSPI hspi1

#ifndef CRITICAL_SECTION_BEGIN
#define CRITICAL_SECTION_BEGIN( )      __disable_irq()
#endif /* !CRITICAL_SECTION_BEGIN */
#ifndef CRITICAL_SECTION_END
#define CRITICAL_SECTION_END( )        __enable_irq()
#endif /* !CRITICAL_SECTION_END */

/*!
 * \brief Holds the internal operating mode of the radio
 */
static RadioOperatingModes_t OperatingMode;


void SX126xIoInit(void) {
}

void SX126xIoIrqInit(void) {
}

void SX126xIoDeInit(void) {
}

void SX126xIoDbgInit(void) {
}

void SX126xIoTcxoInit(void) {
	CalibrationParams_t calibParam;

	SX126xSetDio3AsTcxoCtrl(TCXO_CTRL_1_7V,
			SX126xGetBoardTcxoWakeupTime() << 6); // convert from ms to SX126x time base
	calibParam.Value = 0x7F;
	SX126xCalibrate(calibParam);
}

uint32_t SX126xGetBoardTcxoWakeupTime(void) {
	return 0;
}

void SX126xIoRfSwitchInit(void) {
	SX126xSetDio2AsRfSwitchCtrl( true);
}

RadioOperatingModes_t SX126xGetOperatingMode(void) {
	return OperatingMode;
}

void SX126xSetOperatingMode(RadioOperatingModes_t mode) {
	OperatingMode = mode;
}

void SX126xReset(void) {
	HAL_GPIO_WritePin(LORA_RST_GPIO_Port, LORA_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(LORA_RST_GPIO_Port, LORA_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(20);
}

void SX126xWaitOnBusy(void) {
	int timeout = 1000;
	while (HAL_GPIO_ReadPin(LORA_BUSY_GPIO_Port, LORA_BUSY_Pin) == GPIO_PIN_SET){
		for(int i=0; i<12000; i++)__NOP();
		timeout -= 1;
		if (timeout < 0){
			return;
		}
	}
}

void SX126xWakeup(void) {
	CRITICAL_SECTION_BEGIN();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	uint8_t buf[2] = {(uint8_t)RADIO_GET_STATUS, 0x00};
	HAL_SPI_Transmit(&RADIO_PHY_HSPI, buf, 2, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);
	// Wait for chip to be ready.
	SX126xWaitOnBusy();
	// Update operating mode context variable
	SX126xSetOperatingMode(MODE_STDBY_RC);

	CRITICAL_SECTION_END();
}

void SX126xWriteCommand(RadioCommands_t command, uint8_t *buffer, uint16_t size) {
	SX126xCheckDeviceReady();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&RADIO_PHY_HSPI, (uint8_t *)&command, 1, 1000);
	HAL_SPI_Transmit(&RADIO_PHY_HSPI, buffer, size, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	if (command != RADIO_SET_SLEEP) {
		SX126xWaitOnBusy();
	}
}

uint8_t SX126xReadCommand(RadioCommands_t command, uint8_t *buffer,
		uint16_t size) {
	uint8_t status = 0;

	SX126xCheckDeviceReady();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	uint8_t buf[2] = {(uint8_t)command, 0x00};
	HAL_SPI_Transmit(&RADIO_PHY_HSPI, buf, 2, 1000);
	HAL_SPI_Receive(&RADIO_PHY_HSPI, buffer, size, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	SX126xWaitOnBusy();

	return status;
}

void SX126xWriteRegisters(uint16_t address, uint8_t *buffer, uint16_t size) {
	SX126xCheckDeviceReady();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	uint8_t buf[3] = {(uint8_t)RADIO_WRITE_REGISTER, (uint8_t)((address & 0xFF00) >> 8), (uint8_t)(address & 0x00FF)};
	HAL_SPI_Transmit(&RADIO_PHY_HSPI, buf, 3, 1000);
	HAL_SPI_Transmit(&RADIO_PHY_HSPI, buffer, size, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	SX126xWaitOnBusy();
}

void SX126xWriteRegister(uint16_t address, uint8_t value) {
	SX126xWriteRegisters(address, &value, 1);
}

void SX126xReadRegisters(uint16_t address, uint8_t *buffer, uint16_t size) {
	SX126xCheckDeviceReady();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	uint8_t buf[4] = {(uint8_t)RADIO_READ_REGISTER, (uint8_t)((address & 0xFF00) >> 8), (uint8_t)(address & 0x00FF), 0x00};
	HAL_SPI_Transmit(&RADIO_PHY_HSPI, buf, 4, 1000);
	HAL_SPI_Receive(&RADIO_PHY_HSPI, buffer, size, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	SX126xWaitOnBusy();
}

uint8_t SX126xReadRegister(uint16_t address) {
	uint8_t data;
	SX126xReadRegisters(address, &data, 1);
	return data;
}

void SX126xWriteBuffer(uint8_t offset, uint8_t *buffer, uint8_t size) {
	SX126xCheckDeviceReady();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	uint8_t buf[2] = {(uint8_t)RADIO_WRITE_BUFFER, offset};
	HAL_SPI_Transmit(&RADIO_PHY_HSPI, buf, 2, 1000);
	HAL_SPI_Transmit(&RADIO_PHY_HSPI, buffer, size, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	SX126xWaitOnBusy();
}

void SX126xReadBuffer(uint8_t offset, uint8_t *buffer, uint8_t size) {
	SX126xCheckDeviceReady();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	uint8_t buf[3] = {(uint8_t)RADIO_READ_BUFFER, offset, 0x00};
	HAL_SPI_Transmit(&RADIO_PHY_HSPI, buf, 2, 1000);
	uint8_t dummy = 0;
	HAL_SPI_Receive(&RADIO_PHY_HSPI, &dummy, 1, 1000);
	HAL_SPI_Receive(&RADIO_PHY_HSPI, buffer, size, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	SX126xWaitOnBusy();
}

void SX126xSetRfTxPower(int8_t power) {
	SX126xSetTxParams(power, RADIO_RAMP_40_US);
}

uint8_t SX126xGetDeviceId(void) {
	return SX1262;
}

void SX126xAntSwOn(void) {
	SX126xSetDio2AsRfSwitchCtrl( true );
}

void SX126xAntSwOff(void) {
	SX126xSetDio2AsRfSwitchCtrl( false );
}

bool SX126xCheckRfFrequency(uint32_t frequency) {
	// Implement check. Currently all frequencies are supported
	return true;
}

uint32_t SX126xGetDio1PinState(void) {
	return 1;
}
