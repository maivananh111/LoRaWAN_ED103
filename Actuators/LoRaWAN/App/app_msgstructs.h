/*
 * app_msgstructs.h
 *
 *  Created on: Oct 20, 2024
 *      Author: anh
 */

#ifndef APP_APP_MSGSTRUCTS_H_
#define APP_APP_MSGSTRUCTS_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"
#include "stdbool.h"



typedef enum {
	LIGHT_SOUND 			= 0x01,
	CALLPOINT_ACTUATOR 		= 0x02,
	SMOKE_ACTUATOR 			= 0x03,
	HEAT_ACTUATOR 			= 0x04,
} device_types_t;


typedef enum {
	/**
	 * Bootup message request and response.
	 */
	APPMSG_REQ_BOOT_UP 					= 0xFF,
	APPMSG_RES_BOOT_UP 					= 0xF1,

	/**
	 * Regular check message request and response.
	 */
	APPMSG_REQ_REGULAR_CHECK 			= 0x1F,
	APPMSG_RES_REGULAR_CHECK 			= 0x11,

	/**
	 * PreActivation request and response.
	 */
	APPMSG_REQ_PREACTIVATION_CHECK 		= 0x2F,
	APPMSG_RES_PREACTIVATION_CHECK 		= 0x21,

	/**
	 * Activation status request and response.
	 */
	APPMSG_REQ_ACTIVATION_STATUS 		= 0x3F,
	APPMSG_RES_ACTIVATION_STATUSACK 	= 0x31,
} appmsg_types_t;



typedef struct {
	device_types_t dev_type;
	uint8_t appver_major;
	uint8_t appver_minor;
	uint8_t appver_rev;
	appmsg_types_t reqmsg_type;
	uint8_t run_step;
} appmsg_request_t;

typedef struct {
	appmsg_types_t resmsg_type;
	uint8_t action_confimed;
	uint32_t next_trig_time;
} appmsg_response_t;


uint8_t appmsg_create_message(appmsg_request_t *pinput, uint8_t *poutbuf);
bool appmsg_parse_message(uint8_t *buffer, uint8_t buffersize, appmsg_response_t *poutput);


#ifdef __cplusplus
}
#endif


#endif /* APP_APP_MSGSTRUCTS_H_ */
