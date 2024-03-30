#include <list>
#include "TimeSeries.h"
using namespace std;

class WaveletLevel{

    public:

        WaveletLevel(){};

        void transform(TimeSeries ts);
	
        list<double> inverse();
        list<double> getCoefficients();
        list<double> getDetails();
        void setCoefficients(list<double> coefficients);
        void setDetails(list<double> details);        
        void addCoefficient(double c);        
        void addDetail(double d);

    private:
        list<double> coefficients;
	    list<double> details;

};