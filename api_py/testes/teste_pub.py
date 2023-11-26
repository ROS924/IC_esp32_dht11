from paho.mqtt.properties import Properties
from paho.mqtt.packettypes import PacketTypes 
from paho.mqtt import client as mqtt

from random import randint
import json


with open("mosq.conf") as file:
    r = file.read().split()
    HOST = r[-1]
    PORT = int(r[-2])

def gen_random_payld() -> bytes|str:
    '''Gera um payload aleatório'''

    temp = randint(-20, 80)
    umid = randint(0, 100)
    pl = {"Temperatura": temp, "Umidade": umid, "IndiceCalor": temp+randint(-5, 5)}

    return json.dumps(pl)


client = mqtt.Client(client_id="Publicador_"+str(randint(10_000, 99_999)), protocol=mqtt.MQTTv5)
client.connect(HOST, port=PORT, keepalive=120)
client.loop_start()

pay = gen_random_payld()
client.publish("/sensorDados", pay, 2, properties=Properties(PacketTypes.PUBLISH)).wait_for_publish()
print("Payload publicado:", pay)
