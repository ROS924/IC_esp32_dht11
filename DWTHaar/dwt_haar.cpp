#include "dwt_haar.h"

DwtHaar::DwtHaar(bool sinal[]){

}

// Constroi as matrizes passa-alta ou passsa-baixa
void DwtHaar::fazerMatrizTransf(int linhas, int colunas, double coef0, double coef1, double **matriz){

    int posCoef1 = 0, posCoef0 = 1;

    for(int i = 0; i<linhas;i++){
        for(int j = 0; j<colunas;j++){
            if(j == posCoef0){
                matriz[i][j] = coef0;
            }
            else if(j == posCoef1){
                matriz[i][j] = coef1;
            }
            else{
                matriz[i][j] = 0;
            }
        }

        posCoef0 +=2;
        posCoef1 +=2;

    }


}

//Gera os coeficientes ou detalhes do nível
vector<double> DwtHaar::transformar(double **matriz, vector<double> vetor, int linhas, int colunas){

    vector<double> resultado;

    //Transforma o vetor em matriz
    double matrizVetor[vetor.size()][1];

    for(int i = 0; i<vetor.size(); i++){
        for(int j =0; j<1;j++){
            matrizVetor[i][j] = vetor.at(i);
        }
    }

    //Multiplica as matrizes

    for (int i=0;i<linhas; i++) {
      for (int j=0; j<linhas; j++) {
        for (int k=0; k<colunas; k++) {

          double valor;
          valor = matriz[i][k] * matrizVetor[k][j]; 
          resultado.push_back(valor); 

        }

      }
    
    }

    return resultado;
}

