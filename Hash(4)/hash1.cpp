
#include <ctype.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include <list>
#include <algorithm>
using namespace std;

typedef unsigned int uint;

//template <typename Tkey>
class hash_table {
		public: 
			hash_table(int size); 
			~hash_table();
			void insert(const string &,int);
			void print_table();
			const vector <int> & find (string key);
			bool insert_done();
			void set_showstats();

		private:

			struct key_line{
				bool inuse();//indicates whether the object holds key or not
				bool operator== (const string &) const;

				string key;//word
				vector <int> lines;//keeps all the line numbers where the key is found
			};

			int hash(const string &);
			int nextprime(int);
			int qprobe(const string &,bool count_collision);	
			void resize();

			int num_inuse;
			int max_inuse;

			int collisions;
			bool showstats;
			void showload();

			vector<key_line> table;
};

void hash_table::set_showstats(){
	showstats = true;
}

bool hash_table::insert_done(){
	if (showstats){
		showload();
}
}

void hash_table::showload(){
	cout<<right<<"** N = "<<setw(6)<<table.size()<<" load = "<<setprecision(2)<<double(num_inuse)/table.size()<<'\n';
}

hash_table::~hash_table(){
	cout<<'\n'<<
		"Run Stats ..."<<'\n';
	cout<<"Number of slots used: "<<num_inuse<<'\n';
	cout<<"Max number of slots: "<<max_inuse<<'\n';
	cout<<"Number of collisions: "<<collisions<<'\n';
}

hash_table::hash_table(int size) {
	int N = nextprime (size);
	showstats = false;
	table.assign(N, key_line());

	num_inuse = 0;
	max_inuse = N/2; // quadratic probe max value
	collisions = 0;
}

const vector <int> & hash_table::find(string key){ 
	//if the key is in the table - returns a vector of lines
	//if not  - blank vector
	return table[qprobe(key,false )].lines;

}

bool hash_table::key_line::inuse() {
	if (key !=string() ){
		return true; //key is not empty
	}
	return false;//key is empty
}

bool hash_table::key_line::operator== (const string & s) const {
	if (key == s) {return true;} //input word matches with key
	else {return false;}		   //input word != key
}

void hash_table::insert(const string &key, int cur_line) { 
	int index = qprobe (key,true);

	if (!table[index].inuse()){
		table[index].key = key;
		table[index].lines.push_back(cur_line);
		num_inuse++;
	}
	else if (table[index].inuse()) {
		if (std::find(table[index].lines.begin(), table[index].lines.end(), cur_line) == table[index].lines.end() ){
		table[index].lines.push_back(cur_line);}
	}
		
	if (num_inuse >= max_inuse){
			resize();
			//showload();
	}
}
int hash_table::hash(const string &key) {
	uint index = 0;
	const char *c = key.c_str();

	while (*c)
		index = ((index << 5) | (index >> 27)) + *c++; // looooool
	
	return index % table.size();
}

int hash_table::nextprime(int N) {
	N = 2*N+1;
	int i=2; 
	while (i*i <= N){
		if (N%i == 0) {N++; i =1;}
		i++;
	}

	
//	return N;

	return max(3, N);
}

int hash_table::qprobe(const string &key, bool count_collision) {
	int index = hash(key);

	int k = 0;
	while (table[index].inuse() && ( (table[index].key != key)) ) {// (table[index] != string() && table[index] != key) {
		index += 2 * (++k) - 1;
		index = index % table.size();
		if (count_collision){
			collisions++;
			}
	}
	return index;
}

void hash_table::resize() {
	vector<key_line> tmp_table;
	showload();

	for (int i=0; i < (int)table.size(); i++) {
		if (table[i].inuse()  )//if word slot not empty ->the word at i exists
			tmp_table.push_back(table[i]);//copy both key and lines
			
	}

	int N = nextprime(table.size());
	table.assign(N, key_line()) ;

	num_inuse = 0;
	max_inuse = N/2;

	for (int i=0; i < (int)tmp_table.size(); i++) {
		string &key = tmp_table[i].key;
		vector <int> &lines = tmp_table[i].lines; 
		table[ qprobe(key,false) ].key = key;
		table [qprobe(key,false)].lines= lines;
		num_inuse++;
	}
	

}

void hash_table::print_table() {
  for (int i=0; i< (int)table.size(); i++) {
    if ((table[i].key != string() )){
      cout<<setw(3)<<i<< " "<<table[i].key<<' ';
		cout<<"vector elements: "<<table[i].lines.size()<<" ";
	  
		//for (int j=0; i< table[i].lines.size(); i++){
	//	  cout<<table[i].lines[j]<<" ";
	 // }
	  cout<<'\n';
	}
  }
}

char replace_punctuation (char c){
		if (ispunct(c) ) {
			return ' ';
		}
		return c;
	}

int main(int argc, char *argv[]) {
	if (argc < 3){
		cout<<"./shashtable [-N number] [-showstats] -f textfile"<<'\n';
		return 0;}

	string filename=""; int N=23;
	istringstream sin;
	bool show = false;
	for (int i=0; i<argc; i++){
		if (string (argv[i]) == "-f"){
			filename = ( argv[i+1]);}
		else if (string(argv[i]) == "-N"){
			N = atoi (argv[i+1]);
			}
		else if (string(argv[i]) == "-showstats"){
			show = true;}
	}	
	if (filename == ""){	
		cout<<"./shashtable [-N number] [-showstats] -f textfile"<<'\n';
		return 0;}

	hash_table H(N);
	if (show) H.set_showstats();

	ifstream fin;
	fin.open (filename.c_str());

	vector <string> every_line; string key, line;
	int line_number=1;

	while(getline(fin, line)){ //cin>>key){

		transform (line.begin(), line.end(), line.begin(), replace_punctuation );



		every_line.push_back(line);
		sin.str(line);
		while (sin>>key){
			H.insert(key, line_number);
		}
		sin.clear();
		++line_number;
	}

	fin.close();	
	H.insert_done();

	line = "";
	vector <int> lines;


	do{
		cout<<"find> ";
		cin>>line;
		lines = H.find(line);
		for (int i =0; i< lines.size(); i++){
		cout<<lines[i]<<": ";
		cout<<every_line[lines[i]-1]<<'\n';
		
		}
	} while (1) ;
		




/*
	cout<<"find> ";
	while (cin>>line){
		lines = H.find(line);
		for (int i =0; i< lines.size(); i++){
			cout<<lines[i]<<": ";
			cout<<every_line[lines[i]-1]<<'\n';
		}
		cout<<'\n'<<"find> ";
	}
	
*/
	
}





