#include <iostream>
#include "dwt_haar.h"
using namespace std;

int main(){

double sinal [32]= {37.0,37.0,36.0,36.0,36.0,36.0,37.0,37.0,38.0,39.0,40.0,42.0,44.0,48.0,50.0,51.0,52.0,53.0,54.0,53.0,53.0,53.0,52.0,51.0,51.0,50.0,49.0,48.0,48.0,47.0};


DwtHaar* haar = new DwtHaar();

for(int i=0; i<32;i++){
    haar->inserirValor(sinal[i]);
}

haar->executar();

int tamanhoSaida;
tamanhoSaida = haar->getTamSaida();

double *saida;

saida = haar->getSaida();

cout<<"Exibir resultado da wavelet Haar: "<<endl<<endl;

for(int i = 0; i < tamanhoSaida; i++){
    cout<<saida[i]<<" ";
}
cout<<endl;

}