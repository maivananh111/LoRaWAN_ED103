/*
 * app_msgstructs.c
 *
 *  Created on: Oct 20, 2024
 *      Author: anh
 */

#include "app_msgstructs.h"
#include "stdlib.h"



uint8_t appmsg_create_message(appmsg_request_t *pinput, uint8_t *poutbuf){
	poutbuf[0] = (uint8_t)pinput->reqmsg_type;
	poutbuf[1] = (uint8_t)pinput->dev_type;

	switch (pinput->reqmsg_type){
		case APPMSG_REQ_BOOT_UP:
			poutbuf[2] = pinput->appver_major;
			poutbuf[3] = pinput->appver_minor;
			poutbuf[4] = pinput->appver_rev;
			poutbuf[5] = (uint8_t)(pinput->regularcheck_freq >> 8);
			poutbuf[6] = (uint8_t)(pinput->regularcheck_freq & 0xFF);
			poutbuf[7] = pinput->battery_vol;
			return 8;

		case APPMSG_REQ_REGULAR_CHECK:
		case APPMSG_REQ_PREACTIVATION_CHECK:
			poutbuf[2] = pinput->battery_vol;
			return 3;

		case APPMSG_REQ_ACTIVATION_STATUS:
			if (pinput->run_step >= 5)
				poutbuf[2] = 0xDD;
			else
				poutbuf[2] = (uint8_t)pinput->run_step;
			poutbuf[3] = pinput->battery_vol;

			return 4;

		default:
		break;
	}

	return 0;
}

bool appmsg_parse_message(uint8_t *buffer, uint8_t buffersize, appmsg_response_t *poutput){
	if (buffersize == 0 || buffer == NULL)
		return false;

	poutput->resmsg_type = (appmsg_types_t)buffer[0];
	if (buffersize == 1 && poutput->resmsg_type == APPMSG_RES_ACTIVATION_STATUSACK){
		poutput->next_trig_time = 0;
	}
	else if (buffersize == 6 && poutput->resmsg_type == APPMSG_RES_PREACTIVATION_CHECK){
		poutput->action_confimed = buffer[1];
		poutput->next_trig_time = (buffer[2] << 24)|(buffer[3] << 16)|(buffer[4] << 8)|(buffer[5] << 0);
	}
	else if (buffersize == 5){
		poutput->next_trig_time = (buffer[1] << 24)|(buffer[2] << 16)|(buffer[3] << 8)|(buffer[4] << 0);
	}

	return true;
}





