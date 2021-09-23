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

	//using binary_search() to see if the number is in the prime list
	bool found = binary_search (allPrime.begin(), allPrime.end(), number) ;
	if (found){
		return true;}
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



int random_number()
{
	return (rand()%140 +1);
}



int main(int argc, char *argv[])
{
	int N=0;	
	if (argc == 1){
		N = 140;
		}
	if (argc == 2){
		N = atoi ( argv[1] );//treats the command line argument as int
    }
	srand(N);

	vector< int > nums;
	vector< bool > result;//will convert the numbers into true/false indicators of prime/not prime.
 
    nums.resize (N);
    result.resize(N);
  

	generate( nums.begin(),nums.end(), random_number );
	transform( nums.begin(), nums.end(), result.begin(), isprime() );

	int c = count( result.begin(), result.end(), true );
	
	cout << "Sequence contains "<<c<<" prime numbers."<<'\n';
}
