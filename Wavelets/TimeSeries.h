#include <list>
#include <string>
using namespace std;

class TimeSeries{
    
    public:
        //Inicializa classe
        TimeSeries(){};

        //Getters e setters
        list<double> getObservations();
        void setObservations(list<double> observations);
        double getMean();
        void setMean(double mean);
        double getStdev();
        void setStdev(double stddev);
        double getVariance();
        void setVariance(double variance);
        double getMedian();
        void setMedian(double median);


        // métodos gerais
        void calcMean();
        void calcVariance();
        void calcStdev();
        void calcMedian();
        void basicStat();
        void addElement(double elements);
        void copyElementList(list<double> elements);
        void setElement(int index, list<double> &observations ,double value);
        void cleaningTimeSeries();
        int size();
        string toString();


    private:

        list<double> observations;
        double mean;
        double stdev;
        double variance;
        double median;

};