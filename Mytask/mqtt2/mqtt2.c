/*
 * mqtt.c
 *
 *  Created on: 2019年2月15日
 *      Author: Snail
 */
 #include "main.h"
#include "mqtt2.h"
#include "mqtt_publish.h"
#include "mqtt_packet.h"
#include "mqtt_subscribe.h"



uint16_t mqtt_send(mqtt_t *mqtt, uint8_t *tx_buff, uint8_t *topic, uint8_t *buf)
{
	uint16_t len = 0;

	len = strlen((char *)tx_buff) + strlen((char *)topic) + 2 + 5;

	if(buf == NULL)
	{
		return 0;
	}
	len = mqtt_serialize_publish(buf, len, 0, 1, 0, ++mqtt->packetid, topic, tx_buff, strlen((char *)tx_buff));

	/* 定时器单独做 用结构体函数形式？？ */
	mqtt->client_flag = CLNT_PUBLISH;
	return len;
}

uint8_t mqtt_recv(mqtt_t *mqtt, uint8_t *buf, uint16_t rx_size)
{
	uint8_t packet_type, dup, session_present, connack_rc, res = 1;
	uint16_t packet_id;

	if((buf[0] & 0x30) == 0x30)
	{
		uint8_t *set_buf = buf;
		while(*set_buf++ != '{' && rx_size--)
		{
		
		}
		//mqtt_set_param(set_buf);
	}
	if(mqtt->client_flag == CLNT_CONN)
	{
		if(mqtt_connack(&session_present, &connack_rc, buf, rx_size) == 1 && \
		   session_present == 0 &&
		   connack_rc == 0)
		{
			Trace_log("无线MQTT登录成功\r\n");
			mqtt->client_flag = CLNT_IDLE;
			task_post_event_wireless(MQTT_SUB);
			res = 0;
		}
	}
	else if(mqtt->client_flag == CLNT_PUBLISH)
	{
    	if(mqtt_deserialize_ack(&packet_type, &dup, &packet_id, buf, (int)rx_size) == 1 && 
		     MQTT_PUBACK ==  packet_type &&
		     mqtt->packetid == packet_id)
		{
			/*停止定时器*/
			Trace_log("无线MQTT发布成功\r\n");
			mqtt->client_flag = CLNT_IDLE;
			mqtt->dup_flag = 0;
			res = 0;
		}
	}
	else if(mqtt->client_flag == CLNT_SUBSCRIBE)
	{
		Trace_log("订阅成功");
			mqtt->client_flag = CLNT_IDLE;
			mqtt->dup_flag = 0;
			res = 0;
	}
	else 
	{
		if(buf[0] == 0XD0)
		{
			mqtt->client_flag = CLNT_IDLE;
			res = 0;
		}
	}
	return res;
}

void mqtt_init_connser(conn_pack_t *options, uint8_t *mqtt_user_name, uint8_t *mqtt_password, uint8_t *cliedt_id)
{
	options->mqtt_versionl = 4;
	options->willFlag = 0;
	options->keepAliveInterval = 120;
	
	
	if(strlen((char *)mqtt_user_name))
	{
		options->username = (char *)mqtt_user_name;
		options->password = (char *)mqtt_password;
		options->usernameflag = 1;
		options->passwordflag = 1;
	}
	options->clean_sesion = 1;
	options->clientId = (char *)cliedt_id;
}

int mqtt_connser(mqtt_t *mqtt,uint8_t *buf,conn_pack_t *options)
{	
	uint8_t *ptr = buf ;
	mqtt_head_t        conn_head = {0};
	mqtt_connectflags  flag = {0};
	int rc = -1;
	int len;
	
	conn_head.byte = 0;
	conn_head.bits.messageType = MQTT_CONNECT;
	
	len = mqtt_conlength(options);
	
	writeChar(&ptr, conn_head.byte);
	
	/*写入conn数据长度*/
	ptr += mqtt_packet_encode(ptr, len);
	
	if(options->mqtt_versionl == 4)
	{
		mqtt_writestring(&ptr,(uint8_t *)"MQTT");
		*ptr++ = 4;
	}
	else 
	{
		mqtt_writestring(&ptr,(uint8_t *)"MQisdp");
		*ptr++ = 3;
	}
	
	flag.connflag = 0;
	
	flag.bits.clean_sesion = options->clean_sesion;
	flag.bits.will_flag = (options->willFlag) ? 1 : 0;
	
	if(flag.bits.will_flag)
	{
		flag.bits.will_qos = options->will.qos;
		flag.bits.will_retain = options->will.retained;
	}
	
	if(options->usernameflag > 0)
	{
		flag.bits.username_flag = 1;
	}
	if(options->passwordflag > 0)
	{
		flag.bits.password_flag = 1;
	}
	*ptr++ = flag.connflag;
	
	*ptr++ = options->keepAliveInterval >> 8;
	*ptr++ = options->keepAliveInterval;
	
	mqtt_writestring(&ptr,(uint8_t *)options->clientId);
	
	if(options->willFlag)
	{
		mqtt_writestring(&ptr,(uint8_t *)options->will.topicname);
		mqtt_writestring(&ptr,(uint8_t *)options->will.message);
	}
	if(flag.bits.username_flag)
	{
		mqtt_writestring(&ptr,(uint8_t *)options->username);
	}
	if(flag.bits.password_flag)
	{
		mqtt_writestring(&ptr,(uint8_t *)options->password);
	}
	mqtt->client_flag = CLNT_CONN;
	rc = ptr - buf;
	return rc;
} 

int mqtt_conlength(conn_pack_t *options)
{
	int mqtt_packlen = 0;
	
	if(options->mqtt_versionl == 3)
	{
		mqtt_packlen = 12;    
	}
	else
	{
		mqtt_packlen = 10;
	}
	
	mqtt_packlen += strlen(options->clientId) + 2;
	
	if(options->willFlag)
	{
		mqtt_packlen += strlen(options->will.topicname) + 2 + strlen(options->will.message) + 2;
	}
	if(options->usernameflag)
	{
		mqtt_packlen += strlen(options->username) + 2;
	}
	if(options->passwordflag)
	{
		mqtt_packlen += strlen(options->password) + 2;
	}
	return mqtt_packlen;
}

/**
  * @brief  mqtt连接应答
  * @param  *mqtt 指向当前mqtt对象的指针
  * @param  *buf  缓冲区
  * @retval None
  */
int mqtt_connack(uint8_t *sessionPresent,uint8_t *connack_rc,uint8_t *buf,int buflen)
{
	mqtt_head_t        header = {0};
	int rc = 0;
	int mylen;
	uint8_t *curdata = buf;
	uint8_t *enddata  = NULL;
	
	mqtt_connack_t flags = {0};
	
	header.byte = *curdata;
	curdata ++;
	
	if(header.bits.messageType != MQTT_CONNACK)
	{
		goto exit;
	}
	
	/*获取数据长度,并且把指针偏移到数据位*/
	curdata += (rc = mqtt_packet_decodebuf(curdata,&mylen));
	
	enddata = (curdata + mylen);
	
	if (enddata - curdata < 2) 
	{
		goto exit;
	}
	
	/*Get Session Present 和连接返回码*/
	flags.all = *curdata++;
	*sessionPresent = flags.bits.sessionpresent;
	*connack_rc = *curdata;
	
	rc = 1;
	
	exit:
	return rc;
}
/**
  * 将断开连接的数据包序列化到提供的缓冲区中，准备写入套接字
  * @param 将数据包序列化到其中的缓冲区
  * @param buflen提供的缓冲区的字节长度，以避免溢出
  * @return serialized length, or error if 0
  */
int mqtt_disc_conn(uint8_t *buf,int buflen)
{
	buf[0] = MQTT_DISCONNECT << 4;
	buf[1] = 0;
	return 2;
}
/**
  * @brief  mqtt心跳请求
  * @param  *mqtt 指向当前mqtt对象的指针
  * @param  *buf  缓冲区
  * @retval None
  */
int mqtt_ping_req(mqtt_t *mqtt, uint8_t *buf)
{
	buf[0] = MQTT_PINGREQ << 4;
	buf[1] = 0;
	mqtt->client_flag = CLNT_PINGREQ;
	return 2;	
}
