from datetime import datetime
from typing import Iterator
from pathlib import Path
import sqlite3 as sql

class DataBase:
    def __init__(self, path:str=""):
        self._db = sql.connect(Path(path, 'database.db').as_posix(), detect_types=True)
        self._cur = self._db.cursor()

        try:
            self._cur.execute("SELECT * FROM METEOROLOGIA")
        except sql.OperationalError:
            self._cur.execute("CREATE TABLE METEOROLOGIA (Data BIGINT, Temperatura FLOAT, Umidade FLOAT, IndiceCalor FLOAT)")
        finally:
            self._cur.fetchall()
    
    def __del__(self):
        self._cur.close()
        self._db.close()
        
    def adic_dado(self, data:int|float|datetime, temperatura:float, umidade:float, indice_calor:float) -> None:
        if isinstance(data, datetime):
            data = int(data.timestamp())
        elif isinstance(data, float):
            data = int(data)

        self._cur.execute("INSERT INTO METEOROLOGIA VALUES (?, ?, ?, ?)", (data, temperatura, umidade, indice_calor))
        self._cur.fetchone()
        self._db.commit()
    
    def obter_dados(self, de:int|datetime|None=None, ate:int|datetime|None=None) -> Iterator[tuple[datetime, float, float, float]]:
        dcond = ""
        acond = ""

        if not (de is None):
            if  isinstance(de, datetime):
                de = int(de.timestamp())
            dcond = "Data >= "+str(de)
        
        if not (ate is None):
            if  isinstance(ate, datetime):
                ate = int(ate.timestamp())
            acond = "Data <= "+str(ate)
        
        if not (dcond or acond):
            cond = ""
        else:
            cond = "WHERE "
            if dcond and acond:
                cond += dcond + " AND " + acond
            else:
                cond += dcond+acond

        self._cur.execute("SELECT * FROM METEOROLOGIA"+cond)

        return map(lambda i: (datetime.fromtimestamp(i[0]), i[1], i[2], i[3]), self._cur.fetchall())
    
    def imprimir_dados(self, de:int|datetime|None=None, ate:int|datetime|None=None) -> None:
        head = "{}Dia e Hora{}Temperatura{}Umidade{}Sens. Term.".format(*(' '*i for i in (5, 9, 3, 3)))
        print(head, '\n'+'-'*len(head))
        
        for d, t, u, s in self.obter_dados(de, ate):
            data_str = f"{d.day:02d}/{d.month:02d}/{d.year:02d} - {d.hour:02d}:{d.minute:02d}:{d.second:02d}"
            print(f"{data_str}    {t:6.2f} °C     {u:5.2f}%    {s:6.2f} °C")


if __name__ == "__main__":
    DataBase().imprimir_dados()