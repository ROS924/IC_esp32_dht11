#include "dwt_haar.h"
#include <cmath>
#include <vector>

DwtHaar::DwtHaar():filtroBaixo0(1/sqrt(2)),filtroBaixo1(1/sqrt(2)),filtroAlto0(1/sqrt(2)),filtroAlto1(-1/sqrt(2)){

    vector<double> coeficientes;
    vector<vector<double>> detalhes;

    //this->sinal = sinal;
    //this->tamanhoSinal = tamanhoSinal;
    this->coeficientes = coeficientes;
    this->detalhes = detalhes;
    this->nivel = 0;
    this->saida = NULL;
    this->tamSaida = 0;
      
}

// Constroi as matrizes passa-alta ou passsa-baixa
vector<vector<double>> DwtHaar::fazerMatrizTransf(int linhas, int colunas, double coef0, double coef1){
    vector<vector<double>> matriz;
    int posCoef1 = 0, posCoef0 = 1;

    for(int i = 0; i<linhas;i++){
        vector<double> vetorLinha;

        for(int j = 0; j<colunas;j++){
            if(j == posCoef0){
                vetorLinha.push_back(coef0);
            }
            else if(j == posCoef1){
                vetorLinha.push_back(coef1);
            }
            else{
                vetorLinha.push_back(0);
            }
        }

        matriz.push_back(vetorLinha);
        posCoef0 +=2;
        posCoef1 +=2;

    }

    return matriz;

}

//Gera os coeficientes ou detalhes do nível
vector<double> DwtHaar::transformar(vector<vector<double>> matriz, vector<double> vetor, int linhas, int colunas){

    vector<double> resultado;

    //Transforma o vector em matriz
    double matrizVetor[vetor.size()][1];

    for(int i = 0; i<vetor.size(); i++){
        for(int j =0; j<1;j++){
            matrizVetor[i][j] = vetor.at(i);
        }
    }

    //Transforma o vector matriz em matriz
    //double matriz2[matriz.size()][matriz.at(0).size()]; 

    //Multiplica as matrizes

    for (int i=0;i<linhas; i++) {
      for (int j=0; j<linhas; j++) {
        for (int k=0; k<colunas; k++) {

          double valor;
          //valor = matriz[i][k] * matrizVetor[k][j];
          valor = matriz.at(i).at(k) * matrizVetor[k][j]; 
          resultado.push_back(valor); 

        }

      }
    
    }

    return resultado;
}

//Executa a wavelet discreta de Haar 
void DwtHaar::executar(){
    

    //Transoformar o sinal no vector coeficientes
    /* for(int i=0; i<tamanhoSinal; i++){

        coeficientes.push_back(sinal[i]);

    } */

    while(coeficientes.size() > 1){

        nivel++;

        int nColunas = coeficientes.size();
        int nLinas = nColunas/2;

        vector<vector<double>>  matrizA;
        vector<vector<double>>  matrizD;

        //Fazer matriz passa-baixa
        matrizA = fazerMatrizTransf(nLinas,nColunas,filtroBaixo0,filtroBaixo1);

        //Fazer matriz passa-alta
        matrizD = fazerMatrizTransf(nLinas,nColunas,filtroAlto0,filtroAlto1);

        //gerar os coeficientes do nivel
        vector<double> coeficientesNivel;
        coeficientesNivel = transformar(matrizA, coeficientes, matrizA.size(),matrizA.at(0).size());

        //gerar os detalhes
        vector<double> detalheNivel;
        detalheNivel = transformar(matrizD, coeficientes, matrizD.size(),matrizD.at(0).size());
        detalhes.push_back(detalheNivel);

        //Atualizar os coeficientes
        coeficientes = coeficientesNivel;

    }

    //Montar saída
    vector<double> vetorSaida;
    vetorSaida.push_back(coeficientes.at(0));

    int tam;
    tam = detalhes.size();

    for(int i = tam-1; i > 0; i--){
        for(double valor : detalhes.at(i)){
            vetorSaida.push_back(valor);
        }
    }

    tam = vetorSaida.size();
    saida = (double*) malloc(tam * sizeof(double));
    tamSaida = tam;
    //double saida[tam]; //Retorno da função
    for(int i = 0; i < tam; i++){

        saida[i] = vetorSaida.at(i);

    }

}

int DwtHaar::getTamSaida(){
    return tamSaida;
}

double *DwtHaar::getSaida(){
    return saida;
}

void DwtHaar::inserirValor(double valor){

    coeficientes.push_back(valor);

}

