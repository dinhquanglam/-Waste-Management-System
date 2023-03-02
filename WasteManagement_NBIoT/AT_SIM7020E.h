#include <Arduino.h>
#include <Stream.h>

struct pingRESP{
	bool status;
	String addr;
	String ttl;
	String rtt;
};

struct radio{
  String earfcn = "";
  String earfcnOffset = "";
	String pci = "";
  String cellid = "";
	String rsrp = "";
	String rsrq = "";
  String rssi = "";
	String snr = "";
  String band = "";
  String tac = "";
  String ecl = "";
  String txPwr = "";
  String reRsrp = ""; 
};

struct dateTime{
	String date = "";
	String time = "";
};

typedef void (*MQTTClientCallback)(String &topic, String &payload, String &QoS, String &retained);
typedef void (*reponseCallback)(String &datax);

class AT_SIM7020E{
public:
	AT_SIM7020E();
	bool debug;	
	//--------- Parameter config ---------------
	const unsigned int msgLenMul = 2;
	const unsigned int mqttCmdTimeout = 1200;
	const unsigned int mqttBuffSize = 1024;
	//=========Initialization Module=======
	void setupModule(String port = "", String address = "");
	void sendAT();	
  void rebootAT();
	void rebootModule();
	pingRESP pingIP(String IP);
	bool closeUDPSocket();
	bool NBstatus();
	bool attachNetwork();
	void powerSavingMode(unsigned int psm);
	void syncLocalTime();
  void slowClockConfigure();
  void psmSetting();

	//==========Get Parameter Value=========
	String getFirmwareVersion();
	String getIMEI();
	String getICCID();
	String getIMSI();
	String getDeviceIP();
	String getSignal();    
	String getAPN();
	String getNetworkStatus();
  bool getNetworkRegStatus();
	radio getRadioStat();
	bool checkPSMmode();
  void eDRXSetting();
	bool MQTTstatus();
	dateTime getClock(unsigned int timezone);
	//==========Data send/rec.===============
	void waitResponse(String &retdata,String server);
  void sendData(String address, String port, unsigned int len, String payload);
	void _Serial_print(String address,String port,unsigned int len);
	void _Serial_print(String input);
	void _Serial_print(unsigned int data);
	void _Serial_print(char*);
	void _Serial_println();
	//===============Utility=================
	void _serial_flush();	
	//================MQTT===================
	void disconnectMQTT();
	bool newMQTT(String server, String port);
	bool sendMQTTconnectionPacket(String clientID,String username,String password,int keepalive, int version,int cleansession, int willflag, String willOption);
	void publish(String topic, String payload, unsigned int qos, unsigned int retained, unsigned int dup);
	bool subscribe(String topic, unsigned int qos);
	void unsubscribe(String topic);
	unsigned int MQTTresponse();
	String retTopic;
  	String retPayload;
  	String retQoS;
  	String retRetained;
  	int setCallback(MQTTClientCallback callbackFunc);
	bool newMQTTs(String server, String port);
  	bool setCertificate(byte cerType,int cerLength,byte isEnd,String CA);
  	bool checkCertificate(int r_len,int c_len,int p_len);
  	//============ callback ==================
	reponseCallback callback_p;
	MQTTClientCallback MQcallback_p;
    
private:
	//==============Buffer====================
	String dataInput;
	String data_buffer;
	//==============Flag======================
	bool hwConnected = false;
	bool end = false;
	//==============Parameter=================
	unsigned int previousCheck = 0;
  String fw = "";
  String imei = "";
  String iccid = "";
	//============Counter value===============
	byte k=0;
	//==============Function==================
	void echoOff();
  void getInfoInit();
  void selectOperator();
  void setOperationBand();
  void setAPN();
  void lockNetworkParam();
  void cIoTConfigure();
  void releaseConfigure();
  void deConfigure();
  bool checkPhoneFunc();
  void resetPhoneFunction();
  void checkRFFuncBlock();
  void printRadioStat(radio value);



  bool enterPIN();
	void connectNetwork();
  void setReceiveFlag(String type);
  void setTCPSendFlag(bool type);
	bool createUDPSocket(String address,String port);
  bool getSocketStatus();
	void manageResponse(String &retdata,String server);
	void printHEX(char *str);
	void blankChk(String& val);

protected:
	Stream *_Serial;	
};
