#include <list>
#include "WaveletLevel.h"
#include "WaveletsInterface.h"
#include "TimeSeries.h"
using namespace std;


class Haar: public WaveletsInterface{

    public:
        Haar(){};

        void pyramid(TimeSeries ts){};
        int getlevels(){};    
        void setlevels(int levels){};
        list<double> getCoefficientLevel(int level){};                    
        list<double> getDetailLevel(int level){};
        bool allowedLevel(int level){};
	
	 list<WaveletLevel> getTransforms() {};

	 void setTransforms(list<WaveletLevel> transforms) {};
	
	 list<double> inverse(int level){};

	
	 virtual list<double> inverse() {};

	
	 virtual void transform(TimeSeries ts) {};

	
	 virtual list<double> getCoefficients() {};

	
	 virtual list<double> getDetails() {};                            


    private:
        list<WaveletLevel> transforms;
        int levels;

};