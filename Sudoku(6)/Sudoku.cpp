#include <sys/time.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class omega {
	public:
		omega();
		float get_sec();

	private:
		long Tinit;
		long Tmark;
};

omega::omega() {
  struct timeval T;
  gettimeofday(&T, NULL);

  Tinit = 1000000*T.tv_sec + T.tv_usec;
  }

float omega::get_sec() {
	struct timeval T;
	gettimeofday(&T, NULL);

	Tmark = 1000000*T.tv_sec + T.tv_usec;

	return (float)(Tmark-Tinit)/1000000.0;
}

string elapsed(float duration, int i=0) {//0 means seconds
	if (duration <.1) { //if base case (less than .1 of a sec)
		return elapsed (duration*1000, i+1);
	}
  else{
	string final=""; string unit;
	if (i==0) unit = "sec";
	if (i==1) unit = "msec";
	if (i==2) unit = "usec";

	ostringstream os;
	os<<duration<<" "<<unit;

	return os.str(); //return //return solution
}
 //use recursion
}


class sudoku {
	public:
		sudoku();

		void solve();

		void read(const char *);
		void write(const char *);
		void write(const char *, const char *);

	private:
		bool solve(vector <int> cells, int c);

		// various support functions 
		bool error_check_value(bool is_solved);
		bool error_check_uniqueness();

		//helper functions
		bool check_row (int row, int value);
		bool check_column (int column, int value);
		bool check_square (int, int, int);
		vector <int> valid_values (int i, int j);

		void display();

		int game[9][9];
};

sudoku::sudoku() { 
	for (int i=0; i<9; i++) {
		for (int j=0; j<9; j++)
			game[i][j] = 0;
	}
}

void sudoku::solve() {
	cout << "SOLVE\n";


	bool error = false;
	vector <int> cells;//vector of cells that need to be filled in
	int converted_cell=-1;
	for (int i=0; i<9; ++i){
		for (int j=0; j<9; ++j){
			if (game[i][j] == 0) {
				converted_cell = (i*9)+j;
				cells.push_back(converted_cell);
			}
		}
	}

//UNLEASH RECURSION
	solve(cells,0);

	if (solve(cells, 0)) {
		display();
	}
//UNCOMMENT ERROR CHECKING WHEN DONE WITH SOLVE
	if (error_check_value(false) ) error = true; // error check data values
	if (error_check_uniqueness() ) error = true;// error check uniqueness
	if (error) exit(0);// exit if errors detected

	// call recursive solve function/
	//if (solution-found)
	//display();
	// error check data values
	// error check uniqueness
	// exit if errors detected
}

bool sudoku::error_check_value (bool zero_valid){
	bool error = false;
	for (int i=0;i<9; ++i){
		for (int j=0; j<9; j++){
			if (zero_valid){
				if (game[i][j] <0 || game[i][j] >9) {
					error = true;
					cerr<<"line "<<i<<": "<<game[i][j]<<": out of bounds grid index"<<'\n';}
			}
			else if (!zero_valid) {
				if (game[i][j]<1 || game[i][j] >9){
					error = true;
					//cout<<"Zero not valid!"<<'\n';

					cerr<<"line "<<i<<": "<<game[i][j]<<" out of bounds grid index"<<"column "<<j<<'\n';}
			}
		}
	}
	return error;
}

bool sudoku::error_check_uniqueness() {//will use helper functions defined below
	bool error = false;
	int value = 0;
	int num_non_unique =0 ;
	for (int i=0; i< 9; ++i){
		for (int j=0; j< 9 ; ++j){
			if (game[i][j] >0){
				value = game[i][j];
				
				//if (!(check_row(i, value) ) ) {error = true; cerr <<"cell "<<i<<" "<<j<<": non-unique value "<<value<<'\n';}
				//if (!check_column(j, value))  {error = true; cerr <<"cell "<<i<<" "<<j<<": non-unique value "<<value<<'\n';}
				//if (!check_square (i,j,value) ) {error = true;}

				if (! (check_row(i, value) && check_column (j, value) && check_square (i, j, value) ) ) {
					error = true;

					++num_non_unique;
					if (num_non_unique ==1) display();
				
					cerr <<"cell "<<i<<" "<<j<<": non-unique value "<<value<<'\n';}
			}
		}
	}
	return error;
}

bool sudoku::check_row(int r, int value){
	bool unique_row = true;
	int n=0;//number of occurrences
	for (int j=0; j<9; ++j){
		if (game[r][j] == value){
			++n;}
	}

	if (n>1) {unique_row = false;}

	return unique_row ;
}

bool sudoku::check_column (int c, int v){
	bool unique_column = true;
	int n=0;
	for (int i=0;i<9; ++i){
		if (game[i][c] == v) ++n;
	}

	if (n>1) {unique_column = false;}
	return unique_column;
}

bool sudoku::check_square(int i, int j, int v){
	int start_i = (i/3)	*3;//convert into index
	int start_j = (j/3) *3;//same here
	int n=0;
	for (int i= start_i; i<(start_i+3);++i){
		for (j = start_j; j<(start_j+3); ++j) {
			if (game[i][j] == v){
				n++;}
		}
	}
	if (n>1) return false;//repeating value inside the square space

	return true;
}

vector <int> sudoku::valid_values (int i, int j) {
	int copy_game[9][9];
	for (int m=0; m<9; m++){
		for (int n=0; n<9; ++n){
			copy_game[m][n] = game[i][j];
		}
	}

	vector<int> valid_vals;
	for (int value=1; value<10; ++value){
		game[i][j] = value;	//going to plug in random values until it works with the game

		if ( (check_row(i, value) && check_column(j, value) && check_square(i,j,value) ) ){
			valid_vals.push_back(value);}
	}

	game[i][j] = copy_game[i][j];

	return valid_vals;
}


void sudoku::read(const char *fname) {
	cout << "READ\n";
	bool error= false;
	int line = 1;

	ifstream fin(fname);

	int i, j, v;

	while (fin >> i >> j >> v) {
		// error check grid indices
		if (i<0 || i>8){//faulty index for row
			error = true;
			cerr << "line "<<line<<": "<<i<<" "<<j<<" "<<v<<" out-of-bounds grid index "<<'\n';
		}
		if (j<0 || j>8){
			error = true;
			cerr << "line "<<line<<": "<<i<<" "<<j<<" "<<v<<" out-of-bounds grid index "<<'\n';
		} //faulty index for column

		game[i][j] = v;
		++line;

	}
	fin.close();

	// exit if bad grid indices

	if (error_check_value(true) ) error = true; // error check data values
	if (error_check_uniqueness() )error = true;  // error check uniqueness
	if (error) exit(0);// exit if errors detected

	display();//only if no errors found
}





void sudoku::write(const char *fname) {
	ofstream fout(fname);

	for (int i=0; i<9; i++) {
		for (int j=0; j<9; j++) {
			if (0 < game[i][j]) {
				fout << i << " "
					<< j << " "
					<< game[i][j] << "\n";
			}
		}
	}

	fout.close();
}

void sudoku::write(const char *fname, const char *addon) {
	int N1 = strlen(fname);
	int N2 = strlen(addon);

	char *n_fname = new char[N1+N2+2];

	// strip .txt suffix, then concatenate _addon.txt
	strncpy(n_fname, fname, N1-4);
	strcpy(n_fname+N1-4, "_");
	strcpy(n_fname+N1-3, addon);
	strcpy(n_fname+N1-3+N2, ".txt");

	write(n_fname);

	delete [] n_fname;
}

void sudoku::display() {
	cout << "| --------------------------- |\n";
	for (int i=0; i<9; i++) {
		for (int j=0; j<9; j++) {
			if (j == 0) 
				cout << "| " << game[i][j];
			else if (j%3 == 0) 
				cout << " | " << game[i][j];
			else
				cout << "  " << game[i][j];
		}
		cout << " |\n";
		if (i%3 == 2)
			cout << "| --------------------------- |\n";
	}
}

bool sudoku::solve(vector<int>cells, int c) {
	if (c == cells.size() ) {// if solution found, 
		return true;}//   return solution-found
	

	int i= cells[c] / 9;
	int j= cells[c] % 9;
	
	vector <int> values = valid_values (i, j);
	//sort (values.begin(), values.end());
	if (values.size() == 0) return false; //if no valid values left, return road-to-nowhere

	for (unsigned int m = 0; m<values.size(); m++){
		game[i][j] = values[m];

		
		if (solve (cells, c+1) == true) return true;
	}
	game[i][j] = 0;
	return false;


	// set cell index (i,j)
	// determine valid values 
	// if no valid values left,
	//   return road-to-nowhere
	//
	// iterate thru valid values 
	//   game[i][j] = next value
	//   if solve(arguments) == solution-found
	//     return solution-found
	//
	// reset: game[i][j] = 0
	// return road-to-nowhere
}

int main(int argc, char *argv[]) {
	srand(time(NULL));

	if ((argc != 3) ||
			(strcmp(argv[1], "-s") != 0) ||
			strstr(argv[argc-1], ".txt") == NULL) {
		cerr << "usage: Sudoku -s game.txt\n";
		exit(0);
	}

	omega timer;
	sudoku sudoku_game;

	float T0 = 0.0;
	float T1 = 0.0;

	if (strcmp(argv[1], "-s") == 0) {
		sudoku_game.read(argv[2]);

		T0 = timer.get_sec();
		sudoku_game.solve();
		T1 = timer.get_sec();

		sudoku_game.write(argv[2], "solved");
	}

	string elapsed_time = elapsed(T1-T0);
	cout<<"Time: "<<elapsed_time<<" "; // print string elapsed_time using two decimal places
	cout<<"("<<fixed<<setprecision(6)<<T1-T0<<" sec)"; // print T1-T0 using six decimal places for comparison
}
