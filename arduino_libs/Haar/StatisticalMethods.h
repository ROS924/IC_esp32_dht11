#include <list>
#include <string>
using namespace std;

class StatisticalMethods{

    public:
        static double calcMean(list<double> observations);
        static double calcVariance(list<double> observations, double mean);
        static double calcStdev(double variance);
        static double calcMedian(list<double> elements);
};