/*
 * mqtt_packet.h
 *
 *  Created on: 2019年2月15日
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
 * 解码消息长度，根据MQTT算法
 * @param getcharfn 指向从数据源读取下一个字符的函数指针。
 * @param value     解码长度
 * @return          解码数值的长度
 */
int mqtt_packet_decode(int (*getcharfn)(uint8_t *, int), int *value);

int mqtt_packet_len(int rem_len);

int bufchar(unsigned char *c, int count);

int mqtt_packet_decodebuf(unsigned char *buf, int *value);


/**
 * 从接收缓冲区读取的两个字节计算一个整数。
 * @param  pptr 接受缓存区
 * @return 计算得到的整数
 */
int readInt(unsigned char **pptr);


/**
 * 从接收缓存区读取一个字符
 * @param  pptr
 * @return
 */
char readChar(unsigned char **pptr);


/**
 * 写入一个字符到缓存区并且指针自加
 * @param  pptr 待写入的缓存区
 * @param  c    待写入的字符
 */
void writeChar(unsigned char **pptr, char c);


/**
 * 把整形写入到缓存区，并且指针自加，使用两个字符缓存区
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
 * 判断数据是否接收完毕
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
