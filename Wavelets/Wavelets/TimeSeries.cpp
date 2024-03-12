#include <iostream>
#include "TimeSeries.h"
#include "StatisticalMethods.h"
using namespace std;

TimeSeries::TimeSeries(){
    //parametros
    list<double> observations;
    double mean;
    double stdev;
    double variance;
    double median;
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


void TimeSeries::calcMean(){
	this->mean = StatisticalMethods.calcMean(this->observations); 
}
	
void TimeSeries::calcVariance(){
    this->variance = StatisticalMethods.calcVariance(this->observations, this->mean);
}

void TimeSeries::calcStdev(){
    this->stdev = StatisticalMethods.calcStdev(this->variance);		
}

void TimeSeries::calcMedian(){
    this->median = StatisticalMethods.calcMedian(this->observations);		
}

void TimeSeries::basicStat(){
    this->calcMean();
    this->calcVariance();
    this->calcStdev();
    this->calcMedian();
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