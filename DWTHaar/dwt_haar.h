
#ifndef __DWTHAAR_H__

using namespace std;
#define __DWTHAAR_H__

#include <vector>


class DwtHaar {

    public:

        DwtHaar(double sinal[], int tamanhoSinal);

        void fazerMatrizTransf(int linhas, int colunas, double coef0, double coef1, double **matriz);

        vector<double> transformar(double **matriz, vector<double> vetor, int linhas, int colunas);


    private:
        vector<double> coeficientes;
        vector<vector<double>> detalhes;
        vector<double> saida;
        int nivel;
        double filtroBaixo0;
        double filtroBaixo1;
        double filtroAlto0;
        double filtroAlto1;
  
};

#endif