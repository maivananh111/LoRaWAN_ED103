

#include "sx126x_bsp.h"
#include "sx126x.h"
#include "main.h"



extern SPI_HandleTypeDef SX1262_SPI;


bool dio3IsOutput = false;

void SX126xIoInit(void){
	dio3IsOutput = false;

	SX126xReset();
}

void SX126xIoReInit(void){
	dio3IsOutput = false;
}


void SX126xIoDeInit(void){
	dio3IsOutput = false;
}

void SX126xReset(void){
	HAL_GPIO_WritePin(LORA_RST_GPIO_Port, LORA_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(LORA_RST_GPIO_Port, LORA_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(20);
	dio3IsOutput = false;
}

void SX126xWaitOnBusy(void){
	int timeout = 1000;
	while (HAL_GPIO_ReadPin(LORA_BUSY_GPIO_Port, LORA_BUSY_Pin) == GPIO_PIN_SET){
		for(int i=0; i<12000; i++)__NOP();
		timeout -= 1;
		if (timeout < 0){
			return;
		}
	}
}

void SX126xWakeup(void){
	dio3IsOutput = false;
	__disable_irq();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	uint8_t buf[2] = {(uint8_t)RADIO_GET_STATUS, 0x00};
	HAL_SPI_Transmit(&SX1262_SPI, buf, 2, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	SX126xWaitOnBusy();
	__enable_irq();
}

void SX126xWriteCommand(RadioCommands_t command, uint8_t *buffer, uint16_t size){
	SX126xCheckDeviceReady();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&SX1262_SPI, (uint8_t *)&command, 1, 1000);
	HAL_SPI_Transmit(&SX1262_SPI, buffer, size, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	if (command != RADIO_SET_SLEEP)
		SX126xWaitOnBusy();
}

void SX126xReadCommand(RadioCommands_t command, uint8_t *buffer, uint16_t size){
	SX126xCheckDeviceReady();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	uint8_t buf[2] = {(uint8_t)command, 0x00};
	HAL_SPI_Transmit(&SX1262_SPI, buf, 2, 1000);
	HAL_SPI_Receive(&SX1262_SPI, buffer, size, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	SX126xWaitOnBusy();
}

void SX126xWriteRegisters(uint16_t address, uint8_t *buffer, uint16_t size){
	SX126xCheckDeviceReady();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	uint8_t buf[3] = {(uint8_t)RADIO_WRITE_REGISTER, (uint8_t)((address & 0xFF00) >> 8), (uint8_t)(address & 0x00FF)};
	HAL_SPI_Transmit(&SX1262_SPI, buf, 3, 1000);
	HAL_SPI_Transmit(&SX1262_SPI, buffer, size, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	SX126xWaitOnBusy();
}

void SX126xWriteRegister(uint16_t address, uint8_t value){
	SX126xWriteRegisters(address, &value, 1);
}

void SX126xReadRegisters(uint16_t address, uint8_t *buffer, uint16_t size){
	SX126xCheckDeviceReady();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	uint8_t buf[4] = {(uint8_t)RADIO_READ_REGISTER, (uint8_t)((address & 0xFF00) >> 8), (uint8_t)(address & 0x00FF), 0x00};
	HAL_SPI_Transmit(&SX1262_SPI, buf, 4, 1000);
	HAL_SPI_Receive(&SX1262_SPI, buffer, size, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	SX126xWaitOnBusy();
}

uint8_t SX126xReadRegister(uint16_t address){
	uint8_t data;
	SX126xReadRegisters(address, &data, 1);
	return data;
}

void SX126xWriteBuffer(uint8_t offset, uint8_t *buffer, uint8_t size){
	SX126xCheckDeviceReady();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	uint8_t buf[2] = {(uint8_t)RADIO_WRITE_BUFFER, offset};
	HAL_SPI_Transmit(&SX1262_SPI, buf, 2, 1000);
	HAL_SPI_Transmit(&SX1262_SPI, buffer, size, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	SX126xWaitOnBusy();
}

void SX126xReadBuffer(uint8_t offset, uint8_t *buffer, uint8_t size){
	SX126xCheckDeviceReady();

	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
	uint8_t buf[3] = {(uint8_t)RADIO_READ_BUFFER, offset, 0x00};
	HAL_SPI_Transmit(&SX1262_SPI, buf, 2, 1000);
	uint8_t dummy = 0;
	HAL_SPI_Receive(&SX1262_SPI, &dummy, 1, 1000);
	HAL_SPI_Receive(&SX1262_SPI, buffer, size, 1000);
	HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

	SX126xWaitOnBusy();
}

void SX126xSetRfTxPower(int8_t power){
	SX126xSetTxParams(power, RADIO_RAMP_40_US);
}

uint8_t SX126xGetPaSelect(uint32_t channel){
	return SX1262;
}
/*
static void SX126xDio3Control(bool state){
	uint8_t reg_0x0580;
	uint8_t reg_0x0583;
	uint8_t reg_0x0584;
	uint8_t reg_0x0585;
	uint8_t reg_0x0920;

	if (!dio3IsOutput){
		// Read 0x0580
		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_READ_REGISTER);
		SPI_LORA.transfer((0x0580 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0580 & 0x00FF);
		SPI_LORA.transfer(0x00);
		reg_0x0580 = SPI_LORA.transfer(0x00);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);

		// Read 0x0583
		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_READ_REGISTER);
		SPI_LORA.transfer((0x0583 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0583 & 0x00FF);
		SPI_LORA.transfer(0x00);
		reg_0x0583 = SPI_LORA.transfer(0x00);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);

		// Read 0x0584
		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_READ_REGISTER);
		SPI_LORA.transfer((0x0584 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0584 & 0x00FF);
		SPI_LORA.transfer(0x00);
		reg_0x0584 = SPI_LORA.transfer(0x00);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);

		// Read 0x0585
		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_READ_REGISTER);
		SPI_LORA.transfer((0x0585 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0585 & 0x00FF);
		SPI_LORA.transfer(0x00);
		reg_0x0585 = SPI_LORA.transfer(0x00);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);

		// Write 0x0580
		// SX126xWriteRegister(0x0580, reg_0x0580 | 0x08);
		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_WRITE_REGISTER);
		SPI_LORA.transfer((0x0580 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0580 & 0x00FF);
		SPI_LORA.transfer(reg_0x0580 | 0x08);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);

		// Write 0x0583
		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_WRITE_REGISTER);
		SPI_LORA.transfer((0x0583 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0583 & 0x00FF);
		SPI_LORA.transfer(reg_0x0583 & ~0x08);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);

		// Write 0x0584
		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_WRITE_REGISTER);
		SPI_LORA.transfer((0x0584 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0584 & 0x00FF);
		SPI_LORA.transfer(reg_0x0584 & ~0x08);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);

		// Write 0x0585
		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_WRITE_REGISTER);
		SPI_LORA.transfer((0x0585 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0585 & 0x00FF);
		SPI_LORA.transfer(reg_0x0585 & ~0x08);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);

		// Write 0x0920
		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_WRITE_REGISTER);
		SPI_LORA.transfer((0x0920 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0920 & 0x00FF);
		SPI_LORA.transfer(0x06);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);

		dio3IsOutput = true;
	}

	if (state)
	{
		// Set DIO3 High
		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_READ_REGISTER);
		SPI_LORA.transfer((0x0920 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0920 & 0x00FF);
		SPI_LORA.transfer(0x00);
		reg_0x0920 = SPI_LORA.transfer(0x00);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);

		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_WRITE_REGISTER);
		SPI_LORA.transfer((0x0920 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0920 & 0x00FF);
		SPI_LORA.transfer(reg_0x0920 | 0x08);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);
	}
	else
	{
		// Set DIO3 Low
		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_READ_REGISTER);
		SPI_LORA.transfer((0x0920 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0920 & 0x00FF);
		SPI_LORA.transfer(0x00);
		reg_0x0920 = SPI_LORA.transfer(0x00);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);

		SX126xWaitOnBusy();
		digitalWrite(_hwConfig.PIN_LORA_NSS, LOW);
		SPI_LORA.beginTransaction(spiSettings);
		SPI_LORA.transfer(RADIO_WRITE_REGISTER);
		SPI_LORA.transfer((0x0920 & 0xFF00) >> 8);
		SPI_LORA.transfer(0x0920 & 0x00FF);
		SPI_LORA.transfer(reg_0x0920 & ~0x08);
		SPI_LORA.endTransaction();
		digitalWrite(_hwConfig.PIN_LORA_NSS, HIGH);
	}
}
*/
void SX126xAntSwOn(void){
}

void SX126xAntSwOff(void){
}

void SX126xRXena(void){
}

void SX126xTXena(void){
}

bool SX126xCheckRfFrequency(uint32_t frequency){
	return true;
}

void SX126xGetStats(uint16_t *nb_pkt_received, uint16_t *nb_pkt_crc_error, uint16_t *nb_pkt_length_error){
	uint8_t buf[6];

	SX126xReadCommand(RADIO_GET_STATS, buf, 6);

	*nb_pkt_received = (buf[0] << 8) | buf[1];
	*nb_pkt_crc_error = (buf[2] << 8) | buf[3];
	*nb_pkt_length_error = (buf[4] << 8) | buf[5];
}

void SX126xResetStats(void){
	uint8_t buf[6] = {0x00};

	SX126xWriteCommand(RADIO_RESET_STATS, buf, 6);
}
