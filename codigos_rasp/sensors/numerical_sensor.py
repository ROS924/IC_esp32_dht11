from .csv_sensor import _CSVSensor
from scipy import stats
from statistics import mean, stdev
import warnings as warns


class NumericalSensor(_CSVSensor):
    """
    Classe base para sensores que retornam valores numéricos.
    """

    def calculate_statistics(self, readings: list[float]) -> tuple[float, float, tuple[float, float]]:
        if readings:
            avg = mean(readings)
            stddev = stdev(readings)

            with warns.catch_warnings():
                warns.filterwarnings("error")
                try:
                    conf_interval = stats.norm.interval(0.95, loc=avg, scale=stddev / (len(readings) ** 0.5))
                except RuntimeWarning:
                    conf_interval = (avg, avg)

            self._logger.debug(f"Average: {avg}, Std Dev: {stddev}, Conf Interval: {conf_interval}")
                
            return avg, stddev, conf_interval
        
        else:
            self._logger.debug("No readings collected")

            return None, None, (None, None)
