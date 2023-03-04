#include "AT_SIM7020E.h"  
#include "config.h"

/****************************************/
/**        Initialization Module       **/
/****************************************/
AT_SIM7020E::AT_SIM7020E(){}

void AT_SIM7020E::setupModule(String address, String port){
  previousCheck = millis();

  Serial.println(F("\n>>REBOOTING AT .")); 
  rebootAT();

  serialPort.begin(baudrate);
  _Serial = &serialPort;

  Serial.println(F(">>FIRST INITIALIZE MODULE ."));
  sendAT();
  if (!hwConnected){
    Serial.println(F("Hardware can not connect!"));
  }
  else {
    Serial.println(F("Hardware connected"));
  }
  echoOff();

  Serial.println(F(">>GET INFO SIM & MODULE ."));  
  getInfoInit();

  Serial.println(F(">>LOCK NETWORK PARAM .")); 
  lockNetworkParam();

  Serial.println(F(">>DEDICATED CONFIGURE FOR NB-IOT NETWORK"));
  deConfigure(); 

  Serial.println(F(">>CHECK RF FUNCTION BLOCK"));
  checkRFFuncBlock();

  Serial.println(F(">>SHOW NETWORK STATUS"));
  radio value = getRadioStat();
  printRadioStat(value);
/*
  Serial.println(F(">>CREATE UDP SOCKET AND CONNECT TO SERVER"));
  if (createUDPSocket(address, port) && getSocketStatus()){
    Serial.println("Create UDP socket and connect to server successfully");
  }
  else{
    Serial.println("Fail to create UDP socket");
  }
  */
}

/* 
 * Reboot module with hardware pin
 */
void AT_SIM7020E::rebootModule(){
    /*
    pinMode(hwResetPin, OUTPUT);
    digitalWrite(hwResetPin, LOW);
    delay(1000);
    digitalWrite(hwResetPin, HIGH);
    delay(2000);
    */
    Serial.println(F("Arduino UNO doesn't need to reboot"));  
}

/*
 * Reboot module AT
 */
void AT_SIM7020E::rebootAT(){
  pinMode(atRebootPin, OUTPUT);
  digitalWrite(atRebootPin, LOW);
  delay(1500);
  digitalWrite(atRebootPin, HIGH);
  Serial.println(F("Reboot AT module successfully"));
}

/*
 * Send AT cmd
 */
void AT_SIM7020E::sendAT(){
  int count = 0;
  
  _Serial->println(F("AT"));
  delay(100);
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        hwConnected = true;
        Serial.println(F("Send AT cmd successfully"));
        break;
      }
    }
    else{
      unsigned int currentCheck = millis();
      if (currentCheck - previousCheck > 5000){
        if (count > 3){
          Serial.print(F("\nError to connect NB Module, rebooting AT..."));
        }
        previousCheck = currentCheck;
        hwConnected = false;
        Serial.print(F("."));
        rebootAT();
        _Serial->println(F("AT"));
        delay(100);
        count++;
      }
      else{
        delay(500);
        _Serial->println(F("AT"));
        delay(100);
        Serial.print(F("."));
      }
    }
  }
  _serial_flush();
}

/*
 * Set command echo mode off
 */
void AT_SIM7020E::echoOff(){
  _Serial->println(F("ATE0"));
  while (1){
    if (_Serial->available()){
      dataInput=_Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        Serial.println(F("Set Echo off successfully"));
        break;
      }
    }
  }
  _serial_flush();
}

/* 
 * Request software release
 */
String AT_SIM7020E::getFirmwareVersion(){
  String fw = "";
  _Serial->println(F("AT+CGMR"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        break;
      }
      else{
        fw += dataInput;
      }
    }
  }
  
  fw.trim();
  blankChk(fw);
  _serial_flush();
  return fw;
}

/* 
 * Request product serial number identification
 */ 
String AT_SIM7020E::getIMEI(){
  String imei;
  _Serial->println(F("AT+CGSN"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        break;
      }
      else{
        imei += dataInput;
      }
    }
  }
  
  imei.trim();
  blankChk(imei);
  _serial_flush();
  return imei;
}

/* 
 * Show ICCID 
 */
String AT_SIM7020E::getICCID(){
  String iccid = "";
  _Serial->println(F("AT+ICCID"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        break;
      }
      else{
        iccid += dataInput;
      }
    }
  }

  iccid.trim();
  blankChk(iccid);
  _serial_flush();
  return iccid;
}

/*
 * Get some info at initialization, include fw version, imei, iccid
 */
void AT_SIM7020E::getInfoInit(){
  Serial.print(F(">>FW ver : "));
  fw = getFirmwareVersion();
  Serial.println(fw);
    
  Serial.print(F(">>IMEI   : "));
  imei = getIMEI();
  Serial.println(imei);
  
  Serial.print(F(">>ICCID  : "));
  iccid = getICCID();
  Serial.println(iccid);
}

/* 
 * Operator selection: Viettel: 45204
 */
void AT_SIM7020E::selectOperator(){
  _Serial->println(F("AT+COPS=1,2,\"45204\""));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        Serial.println(F("Set operator successfully"));
        break;
      }
    }
  }
  _serial_flush();
}

/* 
 * Set mobile operationn band: Vietnam: Band3 
 */
void AT_SIM7020E::setOperationBand(){
  _Serial->println(F("AT+CBAND=3"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1) {
        Serial.println(F("Set operation band successfully"));
        break;
      }
    }
  }
  _serial_flush();
}

/* 
 * Set APN: nbiot
 */
void AT_SIM7020E::setAPN(){
  _Serial->println(F("AT*MCGDEFCONT=\"IP\",\"nbiot\""));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        Serial.println(F("Set APN sucessfully"));
        break;
      }
    }
  }

  _Serial->println(F("AT*MCGDEFCONT?"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("*MCGDEFCONT")) != -1){
        break;
      }
    }
  }
  _serial_flush();
}

/*
 * Set some param of VN/VT Network
 */
void AT_SIM7020E::lockNetworkParam(){
  selectOperator();
  setOperationBand();
  setAPN();
}

/* 
 * Configure CIoT optimization 
 */
void AT_SIM7020E::cIoTConfigure(){
   _Serial->println(F("AT+CCIOTOPT=1,3,1"));
   while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        Serial.println(F("Configure CIoT optimization sucessfully"));
        break;
      }
    }
  }
  _serial_flush();
}

/*
 * Configure NB-IoT release indication
 */
void AT_SIM7020E::releaseConfigure(){
  _Serial->println(F("AT+CNBIOTRAI=1"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        Serial.println(F("Configure release indication sucessfully"));
        break;
      }
    }
  }
  _serial_flush();
}

/*
 * Dedicated configure for NB-IoT Network
 */
void AT_SIM7020E::deConfigure(){
  cIoTConfigure();
  releaseConfigure();
}

/*
 * Check Phone function whether it's full functionality
 */
bool AT_SIM7020E::checkPhoneFunc(){
  bool status = false;
  _Serial->println(F("AT+CFUN?"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("+CFUN:")) != -1){
        if (dataInput.indexOf(F("1")) != -1) {
          status = true;
          break;        
        }
      }
    }
  }
  _serial_flush();
  return status;
}

// Set Phone Functionality : 1 Full functionality, 0 Minimum functionality
void AT_SIM7020E::resetPhoneFunction(){
  _Serial->println(F("AT+CFUN=0"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        Serial.println(F("Set Phone Functionality to Minimum..."));
        break;
      }
    }
  }
  delay(5000);
  _Serial->println(F("AT+CFUN=1"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        Serial.println(F("Reset Phone Function successfully"));
        break;
      }
    }
  }
  _serial_flush();
}

/* 
 * Check if SIM/eSIM need PIN or not.
 */
bool AT_SIM7020E::enterPIN(){
  bool status = false;
  _Serial->println(F("AT+CPIN?"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("+CPIN:")) != -1){
        if (dataInput.indexOf(F("READY")) != -1){
          status = true;
          break;
        }
      }
    }
  }
  _serial_flush();
  return status;
}

void AT_SIM7020E::checkRFFuncBlock(){
  int count = 0;
  bool phoneFunc = checkPhoneFunc();
  bool pin = enterPIN();
  bool networkReg = getNetworkRegStatus();
  String csq = getSignal();
  
  Serial.print("Status Phone Function: ");
  Serial.println(phoneFunc);

  Serial.print("Status PIN: ");
  Serial.println(pin);
  
  Serial.print("Status Network Reg: ");
  Serial.println(networkReg);

  Serial.print("CSQ = ");
  Serial.println(csq);
  
  while (!phoneFunc || !pin || !networkReg || (csq == "") || (csq == "N/A")){
    count ++;
    if (count > 3){
      rebootAT();
      break;
    }
    resetPhoneFunction();
    Serial.print("Status Phone Function: ");
    Serial.println(checkPhoneFunc());

    Serial.print("Status PIN: ");
    Serial.println(enterPIN());
  
    Serial.print("Status Network Reg: ");
    Serial.println(getNetworkRegStatus());

    Serial.print("CSQ = ");
    Serial.println(getSignal()); 
  }
}


// eDRX setting
void AT_SIM7020E::eDRXSetting(){
  _Serial->println(F("AT+CEDRXS=2,5,\"0111\""));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readString();
      if (dataInput.indexOf(F("OK")) != -1){
        Serial.println(F("Setting eDRX sucessfully"));
        break;
      }
      else {
        Serial.println(F("Failed"));
        break;
      }
    }
  }
}

// Configure slow clock for PSM: enable automatically
void AT_SIM7020E::slowClockConfigure(){
  _Serial->println(F("AT+CSCLK=2"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        Serial.println(F("Configure slow clock sucessfully"));
        break;
      }
    }
  }
}

// Set powerSavingMode : 0 turn off, 1 turn on
void AT_SIM7020E::powerSavingMode(unsigned int psm){
  _Serial->println(F("AT+CPSMS="));
  _Serial->println(psm);
  _serial_flush();
  Serial.println(F("Set Power Saving Mode successfully. Now "));
  Serial.println(psm);
}

// Power Saving Mode Setting
void AT_SIM7020E::psmSetting(){
  _Serial->println(F("AT+CPSMS=1,,,\"10101010\",\"00100001\""));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readString();
      if (dataInput.indexOf(F("OK")) != -1){
        Serial.println(F("Setting psm sucessfully"));
        break;
      }
      else {
        Serial.println(F("Failed"));
        break;
      }
    }
  }
}



bool AT_SIM7020E::attachNetwork(){
  bool status = false;
  if (!NBstatus()){
    for(byte i = 0; i < 60; i++){
      resetPhoneFunction();
      connectNetwork();
      delay(1000);
      if (NBstatus()){ 
        status = true;
        break;
      }
      Serial.print(F("."));
    }
  }
  else status = true;
    
  _serial_flush();
  _Serial->flush();
  return status;
}

// Check network connecting status : 1 connected, 0 not connected
bool AT_SIM7020E::NBstatus(){
  bool status=false;
  _serial_flush();
  _Serial->println(F("AT+CGATT?"));
  delay(800);
  for(byte i=0;i<60;i++){
    if(_Serial->available()){
      dataInput=_Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("+CGATT: 1"))!=-1){
        status=true;
      }
      else if(dataInput.indexOf(F("+CGATT: 0"))!=-1){
        status=false;
      }
      else if(dataInput.indexOf(F("OK"))!=-1) {
        break;        
      }
      else if(dataInput.indexOf(F("ERROR"))!=-1) {
        break;        
      }
    }
  }
  dataInput="";
  return status;
}



// Attach network : 1 connected, 0 disconnected
void AT_SIM7020E::connectNetwork(){  
  _Serial->println(F("AT+CGATT=1"));
  for(int i=0;i<30;i++){
    if(_Serial->available()){
      dataInput =  _Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("OK"))!=-1) break;
      else if(dataInput.indexOf(F("ERROR"))!=-1) break;
    }
  }
  Serial.print(F("."));
}

/*
 * Set receive flag (hex/ string)
 */
void AT_SIM7020E::setReceiveFlag(String type){
  if (type == "hex"){
    _Serial->println(F("AT+CSORCVFLAG=0"));
  }
  else if (type == "string"){
    _Serial->println(F("AT+CSORCVFLAG=1"));
  }
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        Serial.print(F("Set receive flag successfully. Type is: "));
        Serial.println(type);
        break;
      }
    }
  }
}

/*
 * Set TCP send flag: 0 - disable, 1 - enable
 */
void AT_SIM7020E::setTCPSendFlag(bool type){
  if (!type){
    _Serial->println(F("AT+CSOSENDFLAG=0"));
  }
  else if (type){
    _Serial->println(F("AT+CSOSENDFLAG=1"));
  }
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        Serial.println(F("Set TCP send flag successfully"));
        break;
      }
    }
  }
}

/* 
 * Create a UDP socket
 */
bool AT_SIM7020E::createUDPSocket(String address, String port){
  bool status = false;
  _Serial->println(F("AT+CSOC=1,2,1"));
  delay(200);
  while (1){
    if (_Serial->available()){
      dataInput=_Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
      }
      else if (dataInput.indexOf(F("+CSOC: 0")) != -1){
        status = true;
        break;
      }
      else if (dataInput.indexOf(F("+CSOC: 1")) != -1){
        status = false;
        closeUDPSocket(); // Cause we just you one socket 0
        _Serial->println(F("AT+CSOC=1,2,1"));
      }
    }
  }

  if (status){
    _Serial->print(F("AT+CSOCON=0,"));
    _Serial->print(port);
    _Serial->print(F(","));
    _Serial->println(address);
    while (1){
      if (_Serial->available()){
        dataInput = _Serial->readStringUntil('\n');
        if (dataInput.indexOf(F("OK")) != -1){
          break;
        }
        else if (dataInput.indexOf(F("ERROR")) != -1) {
          status = false;
          break;
        }
      }
    }
  }
  return status;
}

/*
 * Get socket status
 */
bool AT_SIM7020E::getSocketStatus(){
  bool status = false;
  _Serial->println(F("AT+CSOSTATUS=0"));
  while (1){
    if (_Serial->available()){
      dataInput=_Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("+CSOSTATUS:")) != -1){
        if (dataInput.indexOf(F("0,2")) != -1){
          status = true;
          break;         
        }
        else{
          status = false;
          break;
        }
      }
    }
  }
  return status;  
}

/* 
 * Close a UDP socket 0
 */
bool AT_SIM7020E::closeUDPSocket(){
  _Serial->println(F("AT+CSOCL=0"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK")) != -1){
        break;
      }
    }
  }
}

/*
 * Ping IP
 */
pingRESP AT_SIM7020E::pingIP(String IP){
  pingRESP pingr;
  String data = "";
  int replytime = 0;
  int ttl = 0;
  _Serial->println("AT+CIPPING="+IP);
  Serial.println("Ping IP");
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("ERROR")) != -1){
        Serial.println("Ping IP fail");
        break;
      }
      else if (dataInput.indexOf(F("+CIPPING: ")) != -1){
        data = dataInput;

        byte index = data.indexOf(F(","));
        byte index2 = data.indexOf(F(","), index+1);
        byte index3 = data.indexOf(F(","), index2+1);
        
        pingr.addr = data.substring(index+1, index2);
        replytime += data.substring(index2+1, index3).toInt();

        ttl += data.substring(index3+1, data.length()).toInt();
      }
      if (dataInput.indexOf(F("+CIPPING: 4")) != -1) {
        break;
      }
    }
  }

  if (data != ""){
    pingr.ttl = String(ttl / 4);
    pingr.rtt = String((replytime / 4.0) * 100);
    blankChk(pingr.ttl);
    blankChk(pingr.rtt);
    Serial.println(">>Ping IP : " + pingr.addr + ", ttl = " + pingr.ttl + ", replyTime = " + pingr.rtt + "ms");
    pingr.status = true;
  } else{ 
    Serial.println(F(">>Ping Failed"));
    pingr.status = false;
  }
  _serial_flush();
  dataInput = "";
  return pingr;
}




/****************************************/
/**          Get Parameter Value       **/
/****************************************/
/* 
 * Get signal quality report 
 */
String AT_SIM7020E::getSignal(){
  _serial_flush();
  int rssi = 0;
  int count = 0;
  String data_csq = "";
  dataInput = "";
  do
  {
    _Serial->println(F("AT+CSQ"));
    delay(200);
    while(1)  {    
      if (_Serial->available()){
        dataInput = _Serial->readStringUntil('\n');
        if (dataInput.indexOf(F("OK")) != -1){
          break;
        }
        else {
          if (dataInput.indexOf(F("+CSQ")) != -1){
            byte start_index = dataInput.indexOf(F(":"));
            byte stop_index  = dataInput.indexOf(F(","));
            data_csq = dataInput.substring(start_index+1, stop_index);

            rssi = data_csq.toInt();
            rssi = (2 * rssi) - 113;
            data_csq = String(rssi);
          }
        }
      }
    }
    if (rssi == -113){
      count++;
    }
  } while (rssi == -113 && count <= 10 || rssi == 85 && count <= 10);
  if (rssi == -113 || rssi == 85){
    data_csq = "-113";
    count = 0;
  }
  _serial_flush();
  return data_csq;
}

/*
 * Get Network registration status
 */
bool AT_SIM7020E::getNetworkRegStatus(){
  bool status = false;
  
  _Serial->println(F("AT+CGREG?"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("+CGREG:")) != -1){
        if (dataInput.indexOf(F("0,1")) != -1) {
          status = true;
          break;        
        }
      }
    }
  }
  _serial_flush();
  return status;
}

String AT_SIM7020E::getIMSI(){
  String imsi = "";
  _Serial->println(F("AT+CIMI"));
  while(1){
    if(_Serial->available()){
      dataInput=_Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("OK"))!=-1 && imsi.indexOf(F("45204"))!=-1) {
        break;
      }
      else if(dataInput.indexOf(F("ERROR"))!=-1){
        _Serial->println(F("AT+CIMI"));
      }
      else{
        imsi+=dataInput;
      }
    }
  }

  byte index = imsi.indexOf(F("45204"));
  imsi = imsi.substring(index,imsi.length());
  imsi.replace(F("OK"), "");  
  imsi.trim();

  blankChk(imsi);
  _serial_flush();
  return imsi;
}

String AT_SIM7020E::getDeviceIP(){
  _serial_flush();
  String deviceIP;
  _Serial->println(F("AT+CGPADDR=1"));
  bool chk=false;
  while(1){
    if(_Serial->available()){
      dataInput=_Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("+CGPADDR"))!=-1){
        chk=true;
        byte index = dataInput.indexOf(F(":"));
        byte index2 = dataInput.indexOf(F(","));
        deviceIP = dataInput.substring(index2+1, dataInput.length());
      }
      else if(dataInput.indexOf(F("OK"))!=-1 && chk){
        break;
      } 
    }
  }
  deviceIP.replace(F("\""),"");
  deviceIP.trim();

  blankChk(deviceIP);
  dataInput = "";
  _serial_flush();
  return deviceIP;
}

String AT_SIM7020E:: getAPN(){
  String out = "";
  _Serial->println(F("AT+CGDCONT?"));

  while(1){
    if(_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("+CGDCONT: 1")) != -1){
        byte index = dataInput.indexOf(F(":"));
        byte index2 = dataInput.indexOf(F(","));

        index = dataInput.indexOf(F(","), index2+1);
        index2 = dataInput.indexOf(F(","), index+1);
        out = dataInput.substring(index+2, index2-1);
        if (out == ",,") out = "";
        k=1;
      }
      if (dataInput.indexOf(F("OK"))!=-1){
        if (k==1) break;
        else {
          _Serial->println(F("AT+CGDCONT?"));
        }
      }
    }
  }
  
  dataInput="";
  blankChk(out);
  k=0;
  _serial_flush();
  return out;
}

/*
 * EPS Network Registration Status
 */
String AT_SIM7020E::getNetworkStatus(){
  String out = "";
  String data = "";
  byte count = 0;

  _Serial->println(F("AT+CEREG=2"));
  delay(200);
  _serial_flush();
  delay(500);
  _Serial->println(F("AT+CEREG?"));
  while(1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("+CEREG")) != -1){
        count++;
        if (count<10 && dataInput.indexOf(F(",2")) != -1){
          _serial_flush();
          _Serial->println(F("AT+CEREG?"));
        }
        else {
          data = dataInput;
          byte index = data.indexOf(F(": "));
          byte index2 = data.indexOf(F(","));
          byte index3 = data.indexOf(F(","), index2+1);
          out = data.substring(index2+1, index2+2);
          if (out == F("1")) out = F("Registered");
          else if (out == "0") out = F("Not Registered");
          else if (out == "2") out = F("Trying");
        }
      }
      else if (dataInput.indexOf(F("OK")) != -1) break;
    }
  }
  blankChk(out);
  _serial_flush();
  return(out);
}



/* 
 * Get radio stat.
 */
radio AT_SIM7020E::getRadioStat(){
  radio value;
  String out = "";
  _Serial->println(F("AT+CENG?"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("+CENG:")) != -1){
        
        byte indexStart = dataInput.indexOf(F(":")) + 2;
        byte indexEnd = dataInput.indexOf(F(","));
        value.earfcn = dataInput.substring(indexStart, indexEnd);

        indexStart = indexEnd + 1;
        indexEnd = dataInput.indexOf(F(","), indexStart);
        value.earfcnOffset = dataInput.substring(indexStart, indexEnd);
        
        indexStart = indexEnd + 1;
        indexEnd = dataInput.indexOf(F(","), indexStart);
        value.pci = dataInput.substring(indexStart, indexEnd);

        indexStart = indexEnd + 1;
        indexEnd = dataInput.indexOf(F(","), indexStart);
        value.cellid = dataInput.substring(indexStart, indexEnd);

        indexStart = indexEnd + 1;
        indexEnd = dataInput.indexOf(F(","), indexStart);
        value.rsrp = dataInput.substring(indexStart, indexEnd);

        indexStart = indexEnd + 1;
        indexEnd = dataInput.indexOf(F(","), indexStart);
        value.rsrq = dataInput.substring(indexStart, indexEnd);

        indexStart = indexEnd + 1;
        indexEnd = dataInput.indexOf(F(","), indexStart);
        value.rssi = dataInput.substring(indexStart, indexEnd);

        indexStart = indexEnd + 1;
        indexEnd = dataInput.indexOf(F(","), indexStart);
        value.snr = dataInput.substring(indexStart, indexEnd);

        indexStart = indexEnd + 1;
        indexEnd = dataInput.indexOf(F(","), indexStart);
        value.band = dataInput.substring(indexStart, indexEnd);

        indexStart = indexEnd + 1;
        indexEnd = dataInput.indexOf(F(","), indexStart);
        value.tac = dataInput.substring(indexStart, indexEnd);

        indexStart = indexEnd + 1;
        indexEnd = dataInput.indexOf(F(","), indexStart);
        value.ecl = dataInput.substring(indexStart, indexEnd);

        indexStart = indexEnd + 1;
        indexEnd = dataInput.indexOf(F(","), indexStart);
        value.txPwr = dataInput.substring(indexStart, indexEnd);

        indexStart = indexEnd + 1;
        indexEnd = dataInput.indexOf(F(","), indexStart);
        value.reRsrp = dataInput.substring(indexStart, indexEnd);
      }
      else if (dataInput.indexOf(F("OK")) != -1){
        break;
      }
    }
  }
  blankChk(value.pci);
  blankChk(value.rsrp);
  blankChk(value.rsrq);
  blankChk(value.snr);
  _serial_flush();
  return value;
}

void AT_SIM7020E::printRadioStat(radio value){
  Serial.print(F("earfcn = "));
  Serial.println(value.earfcn);
  Serial.print(F("earfcnOffset = "));
  Serial.println(value.earfcnOffset);
  Serial.print(F("pci = "));
  Serial.println(value.pci);
  Serial.print(F("cellid = "));
  Serial.println(value.cellid);
  Serial.print(F("rsrp = "));
  Serial.println(value.rsrp);
  Serial.print(F("rsrq = "));
  Serial.println(value.rsrq);
  Serial.print(F("rssi = "));
  Serial.println(value.rssi);
  Serial.print(F("snr = "));
  Serial.println(value.snr);
  Serial.print(F("band = "));
  Serial.println(value.band);
  Serial.print(F("tac = "));
  Serial.println(value.tac);
  Serial.print(F("ecl = "));
  Serial.println(value.ecl);
  Serial.print(F("txPwr = "));
  Serial.println(value.txPwr);
  Serial.print(F("reRsrp = "));
  Serial.println(value.reRsrp);
}

void AT_SIM7020E::blankChk(String& val){
  if (val == ""){
    val = "N/A";
  }
}

/*
 * Check PSM mode
 */
bool AT_SIM7020E::checkPSMmode(){
  bool status = false;
  _Serial->println(F("AT+CPSMS?"));
  while (1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if (dataInput.indexOf(F("+CPSMS: ")) != -1){
        if (dataInput.indexOf(F("1")) != -1) status = true;
        else status = false;
      }
      if (dataInput.indexOf(F("OK")) != -1){
        break;
      }
    }
  }
  return status;
}








// Send message type char *
void AT_SIM7020E::_Serial_print(char *msg){
  _Serial->print(msg);
}

// Send '\r\n'
void AT_SIM7020E::_Serial_println(){
  _Serial->println();
}




/****************************************/
/**          Utility                   **/
/****************************************/
// print char * to hex
void AT_SIM7020E::printHEX(char *str){
  char *hstr;
  hstr=str;
  char out[3];
  memset(out,'\0',2);
  bool flag=false;
  while(*hstr){
    flag=itoa((int)*hstr,out,16);
    
    if(flag){
      _Serial_print(out); 
    }
    hstr++;
  }
}

// Flush unwanted message from serial
void AT_SIM7020E::_serial_flush(){
  while(1){
    if (_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
    }
    else{
      dataInput = "";
      break;
    }
  }
  _Serial->flush();
}

dateTime AT_SIM7020E::getClock(unsigned int timezone){
  dateTime dateTime;
  _Serial->println(F("AT+CCLK?"));
  while(1){
    if(_Serial->available()){
      dataInput=_Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("+CCLK:"))!=-1){
        byte index = dataInput.indexOf(F(":"));
        byte index2 = dataInput.indexOf(F(","),index+1);
        byte index3 = dataInput.indexOf(F("+"),index2+1);
        dateTime.date = dataInput.substring(index+2,index2);         //YY/MM/DD
        dateTime.time = dataInput.substring(index2+1,index3);        //UTC time without adding timezone
      }
      if(dataInput.indexOf(F("OK"))!=-1){
        break;
      }
    }
  }
  if(dateTime.time!="" && dateTime.date!=""){
    byte index = dateTime.date.indexOf(F("/"));
    byte index2 = dateTime.date.indexOf(F("/"),index+1);
    unsigned int yy = ("20"+dateTime.date.substring(0,index)).toInt();
    unsigned int mm = dateTime.date.substring(index+1,index2).toInt();
    unsigned int dd = dateTime.date.substring(index2+1,dateTime.date.length()).toInt();

    index = dateTime.time.indexOf(F(":"));
    unsigned int hr = dateTime.time.substring(0,index).toInt()+timezone;

    if(hr>=24){
      hr-=24;
      //date+1
      dd+=1;
      if(mm==2){
        if((yy % 4 == 0 && yy % 100 != 0 || yy % 400 == 0)){
          if (dd>29) {
            dd==1;
            mm+=1;
          }
        }
        else if(dd>28){ 
          dd==1;
          mm+=1;
        }
      }
      else if((mm==1||mm==3||mm==5||mm==7||mm==8||mm==10||mm==12)&&dd>31){
        dd==1;
        mm+=1;
      }
      else if(dd>30){
        dd==1;
        mm+=1;
      }
    }
    dateTime.time = String(hr)+dateTime.time.substring(index,dateTime.time.length());
    dateTime.date = String(dd)+"/"+String(mm)+"/"+String(yy);
  }
  blankChk(dateTime.time);
  blankChk(dateTime.date);
  return dateTime;
}

void AT_SIM7020E::syncLocalTime(){
  _Serial->println(F("AT+CLTS=1"));
  delay(50);
  while(1){
    if(_Serial->available()){
      dataInput=_Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("OK"))!=-1){
        Serial.println(F("Sync Local Time sucessfully"));
        break;
      }
    }
  }
}

/****************************************/
/**                MQTT                **/
/****************************************/

bool AT_SIM7020E::newMQTT(String server, String port){
  _Serial->print(F("AT+CMQNEW="));
  _Serial->print(F("\""));
  _Serial->print(server);
  _Serial->print(F("\""));
  _Serial->print(F(","));
  _Serial->print(F("\""));
  _Serial->print(port);
  _Serial->print(F("\""));
  _Serial->print(F(","));
  _Serial->print(mqttCmdTimeout);           //command_timeout_ms
  _Serial->print(F(","));
  _Serial->print(mqttBuffSize);             //buff size
  _Serial->println();

  while(1){
    if(_Serial->available()){
      dataInput += _Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("+CMQNEW:"))!=-1 && dataInput.indexOf(F("OK"))!=-1){
        return true;
      }
      else if(dataInput.indexOf(F("ERROR"))!=-1){
        return false;
      }
    }
  }
}

bool AT_SIM7020E::sendMQTTconnectionPacket(String clientID,String username,String password,int keepalive, int version,int cleansession, int willflag, String willOption){
  //AT+CMQCON=<mqtt_id>,<version>,<client_id>,<keepalive_interval>,<cleansession>,<will_flag>[,<will_options>][,<username>,<password>]
  _Serial->print(F("AT+CMQCON=0,"));
  _Serial->print(version);                    //<version> : 3 > 3.1, 4 > 3.1.1
  _Serial->print(F(","));
  _Serial->print(F("\""));
  _Serial->print(clientID);                   //<client_id> : should be unique.Max length is 120
  _Serial->print(F("\""));
  _Serial->print(F(","));
  _Serial->print(keepalive);                  //<keepalive_interval> : 0 - 64800 
  _Serial->print(F(","));
  _Serial->print(cleansession);               //<cleansession>
  _Serial->print(F(","));
  _Serial->print(willflag);                   //<will_flag>  >> if 1 must include will_option

  if(willflag==1){
    _Serial->print(F(","));
    _Serial->print(willOption);               //"topic=xxx,QoS=xxx,retained=xxx,message_len=xxx,message=xxx"
  }

  if (username.length()>0){
    _Serial->print(F(","));
    _Serial->print(F("\""));
    _Serial->print(username);                 //<username> String, user name (option). Max length is 100
    _Serial->print(F("\""));
    _Serial->print(F(","));
    _Serial->print(F("\""));
    _Serial->print(password);                 //<password> String, password (option). Max length is 100
    _Serial->print(F("\""));
  }
  _Serial->println();

  while(1){
    dataInput = _Serial->readStringUntil('\n');
    if(dataInput.indexOf(F("OK"))!=-1){
      return true;
    }
    else if(dataInput.indexOf(F("ERROR"))!=-1 || dataInput.indexOf(F("+CMQDISCON"))!=-1){
      return false;
    }
  }
}

void AT_SIM7020E::disconnectMQTT(){
  _Serial->println(F("AT+CMQDISCON=0"));
  while(1){
    if(_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("OK"))!=-1){
        break;
      }
      if(dataInput.indexOf(F("ERROR"))!=-1){
        break;
      }
    }
  }
  dataInput=F("");
}

bool AT_SIM7020E::MQTTstatus(){ 
  _Serial->println(F("AT+CMQCON?"));
  String inp="";
  while(1){
    if(_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("+CMQCON:"))!=-1){
        inp=dataInput;
      }
      else if(dataInput.indexOf(F("OK"))!=-1){
        break;
      }
      else if(dataInput.indexOf(F("ERROR"))!=-1){
        return false;
      }
    }
  }
  dataInput=F("");
  if(inp!=""){
    //+CMQCON: <mqtt_id>,<connected_state>,<server>
    byte index = inp.indexOf(F(","));
    byte index2 = inp.indexOf(F(","),index+1);
    if(inp.substring(index+1,index2).indexOf(F("1"))!=-1){
      return true;
    }
    else if(inp.substring(index+1,index2).indexOf(F("0"))!=-1){
      return false;
    }
  }
}

void AT_SIM7020E::publish(String topic, String payload, unsigned int qos, unsigned int retained, unsigned int dup){
  //AT+CMQPUB=<mqtt_id>,<topic>,<QoS>,<retained>,<dup>,<message_len>,<message>
  dataInput=F("");
  char data[payload.length()+1];
  memset(data,'\0',payload.length());
  payload.toCharArray(data,payload.length()+1);

  _Serial->print(F("AT+CMQPUB=0,\""));
  _Serial->print(topic);                        //<topic> String, topic of publish message. Max length is 128
  _Serial->print(F("\""));            
  _Serial->print(F(","));
  _Serial->print(qos);                          //<Qos> Integer, message QoS, can be 0, 1 or 2.
  _Serial->print(F(","));
  _Serial->print(retained);                     //<retained> Integer, retained flag, can be 0 or 1.
  _Serial->print(F(","));
  _Serial->print(dup);                          //<dup> Integer, duplicate flag, can be 0 or 1.  
  _Serial->print(F(","));
  _Serial->print(payload.length()*msgLenMul);   //<message_len> Integer, length of publish message,can be from 2 to 1000.
                                                //If message is HEX data streaming,then <message_len> should be even.
  _Serial->print(F(",\""));
  printHEX(data);
  _Serial->print(F("\""));
  _Serial->println();  
}

bool AT_SIM7020E::subscribe(String topic, unsigned int qos){  
  //AT+CMQSUB=<mqtt_id>,<topic>,<QoS>
  _Serial->print(F("AT+CMQSUB=0,\""));
  _Serial->print(topic);                         //<topic> String, topic of subscribe message. Max length is 128.
  _Serial->print(F("\","));
  _Serial->println(qos);                         //<Qos> Integer, message QoS, can be 0, 1 or 2.

}

void AT_SIM7020E::unsubscribe(String topic){
  _Serial->print(F("AT+CMQUNSUB=0,\""));
  _Serial->print(topic);
  _Serial->println(F("\""));
  while(1){
    if(_Serial->available()){
      dataInput = _Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("OK"))!=-1){
        if(debug) Serial.print(F("Unsubscribe topic :"));
        if(debug) Serial.println(topic);
        break;
      }
      else if(dataInput.indexOf(F("ERROR"))!=-1){
        break;
      }
    }
  }
  dataInput=F("");
}

unsigned int AT_SIM7020E::MQTTresponse(){   //clear buffer before this
  unsigned int ret=0;
  if (_Serial->available()){
      char data=(char)_Serial->read();
      if(data=='\n' || data=='\r'){
          end=true;
      }
      else{
        dataInput+=data;        
      }
  }
  if (end){
        if (dataInput.indexOf(F("+CMQPUB"))!=-1 || dataInput.indexOf(F("+CMQPUBEXT"))!=-1){
        //+CMQPUB: <mqtt_id>,<topic>,<QoS>,<retained>,<dup>,<message_len>,<message>
        byte index = dataInput.indexOf(F(","));
        byte index2 = dataInput.indexOf(F(","),index+1);
        index = dataInput.indexOf(F(","),index2+1);
        index2 = dataInput.indexOf(F(","),index+1);
        index = dataInput.indexOf(F(","),index2+1);
        index2 = dataInput.indexOf(F(","),index+1);
        int msgLen = dataInput.substring(index+1,index2).toInt();

        char buf[dataInput.length()+1];
        memset(buf,'\0',dataInput.length());  //reset data
        dataInput.toCharArray(buf, sizeof(buf));

        char *p = buf;
        char *str;
        byte i=0;
        byte j=0;
        while ((str = strtok_r(p, ",", &p)) != NULL){
          // delimiter is the comma
          if(i==1){
            retTopic=str;
            int topiclen=retTopic.length();
            retTopic.replace(F("\""),"");
          }
          if(i==2){
            retQoS=str;
          }
          if(i==3){
            retRetained=str;
          }
          if(i==6 && dataInput.indexOf(F("+CMQPUB:"))!=-1 || i==8 && dataInput.indexOf(F("+CMQPUBEXT:"))!=-1){
            retPayload=str;
            if(msgLen>500){
              Serial.println(F("Data incoming overload. [Max 250 characters]."));
            }
            else if(msgLen<500 && dataInput.indexOf(F("+CMQPUBEXT:"))!=-1){
              //Do nothing
            }
            else{
              retPayload.replace(F("\""),"");
              if (MQcallback_p != NULL){
                MQcallback_p(retTopic,retPayload,retQoS,retRetained);
              }
            }
          }
          i++;
        }
        ret=1;
      }
      else if(dataInput.indexOf(F("OK"))!=-1) ret=2;
      else if(dataInput.indexOf(F("ERROR"))!=-1) ret=3;
      dataInput = F("");
      end = false;
    }
    retPayload = "";
  return ret;
}

int AT_SIM7020E::setCallback(MQTTClientCallback callbackFunc){
     int r = -1;

     if (MQcallback_p == NULL){
          MQcallback_p = callbackFunc;
          r = 0;
     }
     return r;
}

/****************************************/
/**                MQTTs               **/
/****************************************/

bool AT_SIM7020E::newMQTTs(String server, String port){
  _Serial->print(F("AT+CMQTTSNEW="));
  _Serial->print(F("\""));
  _Serial->print(server);
  _Serial->print(F("\""));
  _Serial->print(F(","));
  _Serial->print(F("\""));
  _Serial->print(port);
  _Serial->print(F("\""));
  _Serial->print(F(","));
  _Serial->print(mqttCmdTimeout);           //command_timeout_ms
  _Serial->print(F(","));
  _Serial->print(mqttBuffSize);             //buff size
  _Serial->println();

  while(1){
    if(_Serial->available()){
      dataInput += _Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("+CMQTTSNEW:"))!=-1 && dataInput.indexOf(F("OK"))!=-1){
        return true;
      }
      else if(dataInput.indexOf(F("ERROR"))!=-1){
        return false;
      }
    }
  }
}

bool AT_SIM7020E::setCertificate(byte cerType,int cerLength,byte isEnd,String CA){
  _Serial->print(F("AT+CSETCA="));
  _Serial->print(cerType);
  _Serial->print(F(","));
  _Serial->print(cerLength);
  _Serial->print(F(","));
  _Serial->print(isEnd);
  _Serial->print(F(","));
  _Serial->print(0);          //0 String encoding, 1 HEX Encoding
  _Serial->print(F(",\""));
  _Serial->print(CA);
  _Serial->println(F("\""));

  delay(100);

  while(1){
    if(_Serial->available()){
      dataInput=_Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("OK"))!=-1){
        return true;
      }
      else if (dataInput.indexOf(F("ERROR")) != -1){
        return false;
      }
    }
  }
}

bool AT_SIM7020E::checkCertificate(int r_len,int c_len,int p_len){  //re-check with other server
  bool r=false;
  bool c=false;
  bool p=false;

  _Serial->println(F("AT+CSETCA?"));
  while(1){
    if(_Serial->available()){
      dataInput=_Serial->readStringUntil('\n');
      if(dataInput.indexOf(F("Root CA:"))!=-1){
        if(dataInput.indexOf(String(r_len))!=-1) r=true;
      }
      if(dataInput.indexOf(F("Client CA:"))!=-1){
        if(dataInput.indexOf(String(c_len))!=-1) c=true;
      }
      if(dataInput.indexOf(F("Client Private Key:"))!=-1){
        if(dataInput.indexOf(String(p_len))!=-1) p=true;
      }
      if(dataInput.indexOf(F("OK"))!=-1){
        break;
      }
    }
  }
  return r&&c&&p;
}
