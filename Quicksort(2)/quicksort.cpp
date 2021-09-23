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

template <typename T>
void quicksort (vector<T> &A){
  quicksort (A,0, A.size()-1);
  }
    
  
  int main(int argc, char *argv[]) {
	// perform command-line error checking
	if ( (argc != 3 && argc != 5) ){
		cerr <<"usage: ./Qsort -stl | -rpivot [k0 k1] file.txt"<<endl;
		return 0;}

	ifstream fin;
	int k_1, k_0;
	if (argc == 3) fin.open (argv[2]); //-stl or -rpivot (no args)

	string mode = string (argv[1]);
	if (mode != "-stl" && mode != "-rpivot" ) {
		cerr <<"usage: ./Qsort -stl/-own | file.txt";
		return 0;}

	vector<int> A;

	int din;
	while (fin >> din){
		A.push_back(din);
	}

	fin.close();//close file.txt
	int N = (int) A.size();


	if (mode == "-stl" ) {
		sort(A.begin(), A.end());//std
	}
   if (mode == "-own"){//...
   }

	//printlist(A.begin(), A.end());
	return 1;
}