#include "mqtt.h"
#include <string.h>
#include "MQTTPacket.h"
#include "StackTrace.h"
#include "MQTTPublish.h"
#include "MQTTSubscribe.h"
#include "Includes.h"

 uint16_t mqtt_send(mqtt_t *mqtt, uint8_t *tx_buff, MQTTString topic, uint8_t *buf)
{
	uint16_t len = 0;

	len = strlen((char *)tx_buff) + strlen((char *)topic.cstring) + 2 + 5;

	if(buf == NULL)
	{
		return 0;
	}
	len = MQTTSerialize_publish(buf, len, 0, 1, 0, ++mqtt->packetid, topic, tx_buff, strlen((char *)tx_buff));

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
			trace_log("无线MQTT发布成功\r\n");
			mqtt->client_flag = CLNT_IDLE;
			mqtt->dup_flag = 0;
			res = 0;
		}
	}
	else if(mqtt->client_flag == CLNT_SUBSCRIBE)
	{
		trace_log("订阅成功");
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


