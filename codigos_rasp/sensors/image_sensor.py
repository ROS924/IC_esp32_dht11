from .base_sensor import _BaseSensor
from datetime import datetime
import os


class ImageSensor(_BaseSensor):
    """
    Classe base para sensores que retornam imagens.
    """

    def publish(self, data: dict[str, str | float], image_data: bytes | None = None):
        super().publish(data)
        topic = f'sensor/{self.config["sensor_id"]}/{self.mqtt_topic}'

        if image_data and self.config["mqtt"]["enabled"]:
            self.client.publish(topic + "/image", image_data)
            self._logger.info("Image published to MQTT")
    
    def get_current_paths(self, time=None) -> tuple[str, str]:
        try:
            date = datetime.fromtimestamp(time)
            
        except TypeError:
            date = datetime.now()

        date_str = date.strftime("%Y-%m-%d")
        hour_str = date.strftime("%H-%M-%S")
        day_dir = os.path.join(self.config["base_dir"], date_str, self.sensor_name)
        os.makedirs(day_dir, exist_ok=True)
        file_path = os.path.join(day_dir, f"{hour_str}.jpg")

        return day_dir, file_path
    
    def write_jpg(self, image: bytes, time:int|None=None):
        path, file_path = self.get_current_paths(time)
        
        with open(file_path, 'wb') as img:
            img.write(image)

        self._logger.info(f"Captured image saved as: {file_path}")
    