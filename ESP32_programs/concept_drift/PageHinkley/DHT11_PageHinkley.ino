#include <WiFi.h>
#include <math.h>
#include "DHT.h"
#include <PubSubClient.h>
#define DHTPIN 15
#define DHTTYPE DHT11
#define k 2
#define h 1


DHT dht(DHTPIN,DHTTYPE);
WiFiClient wifiClient;

//LAB-IA
//LabIAterumo2022
const char* ssid = "dlink";
const char* password = "";
//const char* ssid = "labia-v14";
//const char* password = "87654321";
PubSubClient mqttClient(wifiClient); 
char *mqttServer = "10.2.0.67";
//char *mqttServer = "10.42.0.1";
int mqttPort = 1883;


template <typename NumberType>
class PageHinkley{
  private:
  NumberType gt=0, Gt=0, zt, mu, sd; // mu = média; sd = desvio padrão
  NumberType S=0, Sq=0, n = 0; // S = Soma dos itens; Sq = soma dos quadrados dos itens; n = quantidade de itens

  public:

  void atualizar(NumberType valor){
    (this->n)++;
    this->S += valor;
    this->Sq += valor*valor;
    this->mu = this->S/this->n;
    this->sd = sqrt(this->Sq/this->n-this->mu*this->mu);

    this->zt = this->sd?((valor-this->mu)/this->sd):0;
    this->gt = this->gt + this->zt - k;
    this->Gt = fmin(this->gt, this->Gt);
  }

  bool houve_mudanca(){
    return (this->gt - this->Gt) > h;
  }

  void resetar(){
    this->gt = 0;
    this->zt = 0;
    this->mu = 0;
    this->sd = 0;
    this->Gt = 0;
    this->Sq = 0;
    this->n = 0;
    this->S = 0;
  }
};

PageHinkley<float> phTemperatura;
PageHinkley<float> phUmidade;
PageHinkley<float> phIndiceCalor;

void setup() {
  Serial.begin(115200);
  pinMode(DHTPIN, INPUT);
  dht.begin();
  
  // wifi setup
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
  //mqttClient.setCallback(callback);
}

//Conectar esp32 ao mqtt broker
//const char* user = "ROS";
//const char* passwd = "SaNToS";
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
  char data [256];


  //Ler dados do sensor
  float umidade = dht.readHumidity();
  //Celsius
  float temperatura = dht.readTemperature();

  if (isnan(umidade) || isnan(temperatura) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(500);
    return;
  }
 
  //Calcular sensação térmica em Celsius
  float indiceCalor = dht.computeHeatIndex(temperatura,umidade,false);

  phTemperatura.atualizar(temperatura);
  phUmidade.atualizar(umidade);
  phIndiceCalor.atualizar(indiceCalor);

  sprintf(data,"{\"Temperatura\": %f, \"Umidade\": %f, \"IndiceCalor\": %f, \"MudancaTemp\": %d, \"MudancaUmi\": %d, \"MudancaSen\": %d}",
  temperatura, umidade, indiceCalor, (int) phTemperatura.houve_mudanca(), (int) phUmidade.houve_mudanca(), (int) phIndiceCalor.houve_mudanca());

  bool mud = true;
  /*if (phTemperatura.houve_mudanca()){
    Serial.println(F("Houve mudança de temperatura"));
    mud = true;
    phTemperatura.resetar();
  }
  else if (phUmidade.houve_mudanca()){
    Serial.println(F("Houve mudança de umidade"));
    mud = true;
    phUmidade.resetar();
  }
  else if (phIndiceCalor.houve_mudanca()){
    Serial.println(F("Houve mudança de sensação térmica"));
    mud = true;
    phIndiceCalor.resetar();
  }*/

  if (mud){
    if (!mqttClient.connected()){
      reconect();
    }   
    mqttClient.loop();

    Serial.print("Dados enviados: ");
    //Serial.println(data);
    mqttClient.publish("/sensorDados", data);
  } else {
    Serial.print("Dados nao enviados: ");
  }

  if (phTemperatura.houve_mudanca() || phUmidade.houve_mudanca() || phIndiceCalor.houve_mudanca()){
    phTemperatura.resetar();
    phUmidade.resetar();
    phIndiceCalor.resetar();
  }
  Serial.println(data);
  delay(2000);
}