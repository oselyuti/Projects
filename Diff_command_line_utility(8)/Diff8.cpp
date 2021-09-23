#include <iomanip>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

	// class for storing 2D array
template <typename T>
class matrix {
	int Nrows, Ncols;
	T **data;

	public:
		matrix() {data = NULL;}
		~matrix() {
			if (data){
				if (data[0]) delete [] data[0];
				delete [] data;
				data = NULL;
			}
		}
		void assign (int N_rows, int N_cols, const T &value = T()) {
			Nrows = N_rows;
			Ncols = N_cols;
			
			data = new T *[Nrows];
			data [0] = new T [Nrows*Ncols];
			for (int i=1; i < Nrows; ++i) {
				data[i] = data[i-1] + Ncols;}
			for (int i=0; i < Nrows; ++i){
				for (int j=0; j<Ncols; ++j){
					data[i][j] = value;
				}
			}
		}
			int get_Nrows() const {return Nrows;}
			int get_Ncols() const {return Ncols;}
			T * operator[] (int i) {return data[i];}
};


//LCS
class LCS {
	public:
		LCS () {size1 =0; size2=0;lcs_length =0;}
		void text1_push_back(string &filename1);
		void text2_push_back(string &filename2);
		void compute_alignment();
		void report_difference(bool print); 

		unsigned int size1, size2; int lcs_length;//for verbose
		void print_details () ;//for verbose in main

	private:
		// support functions
		stack <int> stack_rows;
		stack <int> stack_cols;
		int subst_cost (int i, int j);
		matrix<int> cost;//edit info
		matrix<int> link;//alignment info
		vector <string> text_1;// text1 buffer
		vector <string> text_2;// text2 buffer
		
		void report_difference(unsigned int &row,unsigned int &col, stack <int> &s_row, stack<int> &s_col, int &length);
		void parse_and_print ();//general function
		void print_difference (vector <int> &inserted_rows, vector <int> &deleted_rows, vector <int> &inserted_cols,
		   vector <int> &deleted_cols, unsigned int i=0);//specific for printing the lines and corresponding edit operations
};




int LCS::subst_cost (int i, int j) {
	if (text_1.at(i) == text_2.at(j) ) return 0;
	else {return 999;}
}

void LCS::print_details() {
		unsigned int i,j;
		cout<<"alignment cost = " <<cost[size1] [size2]<<'\n';

		cout <<"cost matrix:"<<'\n';
		for (i=0; i <= size1; i++) {
			for (j=0; j <= size2; j++){
				cout<<right<<setw(4)<< cost[i][j];
			}
			cout<<'\n';
		}

		cout <<"link matrix:"<<'\n';
		for (i=0; i <= size1; i++) {
			for (j=0; j <= size2; j++){
				cout<<right<<setw(4)<< link[i][j];
			}
			cout<<'\n';
		}
}


void LCS::report_difference(bool print = false){ 
	stack_rows.push (size1); stack_cols.push(size2);
	report_difference(size1, size2,stack_rows,stack_cols, lcs_length);//gives the trail but does not yet print
	parse_and_print();//output edits to make file 1 into file 2
	if (print) cout<<"lcs length: "<<lcs_length<<'\n';	
}


void LCS::print_difference (vector <int> &inserted_rows, vector <int> &deleted_rows, vector <int> &inserted_cols,
		vector <int> &deleted_cols, unsigned int i){

	if (!inserted_rows.empty() && !deleted_rows.empty()) {//change
		cout<<deleted_rows.front();

		if (deleted_rows.front() != deleted_rows.back() ) {cout<<"," << deleted_rows.back() ;}

		cout<< "c"<< inserted_cols.front();

		if (inserted_cols.front() != inserted_cols.back() ) {cout<<"," << inserted_cols.back() ;}
		cout<<'\n';

		for (i=0; i < deleted_rows.size(); i++){
			cout<< "< "<< text_1.at (deleted_rows.at(i) -1 ) <<'\n';}

		cout<<"---"<<'\n';
		for (i=0; i < inserted_cols.size(); i++){
			cout<< "> "<< text_2.at (inserted_cols.at(i) -1 ) <<'\n';
		}
	}

	else if (!inserted_rows.empty() && deleted_rows.empty()){ //insertion
		cout<< inserted_rows.front() <<"a" <<	inserted_cols.front() <<'\n';			
		for (i=0; i < inserted_rows.size(); ++i) {
			cout<<"> " << text_2.at (inserted_cols.at(i) -1)<<'\n';
		}

	}

	else if (!deleted_cols.empty() && inserted_cols.empty()){//deletion
		cout<<deleted_rows.front();

		if (deleted_rows.front() != deleted_rows.back() ) {cout<<"," << deleted_rows.back() ;}

		cout <<"d" <<deleted_cols.front()<<'\n';


		for (i=0; i < deleted_rows.size(); i++){
			cout<<"< " << text_1.at (deleted_rows.at(i) -1)<<'\n';}

	}

	inserted_rows.clear(); inserted_cols.clear();
	deleted_rows.clear(); deleted_cols.clear();
}



void LCS::parse_and_print() {
	vector <int> inserted_rows;vector <int> deleted_rows;
	vector <int> inserted_cols;vector <int> deleted_cols;
	/*cout<<"rows to target:"<<'\n';
	  while (!stack_rows.empty()) {cout<<right<<setw(3)<<stack_rows.top()<<" "; stack_rows.pop();}
	  cout<<'\n'<<"cols to target:"<<'\n';
	  while (!stack_cols.empty()) {cout<<right<<setw(3)<<stack_cols.top()<<" "; stack_cols.pop();}
	  */	
	while (1){
		if ( (!stack_rows.empty() &&link[stack_rows.top()][stack_cols.top()] == 4) || stack_rows.empty() ) {
			print_difference (inserted_rows, deleted_rows, inserted_cols, deleted_cols);
			inserted_rows.clear(); inserted_cols.clear();
			deleted_rows.clear(); deleted_cols.clear();
		}

		//check if stack has elements to continue
		if (stack_rows.empty() ) {break;}

		else if (link[stack_rows.top()][stack_cols.top()] == 2) {//insertion 
			inserted_rows.push_back (stack_rows.top());
			inserted_cols.push_back (stack_cols.top());
			//cout<<"> "<<stack_rows.top()<<"add"<<stack_cols.top() <<text_2.at (index_text2) <<'\n';
		}			

		else if (link[stack_rows.top()][stack_cols.top()] == 1) {//deletion
			deleted_rows.push_back (stack_rows.top());
			deleted_cols.push_back (stack_cols.top());
			//cout<<"< "<<stack_rows.top()<<"d"<<stack_cols.top() <<text_1.at (stack_rows.top()-1) <<'\n';
		}			

		stack_cols.pop();
		stack_rows.pop();
	}
}

void LCS::report_difference(unsigned int &row, unsigned int &col, stack <int> &s_row, stack<int> &s_col, int &lcs_length){
	//int c = cost[row][col];//start from the final cost
	//base case
	if (row ==0 && col == 0) { return;}//can also do link[row][col] == 0;

	if (link[row][col]  == 4 ) {//diagonal path to the prev cell
		s_row.push (row-1) ;
		s_col.push(col-1);
		row--; col--;
		lcs_length++;
	}
	else if (link[row][col] == 1) {//vertical == go up one
		s_row.push (row-1); s_col.push(col);
		row--;//go up //edit_num++;
	}
	else if (link[row][col] == 2) {//horizontal = go left once
		s_row.push (row); s_col.push(col-1);
		col--;//edit_num++;
	}

	report_difference (row, col, s_row, s_col, lcs_length);
}



void LCS::compute_alignment(){
	size1 = text_1.size(); 	size2 = text_2.size();
	int rows = size1; int cols = size2;

	cost.assign (rows+1, cols+1);
	link.assign (rows+1, cols+1);

	const int vertical = 1; const int horizontal = 2; const int diagonal = 4;

	cost[0][0] = 0; link[0][0] = 0;

	for (int i=1; i <= rows; ++i) {
		cost[i][0] = cost [i-1][0] + 1; //deletion cost is one
		link[i][0] = vertical;}
	for (int j=1; j <=cols; ++j){
		cost[0][j] = cost [0][j-1] +1;
		link[0][j] = horizontal;}
	for (int i=1; i <= rows; i++){
		for (int j=1; j<=cols; ++j) {
			int substitution_cost = subst_cost (i-1, j-1);//same lines = MATCH
			cost[i][j] = cost[i-1][j-1] + substitution_cost;//remains unchanged if possible
			link[i][j] = diagonal;

			int delcost = cost[i-1][j] + 1;//delete from file 1
			int inscost = cost[i][j-1] + 1;//insert into file 1 from file2

			if (delcost < cost[i][j]) {
				cost[i][j] = delcost;
				link[i][j] = vertical;}
			if (inscost < cost[i][j]) {
				cost[i][j] = inscost;
				link[i][j] = horizontal;}
		}
	}
}

void LCS::text1_push_back (string &filename){
	ifstream fin;
	fin.open(filename.c_str());
	string line;
	while (getline (fin, line) ) {
		text_1.push_back (line);
	}
	fin.close();
}

void LCS::text2_push_back (string &filename){
	ifstream fin;
	fin.open(filename.c_str());
	string line;
	while (getline (fin, line) ) {
		text_2.push_back (line);
	}
	fin.close();
}

//******************
int main(int argc, char *argv[])
{
	string file1, file2;
	bool verbose=false;
	if (argc == 3){
		file1 = string (argv[1]); file2 = string (argv[2]);}
	else if (argc == 4 && string (argv[1]) == "-verbose") {
		file1 = string (argv[2]); file2 = string (argv[3]);
		verbose = true;
	}
	// check two input files are specified on command line
	else {cerr << "usage: ./Diff8 [-verbose] file1 file2\n";
		return 0;}

	LCS lcs;  // instantiate your "LCS based diff" object
	lcs.text1_push_back (file1);//read from file into vector of strings
	lcs.text2_push_back (file2);

	lcs.compute_alignment();
	
	if (verbose) {
		lcs.print_details();}

	lcs.report_difference(verbose);//tested with 5 different files including Diff.cpp and compared with solution ex. by unix diff command

}
