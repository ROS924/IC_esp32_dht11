#include "DHT.h"
#include <cmath>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adwin.h>
#include <dwthaar.h>


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

Adwin adwinT(10);
Adwin adwinU(10);
Adwin adwinS(10);

DwtHaar* haarT = new DwtHaar();

bool coletar = true;
int contador = 0;

bool mudaT;
bool mudaU;
bool mudaS;

void loop() {

  char data [256];
  

  if(coletar == true){

    // Wait a few seconds between measurements.
    delay(3000);
    

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    //float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    haarT->inserirValor(t);
    contador++;

    if(contador == 60){
      coletar = false;
    }
  
  }
  else{

    haarT->executar();

    int tamanhoSaida;
    tamanhoSaida = haarT->getTamSaida();

    double *serieHaarT;
    serieHaarT = NULL;
    serieHaarT = haarT->getSaida();

    for(int i = 0; i < tamanhoSaida; i++){

      double temp;
      temp = serieHaarT[i];

      mudaT = adwinT.update(temp);

      if(mudaT == 1){
        Serial.println(F("O comportamento da temperatura mudou"));
      }

      sprintf(data,"{\"Temperatura\": %lf}", temp);


      if (!mqttClient.connected()){
        reconect();
      }   
      mqttClient.loop();

      

      
      Serial.print("Dados enviados: ");
      Serial.println(data);
      mqttClient.publish("/sensorDados", data);

    }

    contador = 0;
    coletar = true;

  }

  
}