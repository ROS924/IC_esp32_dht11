#include <iostream>
#include <list>
#include "WaveletLevel.h"
using namespace std;

WaveletLevel::WaveletLevel(){
    list<double> coefficients;
	list<double> details;
};

    

list<double> WaveletLevel::getCoefficients() {
    return coefficients;
}

void WaveletLevel::setCoefficients(list<double> coefficients) {
    this->coefficients = coefficients;
}

list<double> WaveletLevel::getDetails() {
    return details;
}

void WaveletLevel::setDetails(list<double> details) {
    this->details = details;
}

void WaveletLevel::addCoefficient(double c){
    this->coefficients.push_back(c);
}

void WaveletLevel::addDetail(double d){
    this->details.push_back(d);
}