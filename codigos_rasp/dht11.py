from river import drift
import random
import pywt
#from seeed_dht import DHT
#from sensors import NumericalSensor
import argparse
import logging
import time

adwin = drift.ADWIN()

class DHT11DriftSensor(NumericalSensor):
    def __init__(self, pin: int, logger: logging.Logger | None = None):
        super().__init__(logger)
        self._sensor = DHT("11", pin)

    def checkDrift(readings:list, window:list, value:float):

      if window.size() < 10:
        window.append(value)
      else:
        window.pop(0)
        window.append(value)

      adwin.update(value)

      if adwin.drift_detected():
        for val in window:
          readings.append(val)

      return 0

    def collect_readings(self, interval: int) -> tuple[list[float], list[float]]:
        temperature_readings = []
        humidity_readings = []
        start_time = time.time()
        windowTemp = []
        windowHumi = []

        while (time.time() - start_time) < interval:
            try:
                humidity, temperature = self._sensor.read()
                self._logger.debug(f"Temperature: {temperature}, Humidity: {humidity}")

                #temperature drift detection
                self.checkDrift(temperature_readings, windowTemp, temperature)

                #humidity drift detection
                self.checkDrift(humidity_readings,windowHumi,humidity)



                time.sleep(1)

            except IOError as e:
                self._logger.error(f"Error reading from sensor: {e}")
        # contain only the window's values in the moment of the drift detection
        return temperature_readings, humidity_readings

      

    

    @property
    def sensor_name(self) -> str:
        return "grove_dht11_sensor"

    @property
    def mqtt_topic(self) -> str:
        return "dht11"


