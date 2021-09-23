
#include <algorithm>//transform
#include <vector>
#include <cstdlib>
#include <functional>//partial_sum
#include <numeric> //bind2nd

//#include "Rnumgen.h"
using namespace std;
/*
rnumgen::rnumgen(int seedvalue, std::vector <int> &v) {//ADD SECOND ARG
  srand(seedvalue);
  F.resize(v.size() );
  partial_sum (v.begin(), v.end(), F.begin() );
  transform (F.begin(), F.end(), F.begin(), bind2nd (std::divides <float>(), F.back() )  );
  //set up CDF
}

int rnumgen::rand() const { 
    const double randmax = RAND_MAX + 1.0;
    const double p = (double) std::rand() /randmax;
    
    return upper_bound (F.begin(), F.end(), p) - F.begin();
 
}
*/