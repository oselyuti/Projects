#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <sstream>
#include <cstdlib>
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
	// partition A: {<=}, {pivot}, {=>}

	int pindex = left + rand() % (right-left+1);
	T pivot = A[pindex];
	swap (A[pindex], A[right]);//the pivot is now the rightmost element
	
	int i = left-1;//will be incremented and then swapped (if left = 0 -> i will be 0 when access A[i] is needed)  
	for (int j= left; j < right; j++ ){//will step through the partitioned array and find the position for the pivot
		if (A[j] < pivot){//start building array from the beggining.
			i++;//i(smaller element index) is now updated to the currently unsorted element ready to be substituted
			swap (A[i], A[j]);//puts the jth element below pivot in a free spot.
		}
	}//when the for loop is done, all the values below pivot should be less than the pivot.
	// less than pivot < pivot <= greater than/equal to  pivot
	
	//Now I put pivot after all the elements less than or equal to the pivot (i+1)
	swap (A[i+1], A[right] );
	pindex = i+1;//new CORREct position for pivot.
	return pindex;
	

}


//unchecked partition with while loops and bound checks/rand pivot
int partition(std::vector<T> &A, int left, int right) {
	int pindex = left + rand() % (right - left +1 );
		T pivot = A[pindex];
		swap (A[pindex], A[right]);
		
		while (1){
		while (A[i+1] && A[++i] < pivot){};
		while (A[j-1] && A[--j] > pivot){};
		if (!A[i+1] || !A[j-1]) break;

		swap (A[i], A[j]);
		}
   
		pindex = i;
		swap (A[li], A[pindex]);
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
		//int k_0 = atoi(argv[2]);
		//int k_1 = atoi(argv[3]);
		//if (k_0 < 0) k_0 = 0;
		//k1 not checked ->check later

	fin.open (argv[4]);//open file.txt 
	}

	string mode = string (argv[1]);

	vector<data> A;

	data din;
	while (fin >> din){
		A.push_back(din);
	}

	fin.close();//close file.txt
	int N = (int) A.size();

	
	if (mode == "-stl" ) {
		sort(A.begin(), A.end());
	}
	else if (mode == "-rpivot"){ //use my own quicksort
		quicksort (A, 0, (N-1)  );//N-1 is last index
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
