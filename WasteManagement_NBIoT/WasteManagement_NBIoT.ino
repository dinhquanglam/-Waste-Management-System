// 8  -> Servo_signal
// 9  -> Ultra_trig
// 10 -> Ultra_echo
// 11 -> 7020E_PWR
// 12 -> 7020E_RX
// 13 -> 7020E_TX
#include <Servo.h>
#include "AIS_SIM7020E_API.h"
String data_return;
String address    = "171.244.173.204";               //Your IPaddress or mqtt server url
String serverPort = "1884";         
  //Your server port
String clientID = String(random(0xFFFFFF), HEX);

// String topic      = "WasteSystem_NBIoT";               //Your topic     < 128 characters
// String payload    = "HelloWorld!";    //Your payload   < 500 characters
String username   = "";               //username for mqtt server, username <= 100 characters
String password   = "";               //password for mqtt server, password <= 100 characters 
unsigned int subQoS       = 0;
unsigned int pubQoS       = 0;
unsigned int pubRetained  = 0;
unsigned int pubDuplicate = 0;
const long interval = 5000;  //millisecond 
unsigned long previousMillis = 0;
int cnt = 0;

// servo
#define TURN_TIME 175 // ESP bi nong: 455
Servo myservo; 
const int servoPin = 8;

// Ultrasonic
const int trigPin = 9;
const int echoPin = 10;
long duration;
float distance;
int capacity;
float deepOfWaste = 20;
//////////

#define MSG_BUFFER_SIZE (100)

// waste1 Hoa Phuong
String topicStt1 = "NBIoT/waste1/status";
String topicCtrl1 = "NBIoT/waste1/control";
char waste1[MSG_BUFFER_SIZE];
char waste1Ctrl[MSG_BUFFER_SIZE];
int cap1 = 0;
char long1[11] = "105.913118";
char lat1[11] = "21.054636";
char state1[6];
char stateChange[6] = "open";
char open[5]= "open";
char close[6]= "close";

// waste2 - Chu Huy Man
String topicStt2 = "NBIoT/waste2/status";
char waste2[MSG_BUFFER_SIZE];
int cap2 = 15;
bool state2;
char long2[11] = "105.915119";
char lat2[11]= "21.049546";

// waste3 - Bang Lang
String topicStt3 = "NBIoT/waste3/status";
char waste3[MSG_BUFFER_SIZE];
int cap3 = 25;
bool state3;
char long3[11] = "105.911343";
char lat3[11] = "21.049722";

// waste4 - Bang Lang 2
String topicStt4 = "NBIoT/waste4/status";
char waste4[MSG_BUFFER_SIZE];
int cap4 = 60;
bool state4;
char long4[11] = "105.909395";
char lat4[11] = "21.052702";

// waste5 - Doan Khue
String topicStt5 = "NBIoT/waste5/status";
char waste5[MSG_BUFFER_SIZE];
int cap5 = 75;
bool state5;
char long5[11] = "105.908075";
char lat5[11] = "21.050747";

// waste6 - Bung Binh
String topicStt6 = "NBIoT/waste6/status";
char waste6[MSG_BUFFER_SIZE];
int cap6 = 99;
bool state6;
char long6[11] = "105.910140";
char lat6[11] = "21.047495";

// waste7 - Bung Binh
String topicStt7 = "NBIoT/waste7/status";
char waste7[MSG_BUFFER_SIZE];
int cap7 = 90;
bool state7;
char long7[11] = "105.915428";
char lat7[11] = "21.052784";

AIS_SIM7020E_API nb;
void setup() {
  Serial.begin(9600);
  nb.begin(address, serverPort);
  setupMQTT();
  nb.setCallback(callback); 
  previousMillis = millis();  
  //ultrasonic
  pinMode(trigPin,OUTPUT);   
  pinMode(echoPin,INPUT);  
}

void loop() {
  //String payload = nb.messagePayload();
  nb.MQTTresponse();
  publish();
  wasteBinData();
 }


void setupMQTT(){
  if(!nb.connectMQTT(address,serverPort,clientID,username,password)){ 
     Serial.println("\nconnectMQTT");
  }
    nb.subscribe("NBIoT",subQoS);
//  nb.unsubscribe(topic); 
}
void connectStatus(){
    if(!nb.MQTTstatus()){
        if(!nb.NBstatus()){
           Serial.println("reconnectNB ");
           nb.begin();
        }
       Serial.println("reconnectMQ ");
       setupMQTT();
    }   
}
void callback(String &topic,String &payload, String &QoS,String &retained){
  Serial.println("-------------------------------");
  Serial.println("# Message from Topic \""+topic+"\" : "+nb.toString(payload));
  Serial.println("# QoS = "+QoS);
  if(retained.indexOf(F("1"))!=-1){
    Serial.println("# Retained = "+retained);
  }
}
void publish(){
    unsigned long currentMillis = millis();
  // publish
  if (currentMillis - previousMillis >= interval){      
      connectStatus();
      snprintf (waste1, MSG_BUFFER_SIZE," { \"latitude\": \"%s\",\"longtitude\": \"%s\",  \"capacity\": \"%d\", \"state\": \"%s\" } " ,lat1,long1,capacity, open);
      nb.publish(topicStt1,waste1,false,true);
      delay(500);      
      // snprintf (waste2, MSG_BUFFER_SIZE," { \"latitude\": \"%s\",\"longtitude\": \"%s\",  \"capacity\": \"%d\", \"state\": \"%s\" } " ,lat2,long2,cap2, open);
      // nb.publish(topicStt2,waste2,false,true);
      // snprintf (waste3, MSG_BUFFER_SIZE," { \"latitude\": \"%s\",\"longtitude\": \"%s\",  \"capacity\": \"%d\", \"state\": \"%s\" } " ,lat3,long3,cap3, open);
      // nb.publish(topicStt3,waste3,1,true);
      // snprintf (waste4, MSG_BUFFER_SIZE," { \"latitude\": \"%s\",\"longtitude\": \"%s\",  \"capacity\": \"%d\", \"state\": \"%s\" } " ,lat4,long4,cap4, open);
      // nb.publish(topicStt4,waste4,1,true);
      // snprintf (waste5, MSG_BUFFER_SIZE," { \"latitude\": \"%s\",\"longtitude\": \"%s\",  \"capacity\": \"%d\", \"state\": \"%s\" } " ,lat5,long5,cap5, open);
      // nb.publish(topicStt3,waste3,1,true);
      // snprintf (waste6, MSG_BUFFER_SIZE," { \"latitude\": \"%s\",\"longtitude\": \"%s\",  \"capacity\": \"%d\", \"state\": \"%s\" } " ,lat6,long6,cap6, open);
      // nb.publish(topicStt6,waste6,1,true);
      // snprintf (waste7, MSG_BUFFER_SIZE," { \"latitude\": \"%s\",\"longtitude\": \"%s\",  \"capacity\": \"%d\", \"state\": \"%s\" } " ,lat7,long7,cap7, open);
      // nb.publish(topicStt7,waste7,1,true);

      previousMillis = currentMillis; 
  }
}
void wasteBinData(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  // distance= duration*0.034/2;
  distance = int(duration/2/29.412);
  // Serial.println(distance);
  if (distance >= 560){
    distance = 0;
  }
  else if (distance > deepOfWaste && distance < 560){
    distance = deepOfWaste;
  }
  capacity = 100 - (distance/deepOfWaste)*100;
  // // if waste is full, close the bin.
  // if (capacity == 100){
  //   if (!strcmp(state1, open)){ // equal to (strcmp(state1, open) == 0)
  //   // close the bin
  //   myservo.write(0); // Start turning clockwise
  //   delay(TURN_TIME); // Go on turning for the right duration
  //   myservo.write(90);// Stop turning

  //     for(int i=0; i <6; i++){
  //         stateChange[i] = close[i];
  //       }
  //       Serial.println("close");
  //   }
  // }
}

 
