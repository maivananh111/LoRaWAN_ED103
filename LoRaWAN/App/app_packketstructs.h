/*
 * app_packketstructs.h
 *
 *  Created on: Oct 20, 2024
 *      Author: anh
 */

#ifndef APP_APP_PACKKETSTRUCTS_H_
#define APP_APP_PACKKETSTRUCTS_H_

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
	APPPACK_REQ_BOOT_UP 				= 0xFF,
	APPPACK_RES_BOOT_UP 				= 0xF1,

	/**
	 * Regular check message request and response.
	 */
	APPPACK_REQ_REGULAR_CHECK 			= 0x1F,
	APPPACK_RES_REGULAR_CHECK 			= 0x11,

	/**
	 * PreActivation request and response.
	 */
	APPPACK_REQ_PREACTIVATION_CHECK 	= 0x2F,
	APPPACK_RES_PREACTIVATION_CHECK 	= 0x21,

	/**
	 * Activation status request and response.
	 */
	APPPACK_REQ_ACTIVATION_STATUS 		= 0x3F,
	APPPACK_REQ_ACTIVATION_STATUSACK 	= 0x31,
} apppack_types_t;



typedef struct {
	device_types_t devtype;
	uint8_t appver_major;
	uint8_t appver_minor;
	uint8_t appver_rev;
} apppack_req_bootup_t;

typedef struct {
	uint32_t next_trig_time;
} apppack_res_bootup_t;



typedef struct {
	device_types_t devtype;
} apppack_req_regularcheck_t;

typedef struct {
	uint32_t next_trig_time;
} apppack_res_regularcheck_t;



typedef struct {
	device_types_t devtype;
} apppack_req_preactivation_t;

typedef struct {
	uint8_t action_confimed;
	uint32_t next_trig_time;
} apppack_res_preactivation_t;



typedef struct {
	device_types_t devtype;
	uint8_t runstep;
} apppack_req_activation_t;

typedef struct {
	uint32_t next_trig_time;
} apppack_res_activation_t;






uint8_t apppack_req_bootup_create(apppack_req_bootup_t *pack, uint8_t *poutbuf);
bool apppack_res_bootup_parse(uint8_t *pinbuf, uint8_t pinbufsize, apppack_res_bootup_t *pout);


uint8_t apppack_req_regularchecks_create(apppack_req_regularcheck_t *pack, uint8_t *pout);
bool apppack_res_regularchecks_parse(uint8_t *pinbuf, uint8_t pinbufsize, apppack_res_regularcheck_t *pout);


uint8_t apppack_req_preactivation_create(apppack_req_preactivation_t *pack, uint8_t *pout);
bool apppack_res_preactivation_parse(uint8_t *pinbuf, uint8_t pinbufsize, apppack_res_preactivation_t *pout);


uint8_t apppack_req_activation_create(apppack_req_activation_t *pack, uint8_t *pout);
bool apppack_res_activation_parse(uint8_t *pinbuf, uint8_t pinbufsize, apppack_res_activation_t *pout);


#ifdef __cplusplus
}
#endif


#endif /* APP_APP_PACKKETSTRUCTS_H_ */
