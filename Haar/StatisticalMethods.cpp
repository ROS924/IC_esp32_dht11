#include <iostream>
#include "StatisticalMethods.h"
#include <math.h>
#include <algorithm>
using namespace std;

StatisticalMethods::StatisticalMethods(){}

double StatisticalMethods::calcMean(list<double> observations){

    double sum =0;

    for(double element : observations){
        sum += element;
    }

    double mean;

    mean = sum / observations.size();

    return mean;

}

double StatisticalMethods::calcVariance(list<double> observations, double mean){

    double sum = 0;

    for(double element : observations){
        sum  += pow((element - mean), 2);
    }

    double variance;

    variance = sum / observations.size();

    return variance;

}



double StatisticalMethods::calcStdev(double variance){

    double stdev;

    stdev = sqrt(variance);

    return stdev;

}

double StatisticalMethods::calcMedian(list<double> elements){

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

        return median;
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

        return median;

    }

}