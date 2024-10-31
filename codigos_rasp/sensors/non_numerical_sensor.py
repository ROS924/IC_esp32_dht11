from .csv_sensor import _CSVSensor


class NonNumericalSensor(_CSVSensor):
    """
    Classe base para sensores que retornam valores não numéricos.
    """