#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <sstream>
// include header files needed
using namespace std;

class data {
	public:
		// add operator< using lastname, firstname, phone number

		friend istream & operator>>(istream &, data &);
		friend ostream & operator<<(ostream &, const data &);
		bool operator < (const data &dat) const{
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

	private:
		string firstname;
		string lastname;
		string phonenum;
};

istream & operator>>(istream &in, data &r) { 
	in>> r.firstname >> r.lastname >> r.phonenum;// write this to read data object data
	return in;
}

ostream & operator<<(ostream &out, const data &dat) {
	string lf = dat.lastname + ", "+dat.firstname;
	out <<setw(24)<<left<<lf
		<<dat.phonenum<<endl; 		
	return out;	
	// write this to write data object data
}

//template <typename Tdata>
//void quicksort(...) { 
// write this 
//}

//template <typename Tdata>
//void quickselect(...) { 
// write this 
//}
void printlist(vector <data>::iterator begin, vector<data>::iterator end) { 
	// see Lab 1
	for (vector<data>::iterator it = begin; it != end; it++){
		cout<<*it;}
}

int main(int argc, char *argv[]) {
	// perform command-line error checking
	if (argc != 3 && argc != 5){
		cerr <<"usage: ./Qsort -stl | -rpivot [k0 k1] file.txt";
		return 0;}

	ifstream fin;
	if (argc == 3) fin.open (argv[2]);
	else if (argc == 5) fin.open (argv[4]);//open file.txt 

	string mode = string (argv[1]);

	vector<data> A;

	data din;
	while (fin >> din){
		A.push_back(din);
	}

	fin.close();//close file.txt

	if (mode == "-stl" ) {
		sort(A.begin(), A.end());
	}

				/*else {
				  if (sorting option -rpivot) {
				  int N = (int)A.size();

				  int k0 = 0;
				  int k1 = N-1;

				// if specified, update k0, k1 and apply quickselect 
				// to ensure that A[0:k0-1] <= A[k0:k1] <= A[k1+1:N-1]

				quicksort(A, k0, k1);
				}
				*/

				printlist(A.begin(), A.end());
				}
