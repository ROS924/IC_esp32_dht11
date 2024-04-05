#include <iostream>
#include "TimeSeries.h"
#include "StatisticalMethods.h"
#include <math.h>
#include <algorithm>
#include "../../../../msys64/ucrt64/include/c++/13.2.0/bits/algorithmfwd.h"
using namespace std;

TimeSeries::TimeSeries(){
    //parametros
    list<double> observations;
    double mean;
    double stdev;
    double variance;
    double median;
    list<double> elements;
}

StatisticalMethods::StatisticalMethods(){}

//Getters e setters
list<double> TimeSeries::getObservations(){
    return observations;
}

void TimeSeries::setObservations(list<double> observations) {
    this->observations.clear();
    this->observations = observations;
}

double TimeSeries::getMean() {
    return mean;
}

void TimeSeries::setMean(double mean) {
    this->mean = mean;
}

double TimeSeries::getStdev() {
    return stdev;
}

void TimeSeries::setStdev(double stddev) {
    this->stdev = stddev;
}

double TimeSeries::getVariance() {
    return variance;
}

void TimeSeries::setVariance(double variance) {
    this->variance = variance;
}

double TimeSeries::getMedian() {
    return median;
}

void TimeSeries::setMedian(double median) {
    this->median = median;
}


void TimeSeries::calcMean(list<double> observations){
    double sum =0;

    for(double element : observations){
        sum += element;
    }

    double mean;

    mean = sum / observations.size();

	this->mean = mean; 
}
	
void TimeSeries::calcVariance(list<double> observations, double mean){

    double sum = 0;

    for(double element : observations){
        sum  += pow((element - mean), 2);
    }

    double variance;

    variance = sum / observations.size();

    this->variance = variance;
}

void TimeSeries::calcStdev(double variance){

    double stdev;

    stdev = sqrt(variance);

    this->stdev = stdev;		
}

void TimeSeries::calcMedian(list<double> elements){

    list<double> observations;
    int mean;
    double median;

    observations = elements;

    mean = ceil(observations.size()/2);

    sort(observations.begin(),observations.end());

    if((observations.size()%2) != 0 ){

        int i = 0;

        for(double element : observations){

            if(i == mean){
                median = element;
                break;
            }
        }

        this->median = median;
    }
    else{

        double element1, element2;

        for(double element : observations){

            int i = 0;           

            if(i == (mean-1)){
                element1 = element;
            }

            if(i == mean){
                element2 = element;
                break;              
            }
        }


        median = (element1 + element2) / 2;

        this->median = median;

    }

     		
}

void TimeSeries::basicStat(){
    this->calcMean(observations);
    this->calcVariance(observations, mean);
    this->calcStdev(variance);
    this->calcMedian(elements);
}

void TimeSeries::addElement(double element){
    this->observations.push_back(element);
}

void TimeSeries::copyElementList(list<double> elements){
    //LogFactory.getLog(this->getClass()).info("Copying all elements to time series");
    this->observations = elements;
}

/* void TimeSeries::setElement(int index, list<double> &observations ,double value){
    this->observations.splice(index, observations ,value);
} */

void TimeSeries::cleaningTimeSeries(){
    this->observations.clear();
    this->mean = 0;
    this->stdev = 0;
    this->variance = 0;
}

int TimeSeries::size(){
    return this->observations.size();
}

string TimeSeries::toString(){
    //LogFactory.getLog(this->getClass()).info("Function toString called");
    string Mean,Variance,Stdev,Median;

    Mean = to_string(this->mean);
    Variance = to_string(this->variance);
    Stdev = to_string(this->stdev);
    Median = to_string(this->median);
    
    string result = "\n\nMean: " + Mean;
    
    result += "\nVariance: " + Variance;
    
    result += "\nStdev: " + Stdev;
    
    result += "\nMedian: " + Median;
    
    result += "\nSize: " + this->observations.size();
    
    //result += "\nObservations: " + this->observations + "\n\n";
    result += "\n\n";
                    
    return result;
}