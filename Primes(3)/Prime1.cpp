#include <iostream>
#include <math.h>
using namespace std;

bool isprime(int number) {
  if (number ==0 || number ==1) {			return false;}
	int square_root = sqrt(number);
	for (int i=2; i<number; i++){
		if (number %i == 0){return false;}
		}

	return true;
}

int main(){
	int number;
	while ( cin>>number ) {
	  if (isprime(number)){
	    cout << number <<" is a prime number"<<'\n';}
	}
}
