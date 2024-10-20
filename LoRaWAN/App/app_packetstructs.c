/*
 * app_packetstructs.c
 *
 *  Created on: Oct 20, 2024
 *      Author: anh
 */

#include "app_packketstructs.h"
#include "stdlib.h"



uint8_t apppack_req_bootup_create(apppack_req_bootup_t *pack, uint8_t *poutbuf){
	poutbuf[0] = (uint8_t)APPPACK_REQ_BOOT_UP;
	poutbuf[1] = (uint8_t)pack->devtype;
	poutbuf[2] = pack->appver_major;
	poutbuf[3] = pack->appver_minor;
	poutbuf[4] = pack->appver_rev;

	return 5;
}

bool apppack_res_bootup_parse(uint8_t *pinbuf, uint8_t pinbufsize, apppack_res_bootup_t *pout){
	if( pinbufsize != 5 || pinbuf == NULL) return false;
	if(pinbuf[0] != APPPACK_RES_BOOT_UP) return false;
	pout->next_trig_time = (pinbuf[1] << 24)|(pinbuf[2] << 16)|(pinbuf[3] << 8)|(pinbuf[4] << 0);

	return true;
}






uint8_t apppack_req_regularchecks_create(apppack_req_regularcheck_t *pack, uint8_t *pout){
	pout[0] = APPPACK_REQ_REGULAR_CHECK;
	pout[1] = (uint8_t)pack->devtype;

	return 2;
}

bool apppack_res_regularchecks_parse(uint8_t *pinbuf, uint8_t pinbufsize, apppack_res_regularcheck_t *pout){
	if( pinbufsize != 5 || pinbuf == NULL) return false;
	if(pinbuf[0] != APPPACK_RES_REGULAR_CHECK) return false;
	pout->next_trig_time = (pinbuf[1] << 24)|(pinbuf[2] << 16)|(pinbuf[3] << 8)|(pinbuf[4] << 0);

	return true;
}






uint8_t apppack_req_preactivation_create(apppack_req_preactivation_t *pack, uint8_t *pout){
	pout[0] = APPPACK_REQ_PREACTIVATION_CHECK;
	pout[1] = (uint8_t)pack->devtype;

	return 2;
}

bool apppack_res_preactivation_parse(uint8_t *pinbuf, uint8_t pinbufsize, apppack_res_preactivation_t *pout){
	if( pinbufsize != 6 || pinbuf == NULL) return false;
	if(pinbuf[0] != APPPACK_RES_PREACTIVATION_CHECK) return false;
	pout->action_confimed = pinbuf[1];
	pout->next_trig_time = (pinbuf[2] << 24)|(pinbuf[3] << 16)|(pinbuf[4] << 8)|(pinbuf[5] << 0);

	return true;
}






uint8_t apppack_req_activation_create(apppack_req_activation_t *pack, uint8_t *pout){
	pout[0] = APPPACK_REQ_PREACTIVATION_CHECK;
	pout[1] = (uint8_t)pack->devtype;
	pout[2] = (uint8_t)pack->runstep;

	return 3;
}

bool apppack_res_activation_parse(uint8_t *pinbuf, uint8_t pinbufsize, apppack_res_activation_t *pout){
	if( pinbufsize != 6 || pinbuf == NULL) return false;
	if(pinbuf[0] != APPPACK_RES_PREACTIVATION_CHECK) return false;
	pout->next_trig_time = (pinbuf[1] << 24)|(pinbuf[2] << 16)|(pinbuf[3] << 8)|(pinbuf[4] << 0);

	return true;
}





