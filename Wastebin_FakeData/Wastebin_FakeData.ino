#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Wifi and mqtt_broker
const char* ssid = "LamCuong";
const char* password = "0963875959";
const char* mqtt_server = "171.244.173.204";

WiFiClient espClient; 
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (100)

// // waste1 Hoa Phuong - waste thực tế
// char waste1[MSG_BUFFER_SIZE];
// char waste1Ctrl[MSG_BUFFER_SIZE];
// int cap1 = 0;
// float long1 = 105.913118;
// float lat1 = 21.054636;
// char state1[6];
char stateChange[6] = "open";
char open[5]= "open";
char close[6]= "close";

// waste2 - Bang Lang 2
char waste2[MSG_BUFFER_SIZE];
String topicStt2 = "wasteManagement/waste2/status";
int cap2 = 20;
char state2[6];
float long2 = 105.909395;
float lat2 = 21.052702;

// waste3 - Bang Lang
char waste3[MSG_BUFFER_SIZE];
String topicStt3 = "wasteManagement/waste3/status";
int cap3 = 30;
char state3[6];
float long3 = 105.911343;
// float long3 = 106.911343;
float lat3 = 21.049722;

// waste4 - Chu Huy Man
char waste4[MSG_BUFFER_SIZE];
String topicStt4 = "wasteManagement/waste4/status";
int cap4 = 40;
char state4[6];
float long4 = 105.915119;
float lat4= 21.049546;

// waste 5 - Bang Lang 5 
char waste5[MSG_BUFFER_SIZE];
String topicStt5 = "wasteManagement/waste5/status";
int cap5 = 80;
char state5[6];
float long5 = 105.911346;
float lat5 = 21.053850;

// waste6 - Bung Binh
char waste6[MSG_BUFFER_SIZE];
String topicStt6 = "wasteManagement/waste6/status";
int cap6 = 60;
char state6[6];
float long6 = 105.910140;
float lat6 = 21.047495;

// waste 7 - Bang Lang 7
char waste7[MSG_BUFFER_SIZE];
String topicStt7 = "wasteManagement/waste7/status";
int cap7 = 70;
char state7[6];
float long7 = 105.913187;
float lat7 = 21.052748;


// waste8 - Doan Khue
char waste8[MSG_BUFFER_SIZE];
String topicStt8 = "wasteManagement/waste8/status";
int cap8 = 50;
char state8[6];
float long8 = 105.908075;
float lat8 = 21.050747;

// waste9 - Bung Binh
char waste9[MSG_BUFFER_SIZE];
String topicStt9 = "wasteManagement/waste9/status";
int cap9 = 90;
char state9[6];
float long9 = 105.915428;
float lat9 = 21.052784;

// waste 10 - Bang Lang 10
char waste10[MSG_BUFFER_SIZE];
String topicStt10 = "wasteManagement/waste10/status";
int cap10 = 95;
char state10[6];
float long10 = 105.914421;
float lat10 = 21.050450;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    Serial.println(clientId);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("wasteManagement", "hello world");
      // ... and resubscribe
      client.subscribe("wasteManagement");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publish() {
    // snprintf (waste1, MSG_BUFFER_SIZE," { \"latitude\": \"%f\",\"longtitude\": \"%f\",  \"capacity\": \"%d\", \"state\": \"%s\" } " ,lat1,long1,capacity, state1);
    // client.publish("wasteManagement/waste1/status", waste1, true); 
    stateAssign(state2);
    snprintf (waste2, MSG_BUFFER_SIZE," { \"latitude\": \"%f\",\"longtitude\": \"%f\",  \"capacity\": \"%d\", \"state\": \"%s\"} " ,lat2,long2,cap2, state2);
    client.publish("wasteManagement/waste2/status", waste2, true);

    stateAssign(state3);
    snprintf (waste3, MSG_BUFFER_SIZE," { \"latitude\": \"%f\",\"longtitude\": \"%f\",  \"capacity\": \"%d\", \"state\": \"%s\"} " ,lat3,long3,cap3,state3);
    client.publish("wasteManagement/waste3/status", waste3, true);

    stateAssign(state4);
    snprintf (waste4, MSG_BUFFER_SIZE," { \"latitude\": \"%f\",\"longtitude\": \"%f\",  \"capacity\": \"%d\", \"state\": \"%s\"} " ,lat4,long4,cap4, state4);
    client.publish("wasteManagement/waste4/status", waste4, true);

    stateAssign(state5);
    snprintf (waste5, MSG_BUFFER_SIZE," { \"latitude\": \"%f\",\"longtitude\": \"%f\",  \"capacity\": \"%d\", \"state\": \"%s\"} " ,lat5,long5,cap5, state5);
    client.publish("wasteManagement/waste5/status", waste5, true);

    stateAssign(state6);
    snprintf (waste6, MSG_BUFFER_SIZE," { \"latitude\": \"%f\",\"longtitude\": \"%f\",  \"capacity\": \"%d\", \"state\": \"%s\"} " ,lat6,long6,cap6, state6);
    client.publish("wasteManagement/waste6/status", waste6, true); 

    stateAssign(state7);
    snprintf (waste7, MSG_BUFFER_SIZE," { \"latitude\": \"%f\",\"longtitude\": \"%f\",  \"capacity\": \"%d\", \"state\": \"%s\"} " ,lat7,long7,cap7, state7);
    client.publish("wasteManagement/waste7/status", waste7, true);

    stateAssign(state8);
    snprintf (waste8, MSG_BUFFER_SIZE," { \"latitude\": \"%f\",\"longtitude\": \"%f\",  \"capacity\": \"%d\", \"state\": \"%s\"} " ,lat8,long8,cap8, state8);
    client.publish("wasteManagement/waste8/status", waste8, true);

    stateAssign(state9);
    snprintf (waste9, MSG_BUFFER_SIZE," { \"latitude\": \"%f\",\"longtitude\": \"%f\",  \"capacity\": \"%d\", \"state\": \"%s\"} " ,lat9,long9,cap9, state9);
    client.publish("wasteManagement/waste9/status", waste9, true);

    stateAssign(state10);
    snprintf (waste10, MSG_BUFFER_SIZE," { \"latitude\": \"%f\",\"longtitude\": \"%f\",  \"capacity\": \"%d\", \"state\": \"%s\"} " ,lat10,long10,cap10, state10);
    client.publish("wasteManagement/waste10/status", waste10, true);
}

void stateAssign(char *state){
  for(int i=0; i <7; i++){
    state[i] = stateChange[i];
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();

  // MQTT
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);
  

}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  publish();
  delay (5000);
}
