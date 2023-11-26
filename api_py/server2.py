from paho.mqtt import client as mqtt
from db2 import DataBase
from typing import Any
from time import time
import json


DB = DataBase()
PORT = 1883
with open("testes/mosq.conf") as file:
    HOST = file.read().split()[-1]


def on_message_padrao(client:mqtt.Client, userdata:Any, message:mqtt.MQTTMessage) -> None:
    data = time()
    #print(message.payload)
    payload:dict = json.loads(message.payload.decode("utf-8"))
    DB.adic_dado(data, payload['Temperatura'], payload['Umidade'], payload['IndiceCalor'], payload['MudancaTemp'], payload['MudancaUmi'], payload['MudancaSen'])
    print(f"Dados recebidos: ({payload['Temperatura']} °C, {payload['Umidade']}%, {payload['IndiceCalor']} °C, {payload['MudancaTemp']}, {payload['MudancaUmi']}, {payload['MudancaSen']})")
    
def on_message_futuro(client:mqtt.Client, userdata:Any, message:mqtt.MQTTMessage) -> None:
    pass


print("Iniciando o servidor...")
client = mqtt.Client(client_id="ServidorDados", protocol=mqtt.MQTTv5)
client.on_message = on_message_padrao
client.connect(HOST, port=PORT)
client.subscribe('/sensorDados', 2)

print("Servidor iniciado!\n")
try:
    client.loop_forever()
except KeyboardInterrupt:
    print("Servidor finalizado!")
