#include "DHT.h"
#include <cmath>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adwin.h>
#include "esp_sleep.h"


#define DHTPIN   15   // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11

RTC_DATA_ATTR static DHT dht(DHTPIN, DHTTYPE);

RTC_DATA_ATTR static WiFiClient wifiClient;

RTC_DATA_ATTR static const char* ssid = "dlink";
const char* password = "";
//const char* ssid = "labia-v14";
//const char* password = "87654321";
RTC_DATA_ATTR static PubSubClient mqttClient(wifiClient); 
RTC_DATA_ATTR static char *mqttServer = "10.2.0.59";
//char *mqttServer = "10.42.0.1";
RTC_DATA_ATTR static int mqttPort = 1883;

RTC_DATA_ATTR static Adwin adwinT(10);  //guardar
RTC_DATA_ATTR static int firstMeasures = 32;  //guardar


void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));

  dht.begin();

  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print('.');
  }
  
  Serial.println("");
  Serial.println("WiFi conectada.");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());

  //mqtt setup
  mqttClient.setServer(mqttServer, mqttPort);
}

void reconect (){
    Serial.println("Connecting to MQTT Broker...");
    while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      //if (mqttClient.connect(clientId.c_str(),user, passwd)) {
      if (mqttClient.connect(clientId.c_str())){
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("/swa/commands");
      }
      
   }
}



void loop() {
  // Wait a few seconds between measurements.
  delay(3000);
  char data [256];


  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  bool mudaT;


  // Check if any reads failed and exit early (to try again).
  if (isnan(t) ){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


  mudaT = adwinT.update(t);


  sprintf(data,"{\"Temperatura\": %f, \"MudancaTemp\": %d}", t, mudaT);

  if(firstMeasures > 0){

    if (!mqttClient.connected()){
      reconect();
    }   
    mqttClient.loop();

    Serial.print("Dados enviados: ");
    Serial.println(data);
    mqttClient.publish("/sensorDados", data);

    firstMeasures--;

  }else{

    //verifica se O comportamento da temperatura mudou
    if(mudaT == 1){
      if (!mqttClient.connected()){
      reconect();
      }   
      mqttClient.loop();

      Serial.println(F("O comportamento da temperatura mudou"));
      Serial.print("Dados enviados: ");
      Serial.println(data);
      mqttClient.publish("/sensorDados", data);

    }

    const int deep_sleep_sec = 75;
    ESP_LOGI(TAG, "Entering deep sleep for %d seconds", deep_sleep_sec);
    esp_deep_sleep(1000000LL * deep_sleep_sec);
  }

  
}
