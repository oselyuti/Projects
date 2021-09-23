#include <cmath>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class city{
	string name;
	string type;
	int zone ;
	float latitude, longtitude;
	long population;

	public:
	friend istream & operator >>(istream &, city &);
	friend ostream & operator <<(ostream &, city &);

	string get_name () const {return name;}
	string get_type () const{return type;}
	int get_zone ()const {return zone;}
	float get_latitude ()const {return latitude;}
	float get_longtitude () const{return longtitude;}
	int get_population ()const  {return population;}
};

istream & operator>> (istream & in, city &c) {
	//could check for # here?
	in>> c.zone >> c.name>>c.type>> c.latitude >>c.longtitude >>c.population;
	return in;
}
ostream &  operator <<(ostream & out, city &c) {
	out<<left<< setw (18) <<c.name <<
		setw(12) <<c.type   <<
		setw(2) << c.zone	<<
	    right<<setw(10)<<c.population <<
		setw(8)<<fixed<<setprecision (2) << c.latitude <<
		setw(8)<<setprecision (2) <<c.longtitude;
	return out;
}

void read_cityinfo (const char * file, vector <city> & cities ) {
	ifstream fin; fin.open (file);//city_list.txt - given
	city add_city;
	string line;
	istringstream sin;
	while (getline (fin, line)) {
		if (line[0] != '#' && line !="" ){//check for comments and mt strings
			sin.str (line);
			sin>>add_city;//overloaded operator >>
			cities.push_back(add_city);
			sin.clear();}
     else {continue;}//if # or empty line encountered
	}
	fin.close();
}
void write_cityinfo( vector <city> &v) {
	ofstream fout; fout.open ("MY_city_info.txt");//city_info.txt
  fout<<"CITY INFO (N=" << v.size() <<"):\n" <<'\n';
	for (unsigned int i=0; i< v.size(); ++i){
     fout <<right<<setw (3) <<i <<" ";
		fout << v.at(i)<<'\n';} //overloaded operator does the job
	fout.close();
}



class costtable {
	public:
	costtable ( vector <city> & cities){//size arg
		city_num = cities.size();
		size = city_num * (city_num + 1)/2;	//use 1D array; only store data one way (i->j) j->i NOT needed
		distance_table.resize( size);
		time_table.resize( size);
		
		write_table (cities );
	}
	private:
	vector <float> distance_table;
	vector <float> time_table;
	int size;
	int city_num;
	int compute_dist (float long1,float long2, float lat1, float lat2);
	void write_table(vector<city> & c );//N = size = # of cities
												//creates the tables
	//vector <float> table[2];//dist [0] and time is [1]
	public:
	float operator () (int mode, int i, int j){//mode is 0 for dist, 1 for timei
		if (i < j) swap(i,j);

		if (mode ==0) {//distance
			return distance_table.at (i * (i+1)/2 + j);
		}
		else if (mode ==1) {//time
			return time_table.at (i * (i+1)/2 +j);
		}
		return -1.0;//if neither
	}

	void write_traveldistance (vector <city> &c);//writes data to a file
	void write_traveltime(vector <city> &c);//writes times of commute to a file
};


int costtable::compute_dist (float long1,float long2, float lat1, float lat2){
	float difflong = (long1-long2) * M_PI /180.0;//Havershine formula to compute dist
	float difflat  = (lat1 -lat2 ) * M_PI / 180.0;
	//convert to radians
	lat1 = (lat1) * M_PI / 180.0;
	lat2 = (lat2) * M_PI / 180.0;

	float a =pow (sin (difflat/2), 2) +  pow (sin (difflong /2), 2)* cos (lat1) * cos(lat2);
	float radius = 3982.0;//miles
	float C = 2 * asin (sqrt(a) );
	float dist = C * radius;
	dist = 10.0 * round (dist/10.0);
	return dist;
}


void costtable::write_table(vector<city> & c) {
	city tempI, tempJ;
	int index =0;
	distance_table.assign ( size , 0.0);

	for (int i=0; i< city_num; ++i) {
		tempI = c.at (i);
		for (int j =0; j < i; ++j){
			tempJ = c.at(j);
			index = i * (i+1)/2 + j;
			distance_table.at(index) = compute_dist (tempI.get_longtitude(), tempJ.get_longtitude(), 
					tempI.get_latitude(), tempJ.get_latitude() );
			if (tempI.get_type() == tempJ.get_type() && tempI.get_type() == "REGIONAL") {//both are regional
				time_table.at (index) = distance_table.at(index) / 60.0;//avg ground speed
			}
			else { 
				time_table.at (index) = distance_table.at (index) / 570.0;//avg flight speed
			}
		}
	}
}

void costtable::write_traveldistance (vector <city> &c){
	string where;//to output the origin and destination
	ofstream fout;
	fout.open ("MY_city_distancetable.txt");
	fout<<"DISTANCE TABLE:\n";
	for (int i=0; i < city_num; ++i) {
		for (int j=0; j < i; ++j) {//only outputting for previously seen cities 
			where = c.at(i).get_name() + " to " + c.at(j).get_name() + " "; 
			fout<< setw(3)<<right<<i<<" ";
			fout<<left<<setw(38)<<setfill ('.')<<where<<setfill(' ');
			fout<<right<<setw(7)<<fixed<<setprecision(1) <<
				distance_table[i * (i+1)/2 + j] <<" miles" <<'\n';
		}
		fout<<'\n';//new city
	}
	fout.close();
}

void costtable::write_traveltime (vector <city> &c){
	string where;
	ofstream fout;
	fout.open ("MY_city_timetable.txt");
	fout<<"TIME TABLE:\n";
	for (int i=0; i < city_num; ++i) {
		for (int j=0; j < i; ++j) {
			where = c.at(i).get_name() + " to " + c.at(j).get_name() + " "; 
			fout<< setw(3)<<right<<i<<" ";
			fout<<left<<setw(38)<<setfill ('.')<<where<<setfill(' ');
			fout<<right<<setw(5)<<fixed<<setprecision(1) <<
				time_table[i * (i+1)/2 + j] <<" hours" <<'\n';
		}
		fout<<'\n';//new city
	}
	fout.close();
}


int main(int argc, char *argv[])
{
	if ( (argc != 2 ) || (argc==2 && string(argv[1]) != "-graphinfo") ) {
		//a simple call only produces city_info.txt
		//-graphinfo produces 2 other files with distances and travel times between cities
		cerr <<"usage: " << string (argv[0]) <<" -graphinfo \n";
		return 0;}
	
	vector <city> all_cities;
	read_cityinfo ("city_list.txt",all_cities);

	costtable table (all_cities) ;//makes a table of all distances between cities as well 
								//as the time it takes to move between i and j (move those dist)
	
	if (argc == 2 && string (argv[1]) == "-graphinfo") {	
		write_cityinfo (all_cities);
		table.write_traveldistance(all_cities);
		table.write_traveltime (all_cities);}
}
