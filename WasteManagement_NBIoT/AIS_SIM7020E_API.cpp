#include "AIS_SIM7020E_API.h"

AT_SIM7020E atcmd;
void event_null(char *data){}

/****************************************/
/**          Initialization            **/
/****************************************/

AIS_SIM7020E_API::AIS_SIM7020E_API(){
  Event_debug =  event_null;
}

void AIS_SIM7020E_API:: begin(String addressIP, String serverPort){
  atcmd.debug = debug;
  atcmd.setupModule(addressIP, serverPort);
}

pingRESP AIS_SIM7020E_API::pingIP(String IP){
  return atcmd.pingIP(IP);
}

void AIS_SIM7020E_API::enableEDRX(){
  atcmd.eDRXSetting();
}

void AIS_SIM7020E_API::enablePSM(){
  atcmd.slowClockConfigure();
  atcmd.psmSetting();
}

void AIS_SIM7020E_API::disablePSM(){
  atcmd.powerSavingMode(0);
}


/****************************************/
/**          Get Parameter Value       **/
/****************************************/
/*
  - getSignal
      - Get NB-IoT signal
  - getRadioStat
      - Get radio information for troubleshooting.
  - getDeviceIP
      - Get device ip after connected to network.
  - getIMSI
      - Get IMSI from eSim on board.         
  - powerSavingMode
      - Set powerSavingMode : 0 turn off, 1 turn on.  
  - checkPSMmode
      - Check if powerSavingMode is enable or not.
  - NBstatus
      - Check NB connecting status.
  - MQTTstatus     
      - Check MQTT connecting status.
*/
String AIS_SIM7020E_API::getSignal(){
  return atcmd.getSignal();
}

radio AIS_SIM7020E_API::getRadioStat(){
  return atcmd.getRadioStat();
}

String AIS_SIM7020E_API::getDeviceIP(){
  return atcmd.getDeviceIP();
}

String AIS_SIM7020E_API::getIMSI(){
  return atcmd.getIMSI();
}

void AIS_SIM7020E_API::powerSavingMode(unsigned int psm){
  atcmd.powerSavingMode(psm);
}

bool AIS_SIM7020E_API::checkPSMmode(){
  return atcmd.checkPSMmode();
}

bool AIS_SIM7020E_API::NBstatus(){
  return atcmd.NBstatus();
}

bool AIS_SIM7020E_API::MQTTstatus(){
  return atcmd.MQTTstatus();
}

dateTime AIS_SIM7020E_API::getClock(unsigned int timezone){
  return atcmd.getClock(timezone);
}

/****************************************/
/**              MQTT(s)               **/
/****************************************/
/*
  - setupMQTT
      - setup module to use MQTT include serverIP, port, clientID, username, password, keep alive interval, will messege.
  - connectMQTT
      - setup module to use MQTT include serverIP, port, clientID, username, password
  - connectAdvanceMQTT
      - setup module to use MQTT include serverIP, port, clientID, username, password, keep alive interval, will messege. This function doesn't have default value as 0.
  - newMQTT
      - connect device to MQTT server and port
  - sendMQTTconnectionPacket
      - connect device to MQTT with configuration value
  - willConfig
      - create payload for will messege
  - publish
      - publish payload within  1000 characters.
  - subscribe
      - subscribe to the topic to receive data
  - unsubscribe
      - unsubscribe to the topic
  - MQTTresponse
      - receive response from server
  - RegisMQCallback
      - receive response from server
*/
bool AIS_SIM7020E_API::setupMQTT(String server,String port,String clientID,String username,String password,int keepalive, int version,int cleansession, int willflag, String willOption){
  bool conStatus=false;
  if(username.length() > 100 || password.length() > 100){
    Serial.println(F("Username/Password is over 100."));
  }
  else if(username=="" && password!=""){
    Serial.println(F("Username is missing."));
  }
  else if(clientID.length() > 120 || clientID==""&&cleansession!=1){
    Serial.println(F("ClientID is over 120 or ClientID is missing."));
  }
  else if(server==""||port==""){
    Serial.println(F("Address or port is missing."));
  }
  else if(version > 4 || version < 3){
    Serial.println(F("Version must be 3 (MQTT 3.1) or 4 (MQTT 3.1.1)"));
  }
  else if(willflag==1&&willOption==""){
    Serial.println(F("Missing will option."));
  }
  else{

    if(MQTTstatus()) atcmd.disconnectMQTT();

    if(!isMQTTs){
      if(newMQTT(server, port)){
        if(atcmd.sendMQTTconnectionPacket(clientID,username,password,keepalive,version,cleansession,willflag,willOption)){
          flag_mqtt_connect=true;
          Serial.print(F("# ServerIP : "));
          Serial.println(server);
          Serial.print(F("# Port : "));
          Serial.println(port);
          Serial.print(F("# ClientID : "));
          Serial.println(clientID);
        }
        else {
          Serial.println(F("Please check your parameter again."));
        }
      }
      else {
        Serial.println(F("Please check your server/port."));
      }
    }
    else{
      if(newMQTTs(server, port)){
        if(atcmd.sendMQTTconnectionPacket(clientID,username,password,keepalive,version,cleansession,willflag,willOption)){
          flag_mqtt_connect=true;
          Serial.print(F("# ServerIP : "));
          Serial.println(server);
          Serial.print(F("# Port : "));
          Serial.println(port);
          Serial.print(F("# ClientID : "));
          Serial.println(clientID);
        }
        else {
          Serial.println(F("Please check your parameter  or certificates again."));
        }
      }
      else {
        Serial.println(F("Please check your server/port/Certificates again."));
      }
    } 
  }
  atcmd._serial_flush();
  return flag_mqtt_connect;
}

bool  AIS_SIM7020E_API::newMQTT(String server, String port){
  return atcmd.newMQTT(server, port);
}

bool  AIS_SIM7020E_API::newMQTTs(String server, String port){
  return atcmd.newMQTTs(server, port);
}

bool AIS_SIM7020E_API::connectMQTT(String server,String port,String clientID,String username,String password){
  return setupMQTT(server,port,clientID,username,password,60,3,1,0,"");
}

bool AIS_SIM7020E_API::connectAdvanceMQTT(String server,String port,String clientID,String username,String password,int keepalive, int version,int cleansession, int willflag, String willOption){
  return setupMQTT(server,port,clientID,username,password,keepalive,version,cleansession,willflag,willOption);
}

bool  AIS_SIM7020E_API::sendMQTTconnectionPacket(String clientID,String username,String password,int keepalive, int version,int cleansession, int willflag, String willOption){
  return atcmd.sendMQTTconnectionPacket(clientID,username,password,keepalive,version,cleansession,willflag,willOption);
}

String AIS_SIM7020E_API::willConfig(String will_topic, unsigned int will_qos,unsigned int will_retain,String will_msg){
  char data[will_msg.length()+1];
  memset(data,'\0',will_msg.length());
  will_msg.toCharArray(data,will_msg.length()+1);
  int len = will_msg.length()*atcmd.msgLenMul;
  String msg;

  char *hstr;
  hstr=data;
  char out[3];
  memset(out,'\0',2);
  bool flag=false;
  while(*hstr){
    flag=itoa((int)*hstr,out,16);    
    if(flag){
      msg+=out; 
    }
    hstr++;
  }
  return "\"topic="+will_topic+",QoS="+String(will_qos)+",retained="+String(will_retain)+",message_len="+String(len)+",message="+msg+"\"";
}

bool AIS_SIM7020E_API::publish(String topic, String payload, unsigned int pubQoS, unsigned int pubRetained, unsigned int pubDup){
  if(topic==""){
    Serial.println(F("Topic is missing."));
    return false;
  }
  if(payload.length()*atcmd.msgLenMul>1000){
    Serial.println(F("Payload hex string is over 1000."));
    return false;
  }

  // Serial.println(F("-------------------------------"));
  // Serial.print(F("# Publish : "));
  // Serial.println(payload);
  // Serial.print(F("# Topic   : "));
  // Serial.println(topic);

  atcmd.publish(topic, payload, pubQoS, pubRetained, pubDup);
  while(1){
  unsigned int respCode = atcmd.MQTTresponse();
    if(respCode==2){
      return true;
    }
    else if(respCode==3){
      return false;
    }
  }  
}

bool AIS_SIM7020E_API::subscribe(String topic, unsigned int subQoS){
  
  if(topic==""){
    Serial.println(F("Topic is missing."));
    return false;
  }

  Serial.println(F("-------------------------------"));
  Serial.println(F("# Subscribe "));
  Serial.print(F("# Topic : "));
  Serial.println(topic);  
  
  atcmd._serial_flush();
  atcmd.subscribe(topic, subQoS);
  byte count=0;
  while(1){
    delay(80);
    unsigned int respCode = atcmd.MQTTresponse();
    if(respCode==2){
      return true;
    }
    else if(respCode==3){
      if(count>2) return false;
      else{ 
        atcmd.subscribe(topic, subQoS);
        count++;
      }
    }
  }
  atcmd._serial_flush(); 
}

void AIS_SIM7020E_API::unsubscribe(String topic){
  if(topic==""){
    Serial.println(F("Topic is missing."));
  }
  else{
    atcmd.unsubscribe(topic);
  }
}

void AIS_SIM7020E_API::MQTTresponse(){
  atcmd.MQTTresponse();
}

int AIS_SIM7020E_API::setCallback(MQTTClientCallback callbackFunc){
  return atcmd.setCallback(callbackFunc);
}

bool AIS_SIM7020E_API::manageSSL(String rootCA,String clientCA, String clientPrivateKey){ 
  Serial.println("Certificate Setup. Please wait 1-2 minutes");
  isMQTTs=true;
  if(!atcmd.checkCertificate(rootCA.length()+2,clientCA.length()+2,clientPrivateKey.length()+2)){
    if(setCertificate(0,rootCA)){
      if(setCertificate(1,clientCA)){
        if(setCertificate(2,clientPrivateKey)){
          return true;
        }
      }
    }
    return false;
  }
  else return true;    
}

bool AIS_SIM7020E_API::setCertificate(byte type, String CA){ 
  addNewline(CA);
  int cerLength = CA.length();
  if(cerLength>2000){
    String CA_1 = CA.substring(0,1000);
    String CA_2 = CA.substring(1000,2000);
    String CA_3 = CA.substring(2000,cerLength);
    bool stat1 = atcmd.setCertificate(type,cerLength,0,CA_1);
    bool stat2 = atcmd.setCertificate(type,cerLength,0,CA_2);
    bool stat3 = atcmd.setCertificate(type,cerLength,1,CA_3);
    return stat1&&stat2&&stat3;
  }
  else if(cerLength>1000){
    String CA_1 = CA.substring(0,1000);
    String CA_2 = CA.substring(1000,cerLength);
    bool stat1 = atcmd.setCertificate(type,cerLength,0,CA_1);
    bool stat2 = atcmd.setCertificate(type,cerLength,1,CA_2);
    return stat1&&stat2;
  }
  else{
    String CA_1 = CA.substring(0,cerLength);
    bool stat1 = atcmd.setCertificate(type,cerLength,1,CA_1);
    return stat1;
  }
}

bool AIS_SIM7020E_API::setPSK(String PSK){
  return setCertificate(4, PSK);
}

bool AIS_SIM7020E_API::setPSKID(String PSKID){
  return setCertificate(3, PSKID);
}

/****************************************/
/**               Utility              **/
/****************************************/
/*
  - toString 
      - change hex to string
  - char_to_byte
      - use in function toString
  - addNewline
      - add \r\n in certificate, use in MQTTs
*/
String AIS_SIM7020E_API::toString(String dat){
  String str="";
  for(int x=0;x<dat.length();x+=2){
      char c =  char_to_byte(dat[x])<<4 | char_to_byte(dat[x+1]);
    str += c;
  }
  return(str);
}

char AIS_SIM7020E_API:: char_to_byte(char c){
  if((c>='0')&&(c<='9')){
    return(c-0x30);
  }
  if((c>='A')&&(c<='F')){
    return(c-55);
  }
}

void AIS_SIM7020E_API::addNewline(String &str){
  if(str.indexOf(F("\r\n"))==-1){
    byte index = str.indexOf(F("-----"));
    byte index2 = str.indexOf(F("-----"),index+1);
    str = str.substring(0,index2+5)+"\\r\\n"+str.substring(index2+5,str.length());
  }
}
