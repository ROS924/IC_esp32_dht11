
#ifndef __DWTHAAR_H__
using namespace std;

#define __DWTHAAR_H__

#include <vector>


class DwtHaar {

    public:

        DwtHaar();

        vector<vector<double>> fazerMatrizTransf(int linhas, int colunas, double coef0, double coef1);

        vector<double> transformar(vector<vector<double>> matriz, vector<double> vetor, int linhas, int colunas);

        void executar();

        int getTamSaida();

        double *getSaida();

        void inserirValor(double valor);

    private:
        //double *sinal;
        double *saida;
        int tamSaida;
        //int tamanhoSinal;
        vector<double> coeficientes;
        vector<vector<double>> detalhes;
        int nivel;
        const double filtroBaixo0;
        const double filtroBaixo1;
        const double filtroAlto0;
        const double filtroAlto1;
  
};

#endif