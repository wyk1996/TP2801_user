/*
 * mqtt_publish.h
 *
 *  Created on: 2019年2月15日
 *      Author: Snail
 */

#ifndef MQTT_PUBLISH_H_
#define MQTT_PUBLISH_H_
#include "mqtt2.h"
#include "mqtt_publish.h"
#include "mqtt_packet.h"
#include "mqtt_subscribe.h"
#include <stdint.h>

int mqtt_serialize_publishlength(int qos, uint8_t *topicname, int payloadlen);

int mqtt_serialize_publish(uint8_t *buf, int buflen, uint8_t dup,int qos, \
													uint8_t retained, uint16_t packetid, uint8_t *topicName, \
													uint8_t *payload, int payloadlen);

int mqtt_serialize_ack(unsigned char *buf, int buflen, unsigned char packettype, unsigned char dup,
                      unsigned short packetid);

/*mqtt 数据解析*/
int mqtt_deserialize_publish(uint8_t *dup,int *qos,uint8_t *retained,uint16_t *packetid,uint8_t *topicname,uint8_t **payload,int *payloadlen, uint8_t *buf, int buflen);

/*mqtt应答解析解析*/
int mqtt_deserialize_ack(uint8_t *packettype, uint8_t *dup, uint16_t *packetid, uint8_t *buf, int buflen);


#endif /* MQTT_PUBLISH_H_ */
