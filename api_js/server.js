const Meteorologia = require("./meteorologia")
const mqtt = require("mqtt")

console.log("Rodando")

// conexão servidor aberto websocket
const client = mqtt.connect('mqtt://broker.hivemq.com');

// conexão com servidor próprio
/* const client = mqtt.connect('mqtt://broker.hivemq.com', { 
  username: 'my-username',
  password: 'my-password'
}); */


// Receber menssagem do servidor
topico = client.subscribe("/sensorDados")

client.on('message', (topic=topico, message) => {
    console.log(`Received message on topic ${topic}: ${message}`);
    payload = JSON.parse(message)
    temp = payload.Temperatura
    umid = payload.Umidade
    indice = payload.IndiceCalor

    Meteorologia.create({
        temperatura: temp,
        umidade: umid,
        indiceCalor: indice
    })
});
