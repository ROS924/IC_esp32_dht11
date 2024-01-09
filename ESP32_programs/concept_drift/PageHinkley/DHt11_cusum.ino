#include "DHT.h"
//#include <iostream>
//#include <vector>
#include <cmath>
#include <WiFi.h>
#include <PubSubClient.h>

#define paramK 3
#define paramH 1

#define DHTPIN   15   // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

WiFiClient wifiClient;

const char* ssid = "dlink";
const char* password = "";
//const char* ssid = "labia-v14";
//const char* password = "87654321";
PubSubClient mqttClient(wifiClient); 
char *mqttServer = "10.2.0.67";
//char *mqttServer = "10.42.0.1";
int mqttPort = 1883;

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

 /*vector<float> janelaT; //temperatura
  vector<float> janelaU; // umidade
  vector<float> janelaS; // sensacao*/

  float gtT= 0,gtU= 0,gtS= 0;
  float acumT= 0, acumU= 0, acumS= 0, quadsT= 0, quadsU= 0, quadsS= 0;
  float tamT= 0, tamU= 0, tamS= 0;

  /*tamT = 0;
  tamU = 0;
  tamS = 0;
  acumT = 0;
  acumU = 0;
  acumS = 0;
  quadsT = 0;
  quadsU = 0;
  quadsS = 0;

  gtT = 0;
  startT = 0;
  gtU = 0;
  startU = 0;
  gtS = 0;
  startS = 0;*/

void loop() {
  // Wait a few seconds between measurements.
  delay(3000);
  char data [256];

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  bool mudaT = 0;
  bool mudaU = 0;
  bool mudaS = 0;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  /*
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  */
  

  
  
  //janelaT.push_back(t);
  acumT = acumT + t;
  //janelaU.push_back(h);
  acumU = acumU + h;

  //janelaS.push_back(hic);
  acumS = acumS + hic;

  tamT++;
  tamU++;
  tamS++;

  quadsT = quadsT + (t*t);
  quadsU = quadsU + (h*h);
  quadsS = quadsS + (hic*hic);

  float muT,muU,muS, sdT,sdU,sdS;

  //muT = media(janelaT);
  muT = acumT / tamT;
  //muU = media(janelaU);
  muU = acumU / tamU;

  //muS = media(janelaS);
  muS = acumS / tamS;


  //sdT = desvPad(janelaT, muT);
  sdT = sqrt(quadsT / tamT - (muT * muT));

  //sdU = desvPad(janelaU, muU);
  sdU = sqrt(quadsU / tamU - (muU * muU));

  //sdS = desvPad(janelaS, muS);
  sdS = sqrt(quadsS / tamS - (muS * muS));

  float ztT;
  if(sdT != 0){
    
    ztT = (t - muT)/sdT;    
  }
  else{
    ztT = 0;  
  }

  float ztU;
  if(sdU != 0){
    
    ztU = (h - muU)/sdU;    
  }
  else{
    ztU = 0;  
  }

  float ztS;
  if(sdS != 0){
    
    ztS = (hic - muS)/sdS;    
  }
  else{
    ztS = 0;  
  }

  float paramT,paramU,paramS;
  
  paramT = gtT + ztT - paramK;
  gtT = fmax(0, paramT);

  paramU = gtU + ztU - paramK;
  gtU = fmax(0, paramU);

  paramS = gtS + ztS - paramK;
  gtS = fmax(0, paramS);


  if(gtT > paramH){
    Serial.println(F("O comportamento da temperatura mudou"));

    mudaT = 1;

    gtT = 0;
    //janelaT.clear();
    acumT = 0;
    quadsT = 0;
    tamT = 0;
  }

  else if(gtU > paramH){

    Serial.println(F("O comportamento da umidade mudou"));

    mudaU = 1;

    gtU = 0;
    //janelaU.clear();
    acumU = 0;
    quadsU = 0;
    tamU = 0;
  }

  else if(gtS > paramH){

    Serial.println(F("O comportamento da sensação térmica mudou"));

    mudaS = 1;

    
    gtS = 0;
    //janelaS.clear();
    acumS = 0;
    quadsS = 0;
    tamS = 0;
  }

  sprintf(data,"{\"Temperatura\": %f, \"Umidade\": %f, \"IndiceCalor\": %f, \"MudancaTemp\": %d, \"MudancaUmi\": %d, \"MudancaSen\": %d}", t, h, hic, mudaT, mudaU, mudaS);


  if (!mqttClient.connected()){
    reconect();
  }   
  mqttClient.loop();

  

  
  Serial.print("Dados enviados: ");
  Serial.println(data);
  mqttClient.publish("/sensorDados", data);

  
}


