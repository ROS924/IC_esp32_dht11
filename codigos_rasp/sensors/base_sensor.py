from abc import abstractmethod, ABC
from datetime import datetime
import paho.mqtt.client as mqtt
import json
import logging


class _BaseSensor(ABC):
    """
    Classe base para todas as classes base de sensores.
    Uma classe criada para um sensor em específico não deve herdar diretamente dessa classe.
    """
    
    @abstractmethod
    def __init__(self, logger: logging.Logger | None = None):
        if logger:
            self._logger = logger
        else:
            pseudo_logger = logging.getLogger("sensor_logger")
            pseudo_logger.disabled = True
            pseudo_logger.propagate = False
            self._logger = pseudo_logger
        
        self._logger.debug(f'Starting "{self.sensor_name}" sensor')

        with open("config.json") as cfg:
            self.config: dict[str, str | int] = json.load(cfg)
        
        if self.config["mqtt"]["enabled"]:
            self.client = mqtt.Client()
            self.client.connect(self.config["mqtt"]["broker_uri"], self.config["mqtt"]["port"])

        else:
            self.client = None

    @property
    @abstractmethod
    def sensor_name(self) -> str:
        pass
    
    @property
    @abstractmethod
    def mqtt_topic(self) -> str:
        pass
    
    @abstractmethod
    def collect_readings(self, interval: int) -> list[float]:
        pass

    @staticmethod
    def generate_timestamp() -> tuple[int, str]:
        ''' Gera uma tupla com dois itens: o timestamp e sua versão formatada (para envio no MQTT).
        Atualmente a versão formatada é o timestamp convertido em string, mas isso pode ser alterado futuramente.
        '''

        timestamp = int(datetime.now().timestamp())
        return timestamp, str(timestamp)
    
    @abstractmethod
    def get_current_paths(self, time:int|None=None) -> tuple[str, str]:
        pass
    
    def publish(self, data: dict[str, str | float]):
        if not self.config["mqtt"]["enabled"]:
            return
        
        if not self.client:
            raise Exception("Please specify a broker URI in config.json file")
        
        topic = f'sensor/{self.config["sensor_id"]}/{self.mqtt_topic}'
        
        self.client.publish(topic, json.dumps(data))
        self._logger.info(f"Data published to MQTT: {data}")

    def close(self):
        if not self.config["mqtt"]["enabled"]:
            return
        
        if self.client:
            self.client.disconnect()
    