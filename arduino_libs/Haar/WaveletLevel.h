#include <list>
using namespace std;

class WaveletLevel{

    public:

        WaveletLevel(){};

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