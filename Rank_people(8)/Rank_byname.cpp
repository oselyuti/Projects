#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <numeric> //for ::accumulate
#include <algorithm> //for second part with heap
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

class name_t {
  public:
	name_t();
	name_t (const string & first,const string & last);// constructor(s);

    bool operator< (const name_t & new_name) const; //bool operator<(...);
    void print(int W=0) const;

  private:
    string name;
};

name_t::name_t(){name = "";}

name_t::name_t(const string & first, const string & last){
	name = last+", "+first+" ";}	

bool name_t::operator< (const name_t & new_name) const {
	if (name < new_name.name) return true;
	return false;
}

void name_t::print(int W) const {
	cout<<setw (W + 3) <<setfill('.')<<left<< name << " ";
}	

class scores_t {
  public:
	scores_t();

    void insert(int);
    void insert_done();
    void print();

  private:
    vector<int> scores;
    float mean;
};

scores_t::scores_t() {
	mean = 0.0;}

void scores_t::insert(int num){
	scores.push_back(num);}

void scores_t::insert_done(){//prints the mean if scores vector !mt
	if (scores.size() > 0){
		int sum=0;
		for (unsigned int i=0; i<scores.size(); i++){
			sum+= scores.at(i);}
	
		mean = sum / float(scores.size()) ;
	}
}

void scores_t::print(){
	cout<<right<<setfill(' ');
	for (int i=0; i< scores.size(); i++){
		if (i == 0) {
			cout<< setw(2) << scores.at(i) ;}

		else {cout<<" " << setw(2) << scores.at(i) ;}
	}
	//printing the mean
	cout<< " : " << fixed <<setprecision(1) << mean <<'\n';
}



int main(int argc, char * argv[]) {
  //commandline parsing
  string filename; int width;
  if(argc == 3 ) {
	  width = atoi (argv[1]);
	  filename = string (argv[2]);}
  else {cout<<"usage: name_of_executable Width of name field filename"<<'\n';
	  return 0;}

  ifstream fin;
  fin.open (argv[2]);// open filename.txt
  
  map<name_t,scores_t> NS;

  string first, last;
  int score=0;
  string line;
  istringstream sin;
  while (getline(fin, line )){

	  sin.str(line);
	  sin>>first>>last;
	  name_t name(first, last);
//	  name.print();
	  
	
	  scores_t scores;
	  while (sin>>score){
		  scores.insert(score);}
	 
	  scores.insert_done();
//	  scores.print();
	  sin.clear();//finished processing a line
	  
	  //combine the two classes into the NS map
	  NS.insert(make_pair (name, scores));
  }
  /*
  while (fin>>first>>last) {
	  name_t name(first, last);
	  name.print();
	  scores_t scores;

	  while (fin>>score){
		  scores.insert (score);
		cout<<score<<" ";}
  }
*/
/*
  while (more data in file) {
    add first and lastname to name_t object

    while (more scores) {
      insert score into scores_t object
    have insert_done calculate mean score

    create and insert (name_t,scores_t) pair into NS map
  }

  close filename.txt

  while (NP map iterator loop) {
    print name
	print scores
  }

*
*/
	  fin.close();
	  
	  map <name_t, scores_t>::iterator it = NS.begin(); 
	  while (it != NS.end()){
		  it->first.print( width );//names (key in a map) argument is the width specified by the user
		  it->second.print();//scores (value)
		  it++;
	  }
}
