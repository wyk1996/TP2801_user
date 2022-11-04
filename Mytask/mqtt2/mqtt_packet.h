/*
 * mqtt_packet.h
 *
 *  Created on: 2019��2��15��
 *      Author: Snail
 */

#ifndef MQTT_PACKET_H_
#define MQTT_PACKET_H_

#include "mqtt2.h"
#include "mqtt_publish.h"
#include "mqtt_packet.h"
#include "mqtt_subscribe.h"
#include <string.h>

int mqtt_packet_encode(uint8_t *buf, int length);


/**
 * ������Ϣ���ȣ�����MQTT�㷨
 * @param getcharfn ָ�������Դ��ȡ��һ���ַ��ĺ���ָ�롣
 * @param value     ���볤��
 * @return          ������ֵ�ĳ���
 */
int mqtt_packet_decode(int (*getcharfn)(uint8_t *, int), int *value);

int mqtt_packet_len(int rem_len);

int bufchar(unsigned char *c, int count);

int mqtt_packet_decodebuf(unsigned char *buf, int *value);


/**
 * �ӽ��ջ�������ȡ�������ֽڼ���һ��������
 * @param  pptr ���ܻ�����
 * @return ����õ�������
 */
int readInt(unsigned char **pptr);


/**
 * �ӽ��ջ�������ȡһ���ַ�
 * @param  pptr
 * @return
 */
char readChar(unsigned char **pptr);


/**
 * д��һ���ַ�������������ָ���Լ�
 * @param  pptr ��д��Ļ�����
 * @param  c    ��д����ַ�
 */
void writeChar(unsigned char **pptr, char c);


/**
 * ������д�뵽������������ָ���Լӣ�ʹ�������ַ�������
 * @param pptr
 * @param anInt
 */
void writeInt(unsigned char **pptr, int anInt);


/**
 * Writes a "UTF" string to an output buffer.  Converts C string to length-delimited.
 * @param pptr pointer to the output buffer - incremented by the number of bytes used & returned
 * @param string the C string to write
 */
void mqtt_writestring(uint8_t **pptr, uint8_t *string);


int getLenStringLen(char *ptr);


void write_mqttstring(uint8_t **pptr, uint8_t *mqttstring);

/**
 * �ж������Ƿ�������
 * @param mqttstring the MQTTString structure into which the data is to be read
 * @param pptr pointer to the output buffer - incremented by the number of bytes used & returned
 * @param enddata pointer to the end of the data: do not read beyond
 * @return 1 if successful, 0 if not
 */
int readMQTTLenString(uint8_t *mqttstring, uint8_t **pptr, uint8_t *enddata);

/**
 * Helper function to read packet data from some source into a buffer
 * @param buf the buffer into which the packet will be serialized
 * @param buflen the length in bytes of the supplied buffer
 * @param getfn pointer to a function which will read any number of bytes from the needed source
 * @return integer MQTT packet type, or -1 on error
 * @note  the whole message must fit into the caller's buffer
 */
int mqtt_packet__read(unsigned char *buf, int buflen, int (*getfn)(unsigned char *, int));


#endif /* MQTT_PACKET_H_ */
