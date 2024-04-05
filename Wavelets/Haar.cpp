#include <iostream>
#include "Haar.h"
#include "WaveletLevel.h"
#include "WaveletsInterface.h"
using namespace std;

Haar::Haar(){   
    list<WaveletLevel> transforms;
    int levels;
};

WaveletLevel inter;

void Haar::pyramid(TimeSeries ts){
    list<double> coefBase, detBase;
    
		do{
						
			if(this->levels == 0){
				coefBase = ts.getObservations();
				detBase = ts.getObservations();
			}else{
                list<WaveletLevel>::iterator it = this->transforms.begin();
                advance(it,(this->levels - 1));
                WaveletLevel intermed = *it; 
				coefBase = intermed.getCoefficients();
				detBase = intermed.getDetails();
			}
			
			this->levels++;
			
			WaveletLevel newLevel = WaveletLevel();
			
			for(int i = 0; i <= coefBase.size()-2; i +=2){
                list<double>::iterator itI = coefBase.begin();
                advance(itI,(i + 1));
                double intermed = *itI;

                list<double>::iterator iti = coefBase.begin();
                advance(iti,(i));
                double intermedi = *iti;

                list<double>::iterator itB = detBase.begin();
                advance(itB,(i + 1));
                double intermedB = *itB;

                list<double>::iterator itb = detBase.begin();
                advance(itb,(i));
                double intermedb = *itb;

				newLevel.addCoefficient((intermed + intermedi)/2);
				newLevel.addDetail((intermedB - intermedb)/2);
			}
			
			this->transforms.push_back(newLevel);

            list<WaveletLevel>::iterator itt = this->transforms.begin();
            advance(itt,(this->levels - 1));
            inter = *itt; 
			
		}while(inter.getDetails().size() > 1);
}

int Haar::getlevels(){
    return this->levels;
}

void Haar::setlevels(int levels){
    this->levels = levels;
}

list<double> Haar::getCoefficientLevel(int level){
    if(!allowedLevel(level)){
        list<double> nulo;
        return nulo;
    }
			
        list<WaveletLevel>::iterator it = this->transforms.begin();
        advance(it,(this->levels - 1));
        WaveletLevel intermed = *it; 
		
		return intermed.getCoefficients();
}

list<double> Haar::getDetailLevel(int level){
    if(!allowedLevel(level)){
        list<double> nulo;
        return nulo;
    }

    list<WaveletLevel>::iterator it = this->transforms.begin();
    advance(it,(this->levels - 1));
    WaveletLevel intermed = *it; 
    
    return intermed.getDetails();
}

bool Haar::allowedLevel(int level){
    if(level >= this->levels || level < 0){
			return false;
		}
		return true;
}

list<WaveletLevel> Haar::getTransforms(){
    return transforms;
}

void Haar::setTransforms(list<WaveletLevel> transforms){
    this->transforms = transforms;
}

list<double> Haar::inverse(int level){

    list<double> signal ;
    list<double> coefficients, details;
    
    if(!allowedLevel(level)){
        list<double> fod;
        return fod;
    }
        
    
    coefficients = this->getCoefficientLevel(level);
    details = this->getDetailLevel(level);
    
    for(int i = 0; i < coefficients.size(); i++){

        list<double>::iterator it = coefficients.begin();
        advance(it, i);
        double intermed = *it;

        list<double>::iterator itt = details.begin();
        advance(itt, i);
        double intermedt = *itt; 

        signal.push_back(intermed - intermedt);
        signal.push_back(intermed + intermedt);
    }
    
    return signal;

}

list<double> Haar::inverse(){
    return inverse(this->levels-1);
}

void Haar::transform(TimeSeries ts){
    pyramid(ts);
}

list<double> Haar::getCoefficients(){
    return getCoefficientLevel(getlevels() - 1);
}

list<double> Haar::getDetails(){
    return getDetailLevel(getlevels() - 1);
}

