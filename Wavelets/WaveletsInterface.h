#include "TimeSeries.h"
#include <list>

class WaveletsInterface{

    public:
        virtual void transform(TimeSeries ts);
	
        virtual list<double> inverse();
        
        virtual list<double> getCoefficients();
        
        virtual list<double> getDetails();

};