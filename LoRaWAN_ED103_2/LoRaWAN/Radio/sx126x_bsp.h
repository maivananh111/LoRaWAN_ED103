

#ifndef LORA_SX126X_SX126X_BSP_H_
#define LORA_SX126X_SX126X_BSP_H_

#ifdef __cplusplus
extern "C"{
#endif


#include "stm32l4xx_hal.h"
#include "sx126x.h"



#define SX1261_CHIP 1
#define SX1262_CHIP 2
#define SX1268_CHIP 2

/**@brief Initializes the radio I/Os pins interface
 */
void SX126xIoInit(void);

/**@brief Initializes the radio I/Os pins interface after deep sleep wake
 */
void SX126xIoReInit(void);

/**@brief De-initializes the radio I/Os pins interface.
 *
 * \remark Useful when going in MCU low power modes
 */
void SX126xIoDeInit(void);

/**@brief HW Reset of the radio
 */
void SX126xReset(void);

/**@brief Blocking loop to wait while the Busy pin in high
 */
void SX126xWaitOnBusy(void);

/**@brief Wakes up the radio
 */
void SX126xWakeup(void);

/**@brief Send a command that write data to the radio
 *
 * \param   opcode        Opcode of the command
 * \param   buffer        Buffer to be send to the radio
 * \param   size          Size of the buffer to send
 */
void SX126xWriteCommand(RadioCommands_t opcode, uint8_t *buffer, uint16_t size);

/**@brief Send a command that read data from the radio
 *
 * \param   opcode        Opcode of the command
 * \param  buffer        Buffer holding data from the radio
 * \param   size          Size of the buffer
 */
void SX126xReadCommand(RadioCommands_t opcode, uint8_t *buffer, uint16_t size);

/**@brief Write a single byte of data to the radio memory
 *
 * \param   address       The address of the first byte to write in the radio
 * \param   value         The data to be written in radio's memory
 */
void SX126xWriteRegister(uint16_t address, uint8_t value);

/**@brief Read a single byte of data from the radio memory
 *
 * \param   address       The address of the first byte to write in the radio
 *
 * \retval      value         The value of the byte at the given address in radio's memory
 */
uint8_t SX126xReadRegister(uint16_t address);

/**@brief Sets the radio output power.
 *
 * \param  power Sets the RF output power
 */
void SX126xSetRfTxPower(int8_t power);

/**@brief Gets the board PA selection configuration
 *
 * \param  channel Channel frequency in Hz
 * \retval PaSelect RegPaConfig PaSelect value
 */
uint8_t SX126xGetPaSelect(uint32_t channel);

/**@brief Initializes the RF Switch I/Os pins interface
 */
void SX126xAntSwOn(void);

/**@brief De-initializes the RF Switch I/Os pins interface
 *
 * \remark Needed to decrease the power consumption in MCU low power modes
 */
void SX126xAntSwOff(void);

/**@brief Set the RF antenna switch to receiving mode
 *
 * \remark Used only on some modules e.g. eByte E22
 */
void SX126xRXena(void);

/**@brief Set the RF antenna switch to transmitting mode
 *
 * \remark Used only on some modules e.g. eByte E22
 */
void SX126xTXena(void);

/**@brief Checks if the given RF frequency is supported by the hardware
 *
 * \param  frequency RF frequency to be checked
 * \retval isSupported [true: supported, false: unsupported]
 */
bool SX126xCheckRfFrequency(uint32_t frequency);

/**@brief Gets info on the number of packets received
 *
 * \param  nb_pkt_received     Number of received packets with CRC OK
 * \param  nb_pkt_crc_error    Number of received packets with CRC error
 * \param  nb_pkt_length_error Number of received packets with length error
 */
void SX126xGetStats(uint16_t *nb_pkt_received, uint16_t *nb_pkt_crc_error, uint16_t *nb_pkt_length_error);

/**@brief Resets values read by GetStats
 */
void SX126xResetStats(void);


#ifdef __cplusplus
}
#endif

#endif // LORA_SX126X_SX126X_BSP_H_
