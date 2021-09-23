#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <sstream>
#include <cstdlib>
//Made by ALEX SELYUTIN

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
	// outputs data like a known object cout<<data now possible
}

void printlist(vector <data>::iterator begin, vector<data>::iterator end) { 
	for (vector<data>::iterator it = begin; it != end; it++){
		cout<<*it;}
}


//final quicksort with partitioning inside
template <typename T>
void quicksort (vector<T> &A, int left, int right) {
	if (left < right) {

		int pindex = left + rand() % (right-left+1);
		T pivot = A[pindex];
		swap (A[pindex], A[right]);//the pivot is now the rightmost element

		//it is difficult for me to easily understand Gregor's while loops with pre-increments when I have to check the boundaries. for loop works
		//in a similar way
		int i = left-1;//will be incremented and then swapped (if left = 0 -> i will be 0 when access A[i] is needed)  
		for (int j= left; j < right; j++ ){//will step through the partitioned array and find the position for the pivot
			if (A[j] < pivot){//start building array from the beggining.
				i++;//i(smaller element index) is now updated to the currently unsorted element ready to be substituted
				swap (A[i], A[j]);//puts the jth element below pivot in a free spot.
			}
		}//when the for loop is done, all the values below pivot should be less than the pivot.
		// less than pivot < pivot <= greater than/equal to  pivot

		//Now I put pivot after all the elements less than or equal to the pivot (i+1)
		pindex = i+1;//new CORREct position for pivot.
		swap (A[right], A[pindex] );

		//partition the two parts of the vector recursively
		quicksort (A, left, pindex-1);
		quicksort (A, pindex + 1, right);
	}
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

template <typename T>
void quickselect(vector <T> &A, int left, int k, int right ) {//li = last index
	while (1){
		//if partition function were to be implemented in place, the same code from the function can be
		//pasted in place righ here just like i did in quicksort. The write up did not specify the requirement to implement
		//the partitioning in place.
		int pindex = partition (A, left, right);//left part

		if (pindex == k){ //everything below k_0 is ok
			break;
		}
		if (k < pindex) { right = pindex -1;}
		else { left = pindex+1;}
	}
}


int main(int argc, char *argv[]) {
	// perform command-line error checking
	if ( (argc != 3 && argc != 5) ){
		cerr <<"usage: ./Qsort -stl | -rpivot [k0 k1] file.txt"<<endl;
		return 0;}

	ifstream fin;
	int k_1, k_0;
	if (argc == 3) fin.open (argv[2]); //-stl or -rpivot (no args)
	else if (argc == 5){ 
		k_0 = atoi(argv[2]);
		k_1 = atoi(argv[3]);
		if (k_0 < 0) k_0 = 0;//readjust the bottom index to the min
		//k1 not checked ->check later
		fin.open (argv[4]);//open file.txt 
	}

	string mode = string (argv[1]);
	if (mode != "-stl" && mode != "-rpivot" ) {
		cerr <<"usage: ./Qsort -stl | -rpivot [k0 k1] file.txt";
		return 0;}

	vector<data> A;

	data din;
	while (fin >> din){
		A.push_back(din);
	}

	fin.close();//close file.txt
	int N = (int) A.size();


	if (mode == "-stl" ) {
		sort(A.begin(), A.end());//std
	}
	else if (mode == "-rpivot"){ //use my own quicksort
		if (argc == 3) quicksort (A, 0, (N-1)  );//N-1 is last index

		else if (argc == 5){
			int N = (int) A.size();
			int last_index = N-1;
			if (k_1 > last_index ){//check if k1 > N-1
				k_1 = last_index;} // adjust last index when user's request too high


			// to ensure that A[0:k0-1] <= A[k0:k1] <= A[k1+1:N-1]
			quickselect (A, 0, k_0, last_index) ;
			quickselect (A, k_0, k_1, last_index);
			quicksort (A, k_0, k_1);
		}
	}

	printlist(A.begin(), A.end());
	return 1;
}
