// include header file(s) needed
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <sstream>
#include <cstdlib>
using namespace std;

#include "Slist.h"

// modify the data class and the associated input 
// and output operators to store, set and print a 
// line number identifier called ID

class data {
	public:
		friend istream & operator>>(istream &, data &); 
		friend ostream & operator<<(ostream &, const data &);
		bool operator < (const data &dat) const{	// add operator< using lastname, firstname, phone number
			if (lastname < dat.lastname) return true;

			else if (lastname == dat.lastname) {
				if (firstname < dat.firstname) return true;
				else if (firstname == dat.firstname) {
					if (phonenum < dat.phonenum){
						return true;}
					else {return false;} // == || >
				}
			}
			return false;//last name > dat.lastname
		}

		data (){ID = 0;}

	private:
		string firstname;
		string lastname;
		string phonenum;
		int ID;
};

istream & operator>>(istream &in, data &r) { 
	in>> r.firstname >> r.lastname >> r.phonenum;// write this to read data object data
	r.ID++;
	return in;
}

ostream & operator<<(ostream &out, const data &dat) {
	string lf = " " + dat.lastname + " "+dat.firstname;
	out <<setw(25)<<left<<lf
		<<dat.phonenum
		<<setw(9)<<right<<dat.ID
		<<endl; 		
	return out;	
	// outputs data like a known object cout<<data now possible
}

void printlist(slist<data>::iterator begin, slist<data>::iterator end) { 
	for (slist<data>::iterator it = begin; it != end; ++it){
		cout<<*it;}
}

int main(int argc, char *argv[]) {
	// copy command-line check from Qsort.cpp,
	// then modify to apply to this assignment
	if ( argc != 3){
		cerr <<"usage: ./Slist -mergesort|-quicksort repeat.txt"<<endl;
		return 0;}

	ifstream fin;
	if (argc == 3) fin.open (argv[2]); //open input file

	string mode = string (argv[1]);
	if (mode != "-mergesort" && mode != "-quicksort" ) {
		cerr <<"usage: ./Slist -mergesort|-quicksort repeat.txt";
		return 0;}


	slist<data> A;//slist<data>A;

	data din;

	while (fin >> din ){ 
		A.push_back(din);}
	fin.close();// close input file

	A.sort(mode);

	printlist(A.begin(), A.end());
}
