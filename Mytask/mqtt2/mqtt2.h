#ifndef _MQTT2_H_
#define _MQTT2_H_

#include <stdint.h>


#include "mqtt_publish.h"
#include "mqtt_packet.h"
#include "mqtt_subscribe.h"

typedef enum 
{
	MQTT_CONNECT    =    1,
	MQTT_CONNACK,
	MQTT_PUBLISH,
	MQTT_PUBACK,
	MQTT_PUBREC,
	MQTT_PUBREL,
	MQTT_PUBCOMP,
	MQTT_SUBSCRIBE,
	MQTT_SUBACK,
	MQTT_UNSUBSCRIBE,
	MQTT_UNSUBACK,
	MQTT_PINGREQ,
	MQTT_PINGRESP,
	MQTT_DISCONNECT,
}message_t;

typedef enum 
{
	CLNT_CONN    =    1,
	CLNT_PUBLISH,
	CLNT_SUBSCRIBE,
	CLNT_PINGREQ,
	CLNT_IDLE
}client_flag;

enum errors {
    MQTTPACKET_BUFFER_TOO_SHORT = -2,
    MQTTPACKET_READ_ERROR = -1,
    MQTTPACKET_READ_COMPLETE
};

/*连接状态*/
typedef union
{
	uint8_t connflag;
	struct 
	{		
		uint8_t :1;	
		uint8_t clean_sesion    :1;		
		uint8_t will_flag       :1;		
		uint8_t will_qos        :2;
		uint8_t will_retain     :1;
		uint8_t password_flag   :1;
		uint8_t username_flag   :1;
	}bits;
}mqtt_connectflags;

typedef union
{
	uint8_t all;
	struct
	{
		unsigned int : 7;                   /**< unused */
		unsigned int sessionpresent : 1;    /**< session present flag */
	}bits;
}mqtt_connack_t;

/*MQTT固定头*/
typedef union
{
	uint8_t byte;
	struct 
	{
		uint8_t retain         :1;
		uint8_t qos_level      :2;
		uint8_t dup_flag       :1;
		uint8_t messageType    :4;
	}bits;
}mqtt_head_t;

typedef struct
{
	char struct_id[4];
	
	int struct_version;
	char *topicname;
	char *message;
	uint8_t retained;
	uint8_t qos;
}will_options_t;

/*连接选项*/
typedef struct
{
	char struct_id[4];
	
	int struct_version;
	
	char *clientId;
	char *username;
	char *password;	
	
	uint8_t mqtt_versionl;
	uint16_t keepAliveInterval;    /*保持连接时间，[/S]*/
	uint8_t clean_sesion;
	uint8_t willFlag;
	uint8_t usernameflag;
	uint8_t passwordflag;
	will_options_t will;
}conn_pack_t ;

typedef struct 
{
	uint16_t packetid;
	uint8_t  dup_flag; 
	uint8_t  client_flag;
	uint8_t  message_type;
	uint8_t  sessionPresent;
}mqtt_t;

uint16_t mqtt_send(mqtt_t *mqtt, uint8_t *tx_buff, uint8_t *topic, uint8_t *buf);

uint8_t mqtt_recv(mqtt_t *mqtt, uint8_t *buf, uint16_t rx_size);

void mqtt_init_connser(conn_pack_t *options, uint8_t *user_name, uint8_t *mqtt_password, uint8_t *cliedt_id);

int mqtt_connser(mqtt_t *mqtt,uint8_t *buf,conn_pack_t *options);

int mqtt_conlength(conn_pack_t *options);

int mqtt_connack(uint8_t *sessionPresent,uint8_t *connack_rc,uint8_t *buf,int buflen);

int mqtt_disc_conn(uint8_t *buf,int buflen);

int mqtt_ping_req(mqtt_t *mqtt, uint8_t *buf);

#endif
