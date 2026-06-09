#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

void send_status();

WiFiClient espClient;
PubSubClient mqttClient(espClient);

bool light1_status = false;
bool light2_status = false;
bool light3_status = false;
bool light4_status = false;
unsigned long int uptime=0, lastHeartbeat=0;

/* functions definition */
void init_peripherals(){
 
  pinMode(LIGHT_01, OUTPUT);
  pinMode(LIGHT_02, OUTPUT);
  pinMode(LIGHT_03, OUTPUT);
  pinMode(LIGHT_04, OUTPUT);

  digitalWrite(LIGHT_01, LOW);
  digitalWrite(LIGHT_02, LOW);
  digitalWrite(LIGHT_03, LOW);
  digitalWrite(LIGHT_04, LOW);  
}

void init_wifi(){
  WiFi.mode(WIFI_STA);
  Serial.printf("Connecting to %s\n", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(300);
    Serial.print(".");
  }
  Serial.printf("\nConnected | IP Address : ");
  Serial.println(WiFi.localIP());
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
}

void reconnect(){
  
  while(!mqttClient.connected()){
    Serial.println("Connecting MQTT...");

    if(mqttClient.connect("MY_HOME")){
      Serial.println("MQTT Connected");
      mqttClient.subscribe("addyhome/bedroom/light");
      mqttClient.subscribe("addyhome/livingroom/light");
      mqttClient.subscribe("addyhome/kitchen/light");
      mqttClient.subscribe("addyhome/garage/light");

      send_status();
    }
    else{
      Serial.print("Failed: ");
      Serial.println(mqttClient.state());
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){

  Serial.print("Topic: ");
  Serial.println(topic);
  
  String msg;
  for(int i=0; i<length; i++){
    msg += (char)payload[i];
  }
  Serial.print("Payload : ");
  Serial.println(msg);

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, msg);
  if(error)
  {
    Serial.println("JSON Parse Failed");
    return;
  }
  bool state = doc["state"];

  if(!strcmp(topic, "addyhome/bedroom/light"))
  {
    digitalWrite(LIGHT_01, state);
    light1_status = state;
  }
  else if(!strcmp(topic, "addyhome/livingroom/light"))
  {
    digitalWrite(LIGHT_02, state);
    light2_status = state;
  }
  else if(!strcmp(topic, "addyhome/kitchen/light"))
  {
    digitalWrite(LIGHT_03, state);
    light3_status = state;
  }
  else if(!strcmp(topic, "addyhome/garage/light"))
  {
    digitalWrite(LIGHT_04, state);
    light4_status = state;
  }
  send_status();
}

void send_status()
{
    mqttClient.publish("addyhome/bedroom/light/status",
                       light1_status ? "ON" : "OFF");

    mqttClient.publish("addyhome/livingroom/light/status",
                       light2_status ? "ON" : "OFF");

    mqttClient.publish("addyhome/kitchen/light/status",
                       light3_status ? "ON" : "OFF");

    mqttClient.publish("addyhome/garage/light/status",
                       light4_status ? "ON" : "OFF");
}

void setup() {
 Serial.begin(115200);
 init_wifi();
 init_peripherals(); 
 mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
 mqttClient.setCallback(callback);
 reconnect();
 uptime = lastHeartbeat = millis();
}

void loop() {
  if(!mqttClient.connected()){
    reconnect();
  }
  mqttClient.loop();
  if(millis() - uptime >= 3000)
  {
    send_status();
    uptime = millis();
  }
  if(millis() - lastHeartbeat >=10000){
    lastHeartbeat = millis();
    mqttClient.publish("addyhome/status", "ONLINE");
  }
}
