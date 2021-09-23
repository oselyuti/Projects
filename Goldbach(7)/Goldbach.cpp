#include <iostream>
#include <set>
#include <cmath> //for square root
#include <algorithm>
#include <vector>

using namespace std;

class prime_partition {
  public:
    prime_partition();// constructor
    void operator() (int);// operator(int)

  private:
    void create_pset();
    void compute_partitions(vector<int> & ,  const int& , int = 0);// compute_partitions
    void print_partitions();// print_partitions

    set<int> pset;
	int pset_min, pset_max;//min/max prime in a set
	unsigned int max_terms;//max # of primes in a sum

	vector <vector<int> > partitions;

};

prime_partition::prime_partition () {//ready to work
	create_pset();}

void prime_partition::operator() (int num) {//working
	if (pset_min<2 || pset_max>2000 ) {
		return;}
	if (num%2==0){//even
		max_terms = 2;}// if even, the sum of the num will be only two numbers
	else{//if num odd
		max_terms = 3;}//can be one if prime

	partitions.clear();//clear everything from the previous request to compute the sums for the num

	vector <int> temp;
	//unleashing recursion with a fucntion call
	//3rd argument for sum is optional here and it is used through recursion only
	//initially, sum is 0
	compute_partitions(temp, num );//num is the target since we are trying to get all the numbers to add up to num


	print_partitions();
}

void prime_partition::create_pset(){//works	
		pset.clear();//remove all elements, leaving the container with a size of 0
		pset.insert(2);
		for (int i=2; i < 2000; i++){//add to the list's end if prime
			int remainder =0;
			int square_root = sqrt(i);
			for (int j=2; j<=square_root+1; j++){//if the number is prime
				remainder = i % j;
				if (remainder == 0 ){//not prime
					break;}
			}
			if (remainder != 0){pset.insert(i);}
		}
		pset_min = *(pset.begin());
		pset_max = *(pset.end());
}

void prime_partition::print_partitions(){//works
	for (unsigned int i=0; i<partitions.size(); i++){
		for (unsigned int j =0; j<partitions.at(i).size(); j++){
			cout<<partitions.at(i).at(j)<<" ";
		}
		cout<<'\n';
	}
}

void prime_partition::compute_partitions(vector<int> & vec,  const int& target, int sum){// compute_partitions

	if (sum == target ) {
		if (vec.size() < max_terms){ //new partition was found with less terms
			max_terms = vec.size();//reset partitions to look for the smaller number of partitions that compose the sum
			partitions.clear();
		}
		if (vec.size()==max_terms){//necessary because changing the max terms alone will not stop 
								   //it from adding the vector with too many terms to the partitions (must NOT happen)
			partitions.push_back(vec);//add the vector to the final list only if size <= max_terms
			return;
		}
	}

	else if (sum > target || vec.size() > max_terms) {//failed attempt
		return;}

	set<int>::iterator start = pset.begin();
	set<int>::iterator end;
	set<int>::iterator it;//iterator to traverse the prime set

	//setting the end according to the condition:
	//if vector vec is empty (no prime numbers tried yet)  ->  end will be what comes after the target
	//
	//if vector vec is not empty (if we are currently trying to insert something)  ->end is the last number in vec. WHy?
	//because we have tried everything that came before that number already.
	
	if (vec.empty()){
		end = upper_bound (start, pset.end(), target); }//the iterator to the prime element after target -> out of reach for the loop (< condition, not <=)
	else {
		end = upper_bound (start, pset.end(), vec.back() ); }//iterator to the first prime element after the last number already in the vector vec


	//checking if the target is prime and if recursion is even necessary
	for (it = start; it != end; it++){
		if (target == *it) { 
			vec.push_back(target);
			partitions.push_back( vec ); 
			return;}
	}

	//execute this if still looking for a correct partition
	for (it = start; it != end; it ++){
		vec.push_back(*it);
		compute_partitions (vec, target, *it+sum);
		vec.pop_back();//?
	}		
}

	

int main(int argc, char *argv[])
{
  prime_partition goldbach;

  int number;
  while (1) {
	cout << "number = ";
	cin>>number;
    /*if (!(cin >> number) ) {
		cout<<"input an integer"<<'\n';
		cin.clear();
		cin.ignore(200, '\n');
	}*/
	if (cin.eof())
	  break;
	
	goldbach(number);
  }

  cout << "\n";
}
