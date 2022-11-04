/*
 * mqtt_subscribe.h
 *
 *  Created on: 2019Äê2ÔÂ15ÈÕ
 *      Author: Snail
 */

#ifndef MQTT_SUBSCRIBE_H_
#define MQTT_SUBSCRIBE_H_

#include <stdint.h>

int mqtt_subscribe_serialize(unsigned char *buf, int buflen, unsigned char dup, unsigned short packetid, int count,
                            uint8_t* topicFilters[], int requestedQoSs[]);

#endif /* MQTT_SUBSCRIBE_H_ */
