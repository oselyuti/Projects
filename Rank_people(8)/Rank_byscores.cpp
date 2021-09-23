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
    float return_mean() const;
	bool mean_less(scores_t);

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
	for (unsigned int i=0; i< scores.size(); i++){
		if (i == 0) {
			cout<< setw(2) << scores.at(i) ;}

		else {cout<<" " << setw(2) << scores.at(i) ;}
	}
	//printing the mean
	cout<< " : " << fixed <<setprecision(1) << mean <<'\n';
}

float scores_t::return_mean () const {return mean;}
bool scores_t::mean_less (scores_t new_s) {
	return (mean < new_s.mean);
}




class namescores_t {
  public:
    namescores_t() ;
    namescores_t(const name_t &, const scores_t &);

    bool operator<(const namescores_t &) ;
    
    void print_name(int n=0);
    void print_scores();

  private:
    name_t n;
    scores_t s;
};

namescores_t::namescores_t(const name_t & nn, const scores_t & ns){
  n = nn;
  s = ns;}

bool namescores_t::operator < (const namescores_t & new_ns) {
	if (s.return_mean() < new_ns.s.return_mean() ) return true;//if curr
	//if (s.mean_less (new_ns.s ) ) {return true;}
	//else if (s.mean)
	else if (s.return_mean() == new_ns.s.return_mean() ){
      if (new_ns.n < n) return true;
      return false;
  }
	return false;
}


void namescores_t::print_name (int width){
  n.print(width); }

void namescores_t::print_scores (){
  s.print();}



int main(int argc, char * argv[]) {
  //commandline parsing
  string filename; int width, num_print=0;
  if(argc == 4 ) {
	  width = atoi (argv[1]);
      num_print = atoi (argv[2]); //number of elements to print
	  filename = string (argv[3]);}
  else {cout<<"usage: name_of_executable Width of name field filename"<<'\n';
	  return 0;}

  ifstream fin;
  fin.open (argv[3]);// open filename.txt
  vector<namescores_t> NS;

  string first, last;
  int score=0;
  string line;
  istringstream sin;
  while (getline(fin, line )){
	  sin.str(line);
	  sin>>first>>last;
	  name_t name(first, last);
	
	  scores_t scores;
	  while (sin>>score){
		  scores.insert(score);}
	 
	  scores.insert_done();
     
	  sin.clear();//finished processing a line
   
   namescores_t namescores (name, scores);
   NS.push_back (namescores); // create and insert namescores_t object into NS list
  }

  fin.close();
  
  //turn NS list into binary max-heap
  make_heap (NS.begin(), NS.end() );

  for (int i=0; i < num_print && !NS.empty(); i++){
    NS[0].print_name(width);
    NS[0].print_scores();
    pop_heap (NS.begin(), NS.end() );
    NS.pop_back();
  }
}
