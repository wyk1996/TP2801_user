/*
 * mqtt_subscribe.c
 *
 *  Created on: 2019Äê2ÔÂ15ÈÕ
 *      Author: Snail
 */
#include "mqtt_subscribe.h"
#include <string.h>
#include "mqtt2.h"
#include <stdio.h>
/**
  * get payload length
  * @param count  topic num
  * @param topicfilters[] topic buffer
  * @return payload length
  */
int mqtt_subscribe_length(int count ,uint8_t* topicfilters[])
{
	int i;

	int len = 2;	/*add packetid byte*/

	for(i = 0;i < count;i++)
	{
		len += 2 + strlen((char *)topicfilters[i]) + 1; /*length + topic + req_qos*/
	}
	return len;
}

int mqtt_unsubscribe_length(int count, uint8_t* topicFilters[])
{
    int i;
    int len = 2; /* packetid */

    for (i = 0; i < count; ++i)
  	{
        len += 2 + strlen((char *)topicFilters[i]);    /* length + topic*/
    }
    return len;
}

int mqtt_subscribe_serialize(unsigned char *buf, int buflen, unsigned char dup, unsigned short packetid, int count,
                            uint8_t* topicFilters[], int requestedQoSs[])
{
    unsigned char *ptr = buf;
    mqtt_head_t header = {0};
    int rem_len = 0;
    int rc = 0;
    int i = 0;

    if (mqtt_packet_len(rem_len = mqtt_subscribe_length(count, topicFilters)) > buflen) {
        rc = MQTTPACKET_BUFFER_TOO_SHORT;
        goto exit;
    }

    header.byte = 0;
    header.bits.messageType = MQTT_SUBSCRIBE;
    header.bits.dup_flag    = dup;
    header.bits.qos_level   = 1;
    writeChar(&ptr, header.byte); /* write header */

    ptr += mqtt_packet_encode(ptr, rem_len); /* write remaining length */;

    writeInt(&ptr, packetid);

    for (i = 0; i < count; ++i)
		{
        mqtt_writestring(&ptr, topicFilters[i]);
        writeChar(&ptr, requestedQoSs[i]);
    }

    rc = ptr - buf;
exit:
    return rc;
}

int mqtt_suback_serialize(unsigned short *packetid, int maxcount, int *count, int grantedQoSs[], unsigned char *buf,
                           int buflen)
{
    mqtt_head_t header = {0};
    unsigned char *curdata = buf;
    unsigned char *enddata = NULL;
    int rc = 0;
    int mylen;

    header.byte = readChar(&curdata);
    if (header.bits.messageType != MQTT_SUBACK)
		{
        goto exit;
    }

    curdata += (rc = mqtt_packet_decodebuf(curdata, &mylen)); /* read remaining length */
    enddata = curdata + mylen;

    if (enddata - curdata < 2)
		{
        goto exit;
    }

    *packetid = readInt(&curdata);

    *count = 0;
    while (curdata < enddata) {
        if (*count > maxcount) {
            rc = -1;
            goto exit;
        }
        grantedQoSs[(*count)++] = readChar(&curdata);
    }

    rc = 1;
exit:
    return rc;
}

int mqtt_unsubscribe_serialize(unsigned char *buf, int buflen, unsigned char dup, unsigned short packetid, int count,
                            uint8_t* topicFilters[])
{
    unsigned char *ptr = buf;
    mqtt_head_t header = {0};
    int rem_len = 0;
    int rc = -1;
    int i = 0;

    if (mqtt_packet_len(rem_len = mqtt_unsubscribe_length(count, topicFilters)) > buflen) {
        rc = MQTTPACKET_BUFFER_TOO_SHORT;
        goto exit;
    }

    header.byte = 0;
    header.bits.messageType = MQTT_UNSUBSCRIBE;
    header.bits.dup_flag    = dup;
    header.bits.qos_level   = 1;
    writeChar(&ptr, header.byte); /* write header */

    ptr += mqtt_packet_encode(ptr, rem_len); /* write remaining length */;

    writeInt(&ptr, packetid);

    for (i = 0; i < count; ++i)
		{
        mqtt_writestring(&ptr, topicFilters[i]);
    }

    rc = ptr - buf;
exit:
    return rc;
}

int mqtt_unsuback_serialize(uint16_t *packetid, uint8_t *buf, int buflen )
{
	uint8_t type = 0;
	uint8_t dup  = 0;
	int rc = 0;

	rc = mqtt_deserialize_ack(&type, &dup, packetid, buf ,buflen);

	if(type == MQTT_UNSUBACK)
	{
		rc = 1;
	}
	return rc;
}
