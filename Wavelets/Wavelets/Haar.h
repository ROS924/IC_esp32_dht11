#include <list>
#include "WaveletLevel.h"
using namespace std;


class Haar{

    public:
        Haar(){};
    


    private:
        list<WaveletLevel> transform;
        int level;

};