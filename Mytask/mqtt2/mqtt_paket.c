/*
 * mqtt_packet.c
 *
 *  Created on: 2019��2��15��
 *      Author: Snail
 */

#include "mqtt2.h"
#include "mqtt_publish.h"
#include "mqtt_packet.h"
#include "mqtt_subscribe.h"
#include <string.h>
/**
 * ��дʣ�೤�ȣ�������ռ���ֽ�
 * @param buf ��д���buf
 * @param length ʣ�����ݳ���
 * @return д��buffer���ֽ���
 */
int mqtt_packet_encode(uint8_t *buf, int length)
{
    int rc = 0;

    do {
        char d = length % 128;
        length /= 128;
        if (length > 0) {
            d |= 0x80;
        }
        buf[rc++] = d;
    } while (length > 0);
    return rc;
}


/**
 * ������Ϣ���ȣ�����MQTT�㷨
 * @param getcharfn ָ�������Դ��ȡ��һ���ַ��ĺ���ָ�롣
 * @param value     ���볤��
 * @return          ������ֵ�ĳ���
 */
int mqtt_packet_decode(int (*getcharfn)(uint8_t *, int), int *value)
{
    unsigned char c;
    int multiplier = 1;
    int len = 0;

    *value = 0;
    do {
        int rc = MQTTPACKET_READ_ERROR;

        if (++len > 4) {
            rc = MQTTPACKET_READ_ERROR; /* bad data */
            goto exit;
        }
        rc = (*getcharfn)(&c, 1);
        if (rc != 1) {
            goto exit;
        }
        *value += (c & 127) * multiplier;
        multiplier *= 128;
    } while ((c & 128) != 0);
exit:
    return len;
}


int mqtt_packet_len(int rem_len)
{
    rem_len += 1; /* header byte */

    /* now remaining_length field */
    if (rem_len < 128) {
        rem_len += 1;
    } else if (rem_len < 16384) {
        rem_len += 2;
    } else if (rem_len < 2097151) {
        rem_len += 3;
    } else {
        rem_len += 4;
    }
    return rem_len;
}


static unsigned char *bufptr;

int bufchar(unsigned char *c, int count)
{
    int i;

    for (i = 0; i < count; ++i) {
        *c = *bufptr++;
    }
    return count;
}


int mqtt_packet_decodebuf(unsigned char *buf, int *value)
{
    bufptr = buf;
    return mqtt_packet_decode(bufchar, value);
}


/**
 * �ӽ��ջ�������ȡ�������ֽڼ���һ��������
 * @param  pptr ���ܻ�����
 * @return ����õ�������
 */
int readInt(unsigned char **pptr)
{
    unsigned char *ptr = *pptr;
    int len = 256 * (*ptr) + (*(ptr + 1));
    *pptr += 2;
    return len;
}


/**
 * �ӽ��ջ�������ȡһ���ַ�
 * @param  pptr
 * @return
 */
char readChar(unsigned char **pptr)
{
    char c = **pptr;
    (*pptr)++;
    return c;
}


/**
 * д��һ���ַ�������������ָ���Լ�
 * @param  pptr ��д��Ļ�����
 * @param  c    ��д����ַ�
 */
void writeChar(unsigned char **pptr, char c)
{
    **pptr = c;
    (*pptr)++;
}


/**
 * ������д�뵽������������ָ���Լӣ�ʹ�������ַ�������
 * @param pptr
 * @param anInt
 */
void writeInt(unsigned char **pptr, int anInt)
{
    **pptr = (unsigned char)(anInt / 256);
    (*pptr)++;
    **pptr = (unsigned char)(anInt % 256);
    (*pptr)++;
}


/**
 * Writes a "UTF" string to an output buffer.  Converts C string to length-delimited.
 * @param pptr pointer to the output buffer - incremented by the number of bytes used & returned
 * @param string the C string to write
 */
void mqtt_writestring(uint8_t **pptr, uint8_t *string)
{
    int len = strlen((char *)string);
    writeInt(pptr, len);
    memcpy(*pptr, string, len);
    *pptr += len;
}


int getLenStringLen(char *ptr)
{
    int len = 256 * ((unsigned char)(*ptr)) + (unsigned char)(*(ptr + 1));
    return len;
}


void write_mqttstring(uint8_t **pptr, uint8_t *mqttstring)
{
	int len = strlen((char *)mqttstring);
    if (len > 0)
		{
        writeInt(pptr, len);
        memcpy(*pptr, mqttstring, len);
        *pptr += len;
    }
		else
		{
       writeInt(pptr, 0);
    }
}


/**
 * �ж������Ƿ�������
 * @param mqttstring the MQTTString structure into which the data is to be read
 * @param pptr pointer to the output buffer - incremented by the number of bytes used & returned
 * @param enddata pointer to the end of the data: do not read beyond
 * @return 1 if successful, 0 if not
 */
int readMQTTLenString(uint8_t *mqttstring, uint8_t **pptr, uint8_t *enddata)
{
    int rc = 0;
		int num = 0;

    if (enddata - (*pptr) > 1)
		{
        num = readInt(pptr);
        if (&(*pptr)[num] <= enddata)
				{
            mqttstring = *pptr;
            *pptr += num;
            rc = 1;
        }
    }
    return rc;
}

/**
 * Helper function to read packet data from some source into a buffer
 * @param buf the buffer into which the packet will be serialized
 * @param buflen the length in bytes of the supplied buffer
 * @param getfn pointer to a function which will read any number of bytes from the needed source
 * @return integer MQTT packet type, or -1 on error
 * @note  the whole message must fit into the caller's buffer
 */
int mqtt_packet__read(unsigned char *buf, int buflen, int (*getfn)(unsigned char *, int))
{
    int rc = -1;
    mqtt_head_t header = {0};
    int len = 0;
    int rem_len = 0;

    /* 1. read the header byte.  This has the packet type in it */
    if ((*getfn)(buf, 1) != 1) {
        goto exit;
    }

    len = 1;
    /* 2. read the remaining length.  This is variable in itself */
    mqtt_packet_decode(getfn, &rem_len);
    len += mqtt_packet_encode(buf + 1, rem_len); /* put the original remaining length back into the buffer */

    /* 3. read the rest of the buffer using a callback to supply the rest of the data */
    if ((rem_len + len) > buflen) {
        goto exit;
    }
    if ((*getfn)(buf + len, rem_len) != rem_len) {
        goto exit;
    }

    header.byte = buf[0];
    rc = header.bits.messageType;
exit:
    return rc;
}
