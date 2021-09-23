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


template <typename T>
int partition(std::vector<T> &A, int left, int right) {
	// sort: order left, middle and right elements
	int middle = (left+right)/2;

	if (A[middle] < A[left])  swap(A[middle], A[left]);
	if (A[right] < A[left])   swap(A[right], A[left]);
	if (A[right] < A[middle]) swap(A[right], A[middle]);

	if (right-left+1 <= 3)     return middle;  

	// select pivot: median-of-three
	int pindex = middle;
	T pivot = A[pindex];

	// partition A: {<=}, {pivot}, {=>}
	swap(A[pindex], A[right-1]);

	int i = left;
	int j = right-1;

	while (1) {
		while (A[++i] < pivot) { }    
		while (pivot < A[--j]) { }
		if (i>=j) break;
		swap(A[i], A[j]);
	}  

	pindex = i;  
	swap(A[pindex], A[right-1]);  

	return pindex;
}


//initial quicksort
template <typename T>
void quicksort (vector<T> &A, int left, int right) {
	if (left < right) {
		int pindex = partition (A, left, right);//pivot index
		quicksort (A, left, pindex-1);
		quicksort (A, pindex + 1, right);
	}
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
	else if (argc == 5){ 
		int k_0 = atoi(argv[2]);
		int k_1 = atoi(argv[3]);
		if (k_0 < 0) k_0 = 0;
		//k1 not checked ->check later.
	}

	fin.open (argv[4]);//open file.txt 

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
