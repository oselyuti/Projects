#include <algorithm>
#include <iostream>
#include <cmath>
#include <list>
#include <vector>
using namespace std;

class isprime {
  public:
    isprime() { allPrime.push_back(2);}
    bool operator()(int);
	void expand_list (int N);//adds all the prime numbers to the allPrime list up to N

  private:
	list <int> allPrime;
    
};




bool isprime::operator()(int number){

	//check if the number is already in the list
	
	if (number > allPrime.back() ){//need to expand the list
		expand_list (number);
	}

	//using find() to see if the number is in the prime list
	
	//list<int>::iterator it ;
  bool found = binary_search (allPrime.begin(), allPrime.end(), number);
	//it =find (allPrime.begin(), allPrime.end(), number);
	//if (*it!=number){ return false;}
	if (found){return true;}
  else {return false;}


}


void isprime::expand_list (int number){
	//if the list's last element is less than number at hand
	//expand the list for that many numbers after the last element
	
		for (int i=allPrime.back()+1 ; i <= number; i++){//add to the list's end if prime
			int remainder =0;
			int square_root = sqrt(i);
			for (int j=2; j<=square_root+1; j++){//if the number is prime
				remainder = i % j;
		
				if (remainder == 0 ){//not prime
					break;}
			}
				if (remainder != 0){allPrime.push_back(i);}
			}
	}

int main()
{
	isprime pcheck;
	int number;

	while ( cin>>number ) {
	  if (pcheck(number))
	    cout << number<<" is a prime number"<<'\n';
	}
}

//stopped at functor implementation. 
