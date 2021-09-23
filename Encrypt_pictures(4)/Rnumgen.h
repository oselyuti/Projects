#ifndef __RNUMGEN_H__
#define __RNUMGEN_H__

#include <algorithm>
#include <vector>
#include <cstdlib>
#include <functional>
#include <numeric>

using namespace std;


class rnumgen {
  public:
    rnumgen(int seedvalue, std::vector <int> &v ) ;
    int rand() const;

  private:
    std::vector <float> F;
};


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




#endif
