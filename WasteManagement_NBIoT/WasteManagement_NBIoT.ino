// 8  -> Servo_signal
// 9  -> Ultra_trig
// 10 -> Ultra_echo
// 11 -> 7020E_PWR
// 12 -> 7020E_RX
// 13 -> 7020E_TX
#include <Servo.h>
#include "AIS_SIM7020E_API.h"
#include <ArduinoJson.h>

String data_return;
String address    = "171.244.173.204";               //Your IPaddress or mqtt server url
String serverPort = "1884";         
  //Your server port
String clientID = "lamdq3-abcsh2";

// String topic      = "WasteSystem_NBIoT";               //Your topic     < 128 characters
// String payload    = "HelloWorld!";    //Your payload   < 500 characters
String username   = "admin";               //username for mqtt server, username <= 100 characters
String password   = "admin";               //password for mqtt server, password <= 100 characters 
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

#define MSG_BUFFER_SIZE (10)

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
void setupMQTT(){
  if(!nb.connectMQTT(address,serverPort,clientID,username,password)){ 
     Serial.println("\nconnectMQTT");
  }
    nb.subscribe(topicCtrl1,subQoS);   
    // nb.subscribe(topicStt1,subQoS);

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

  // // get state from broker
  // for (int i = 0; i < 200; i++) {
  //   Serial.print((char)payload[i]);  
  // }
  //   StaticJsonDocument<200> doc;
  //   // Deserialize the JSON document
  //   DeserializationError error = deserializeJson(doc, payload);
  //   // Test if parsing succeeds.
  // if (error) {
  //   Serial.print(F("deserializeJson() failed: "));
  //   Serial.println(error.f_str());
  //   return;
  // }
  //   const bool state = doc["state"];
  //   Serial.println();
  //   Serial.println(state);
  //   if(state == false){

  //   // close the pin

  //   myservo.write(0); // Start turning clockwise
  //   delay(TURN_TIME); // Go on turning for the right duration
  //   myservo.write(90);// Stop turning



  //     for(int i=0; i <6; i++){
  //         stateChange[i] = close[i]; 
  //     }      
  //     Serial.println("close");
  //   }else{
  //   // open the pin
  //       myservo.write(180); // Start turning anti-clockwise
  //   delay(TURN_TIME); // Go on turning for the right duration
  //   myservo.write(90);// Stop turning



  //       for(int i=0; i <6; i++){
  //         stateChange[i] = open[i];
  //       }
  //       Serial.println("open");
  //     }  
  // Serial.println();
  // Serial.println("-----------------------");  
}
void publish(){
    stateAssign(state1);
    unsigned long currentMillis = millis();
    // publish
  if (currentMillis - previousMillis >= interval){      
      connectStatus();
      snprintf (waste1, MSG_BUFFER_SIZE," { \"latitude\": \"%f\",\"longtitude\": \"%f\",  \"capacity\": \"%d\", \"state\": \"%s\" } " ,lat1,long1,capacity, state1);
      
      nb.publish(topicStt1,waste1,false,true);
      previousMillis = currentMillis; 
  }
}
void wasteBinData(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = int(duration/2/29.412);
  // Serial.println(distance);
  if (distance >= 560){
    distance = 0;
  }
  else if (distance > deepOfWaste && distance < 560){
    distance = deepOfWaste;
  }
  capacity = 100 - (distance/deepOfWaste)*100;
}
void stateAssign(char *state){
  for(int i=0; i <7; i++){
    state[i] = stateChange[i];
  }
}
void loop() {
  //String payload = nb.messagePayload();
  nb.MQTTresponse();
  wasteBinData();
  publish();
 }

 
