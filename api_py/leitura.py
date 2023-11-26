import matplotlib.pyplot as plt
import pandas as pd
plt.style.use('ggplot')



def plotI(fpath):
    df = pd.read_csv(fpath)
    plt.plot(df.Data, df.Temperatura, label='Temperatura')
    plt.plot(df.Data, df.IndiceCalor, label='IndiceCalor')
    plt.xlabel('Tempo')
    plt.ylabel('Medidas °c')
    plt.legend()
    plt.show()

def plotII(fpath):
    df = pd.read_csv(fpath)
    plt.plot(df.Data, df.Umidade, label='Umidade')
    plt.xlabel('Tempo')
    plt.ylabel('Porcentagem')
    plt.legend()
    plt.show()

plotI('../tabelas_csv/METEOROLOGIA.csv')
plotII('../tabelas_csv/METEOROLOGIA.csv')