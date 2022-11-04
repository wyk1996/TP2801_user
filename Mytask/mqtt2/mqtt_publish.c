/*
 * mqtt_publish.c
 *
 *  Created on: 2019年2月15日
 *      Author: Snail
 */
#include "mqtt2.h"
#include "mqtt_publish.h"
#include "mqtt_packet.h"
#include "mqtt_subscribe.h"
#include <string.h>

int mqtt_serialize_publishlength(int qos, uint8_t *topicname, int payloadlen)
{
	int len = 0;

	len += 2 + strlen((char *)topicname) + payloadlen;

	if(qos > 0){
		len += 2;
	}
	return len;
}

int mqtt_serialize_publish(uint8_t *buf, int buflen, uint8_t dup,int qos, \
													uint8_t retained, uint16_t packetid, uint8_t *topicName, \
													uint8_t *payload, int payloadlen)
{
	uint8_t *ptr = buf;
	mqtt_head_t  header = {0};
	int rem_len = 0;
	int rc = 0;

	if(mqtt_packet_len(rem_len = mqtt_serialize_publishlength(qos, topicName, payloadlen)) > buflen){
		rc = MQTTPACKET_BUFFER_TOO_SHORT;
		goto exit;
	}

	header.bits.messageType = MQTT_PUBLISH;
	header.bits.dup_flag    = dup;
	header.bits.qos_level   = qos;
	header.bits.retain      = retained;

	*ptr++ = header.byte;	/*Write Header*/

	ptr += mqtt_packet_encode(ptr,rem_len); /*Write remaining length*/

	mqtt_writestring(&ptr,topicName);

	if(qos > 0){
		writeInt(&ptr,packetid);
	}

	memcpy(ptr,payload,payloadlen);
	ptr += payloadlen;

	rc = ptr - buf;

	exit:
	return rc;

}

int mqtt_serialize_ack(unsigned char *buf, int buflen, unsigned char packettype, unsigned char dup,
                      unsigned short packetid)
{
    mqtt_head_t header = {0};
    int rc = 0;
    unsigned char *ptr = buf;

    if (buflen < 4) {
        rc = MQTTPACKET_BUFFER_TOO_SHORT;
        goto exit;
    }
    header.bits.messageType = packettype;
    header.bits.dup_flag    = dup;
    header.bits.qos_level   = (packettype == MQTT_PUBREL) ? 1 : 0;
    writeChar(&ptr, header.byte);     /* write header */

    ptr += mqtt_packet_encode(ptr, 2); /* write remaining length */
    writeInt(&ptr, packetid);
    rc = ptr - buf;
exit:
    return rc;
}

int mqtt_serialize_puback(uint8_t *buf, int buflen, uint16_t packetid)
{
	return mqtt_serialize_ack(buf, buflen, MQTT_PUBACK, 0, packetid);
}


int mqtt_serialize_pubrel(uint8_t *buf, int buflen, uint16_t packetid)
{
	return mqtt_serialize_ack(buf, buflen, MQTT_PUBREL, 0, packetid);
}

int mqtt_serialize_pubcomp(uint8_t *buf, int buflen, uint16_t packetid)
{
	return mqtt_serialize_ack(buf, buflen, MQTT_PUBCOMP, 0, packetid);
}
/*mqtt 数据解析*/
int mqtt_deserialize_publish(uint8_t *dup,int *qos,uint8_t *retained,uint16_t *packetid,uint8_t *topicname,uint8_t **payload,int *payloadlen, uint8_t *buf, int buflen)
{
	mqtt_head_t header = {0};
	uint8_t *curdata = buf;
	uint8_t *enddata = NULL;
	int rc = 0;
	int ul_length = 0;

	header.byte = readChar(&curdata);
	if(header.bits.messageType != MQTT_PUBLISH)
	{
		goto exit;
	}

	*dup = header.bits.dup_flag;
	*qos = header.bits.qos_level;
	*retained = header.bits.retain;

	/*read remaining length 解析剩余长度*/
	curdata += (rc = mqtt_packet_decodebuf(curdata,&ul_length));
	enddata = curdata + ul_length;

	if(readMQTTLenString(topicname, &curdata, enddata) ||
        enddata - curdata < 0) { /* 判断是否有足够的缓存区读取协议和剩下来的数据 */
        goto exit;
    }
	if(*qos > 0){
		*packetid = readInt(&curdata);
	}
	*payloadlen = enddata - curdata;
	*payload = curdata;
	rc = 1;
	exit:
	return rc;

}

/*mqtt应答解析解析*/
int mqtt_deserialize_ack(uint8_t *packettype, uint8_t *dup, uint16_t *packetid, uint8_t *buf, int buflen)
{
	mqtt_head_t header = {0};
	uint8_t *curdata = buf;
	uint8_t *enddata = NULL;
	int rc = 0;
	int ul_length ;

	header.byte = readChar(&curdata);
	*dup = header.bits.dup_flag;
	*packettype = header.bits.messageType;

	curdata += (rc = mqtt_packet_decodebuf(curdata, &ul_length));
	enddata = curdata + ul_length;

	if(enddata - curdata < 2){
		goto exit;
	}
	*packetid = readInt(&curdata);

	rc = 1;

	exit:
	return rc;

}

