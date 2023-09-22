from paho.mqtt import client as mqtt
from db import DataBase
from typing import Any
from time import time
import json


DB = DataBase()
with open("testes/mosq.conf") as file:
    r = file.read().split()
    HOST = r[-1]
    PORT = int(r[-2])


def on_message_padrao(client:mqtt.Client, userdata:Any, message:mqtt.MQTTMessage) -> None:
    data = time()
    payload:dict = json.loads(message.payload.decode("utf-8"))
    DB.adic_dado(data, payload['temperatura'], payload['umidade'], payload['indiceCalor'])
    print(f"Dados recebidos: ({payload['temperatura']} °C, {payload['umidade']}%, {payload['indiceCalor']} °C)")
    
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