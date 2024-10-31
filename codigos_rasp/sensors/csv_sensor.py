from .base_sensor import _BaseSensor
from datetime import datetime
import csv
import os


class _CSVSensor(_BaseSensor):
    """
    Classe base para tipos de sensores em que são necessários salvar arquivos csv no dispositivo.
    Uma classe criada para um sensor em específico não deve herdar diretamente dessa classe.
    """

    def _write_header(self, file_path: str, header_row: list[str]):
        if not os.path.isfile(file_path):
            with open(file_path, mode='a', newline='') as file:
                writer = csv.writer(file)
                writer.writerow(header_row)

            self._logger.debug(f"Header written to new file: {file_path}")
    
    def get_current_paths(self, time=None) -> tuple[str, str]:
        try:
            date = datetime.fromtimestamp(time)
            
        except TypeError:
            date = datetime.now()

        date_str = date.strftime("%Y-%m-%d")
        hour_str = date.strftime("%H-%M-%S")
        day_dir = os.path.join(self.config["base_dir"], date_str)
        os.makedirs(day_dir, exist_ok=True)
        file_path = os.path.join(day_dir, f"{self.sensor_name}.csv")

        return day_dir, file_path
    
    def write_csv(self, data: dict[str, float], time: int | None = None):
        path, file_path = self.get_current_paths(time)
        self._write_header(file_path, list(data.keys()))

        with open(file_path, mode='a', newline='\n') as file:
            writer = csv.writer(file)
            writer.writerow(list(data.values()))
        
        info = ", ".join((f"{a}={b}" for a, b in data.items()))
        self._logger.info(f"Data written to CSV: {info}")