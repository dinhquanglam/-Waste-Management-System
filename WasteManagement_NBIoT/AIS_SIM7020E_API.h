#ifndef AIS_SIM7020E_API_h
#define AIS_SIM7020E_API_h


#include <Arduino.h>
#include "AT_SIM7020E.h"


class AIS_SIM7020E_API{
public:
	AIS_SIM7020E_API();
	bool debug;
    bool isMQTTs=false;

	void (*Event_debug)(char *data);	

	void begin(String serverdesport="", String addressI="");

	String getDeviceIP();
	String getSignal();
	String getIMSI();
	radio getRadioStat();
	pingRESP pingIP(String IP);
	dateTime getClock(unsigned int timezone=7);
	void powerSavingMode(unsigned int psm);
	bool checkPSMmode();
  void enableEDRX();
  void enablePSM();
  void disablePSM();
	bool NBstatus();
	bool MQTTstatus();

  void sendStr(String address, String port, String payload);
	void sendMsgHEX(String address,String desport,String payload);
	void sendMsgSTR(String address,String desport,String payload);
  String messagePayload();	

	void waitResponse(String &retdata,String server);
	
	bool newMQTTs(String server, String port);
	bool manageSSL(String rootCA,String clientCA, String clientPrivateKey);
	bool setPSK(String PSK);
	bool setPSKID(String PSKID);
	bool connectMQTT(String server,String port,String clientID,String username="",String password="");
	bool connectAdvanceMQTT(String server,String port,String clientID,String username,String password,int keepalive, int version,int cleansession, int willflag, String willOption);
	bool newMQTT(String server, String port);
	bool sendMQTTconnectionPacket(String clientID,String username,String password,int keepalive, int version,int cleansession, int willflag, String willOption);
	bool publish(String topic, String payload, unsigned int PubQoS=0, unsigned int PubRetained=0, unsigned int PubDup=0);
	bool subscribe(String topic, unsigned int SubQoS=0);
	void unsubscribe(String topic);
	void MQTTresponse();
	int setCallback(MQTTClientCallback callbackFunc);
	String willConfig(String will_topic, unsigned int will_qos,unsigned int will_retain,String will_msg);

	String toString(String dat);
    bool setCertificate(byte type, String CA);

private:
	bool flag_mqtt_connect=false;
	byte count_post_timeout=0;
	byte count_pub_timeout=0;
	byte count_sub_timeout=0;
	void send_msg(String address,String desport,unsigned int len,String payload);
	char char_to_byte(char c);	
	bool setupMQTT(String server,String port,String clientID,String username,String password,int keepalive, int version,int cleansession, int willflag, String willOption);
    void addNewline(String &str);
	
protected:
	 Stream *_Serial;	
};

#endif
