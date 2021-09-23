#include <bits/stdc++.h>//int_max
#include <limits>
#include <cmath>
#include <map>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>//for std::sort, unique
#include <vector>
#include <string>
#include <iostream>
#include <set>
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

void read_cityinfo (const char * file, vector <city> & cities, map <string, int> & key_map ) {
	ifstream fin; fin.open (file);//city_list.txt - given
	city add_city;
	string line;
	int i=0;
	istringstream sin;
	while (getline (fin, line)) {
		if (line[0] != '#' && line !="" ){//check for comments and mt strings
			sin.str (line);
			sin>>add_city;//overloaded operator >>

			key_map.insert (pair<string,int> (add_city.get_name(), i));
			++i;

			cities.push_back(add_city);
			sin.clear();}
		else {continue;}//if # or empty line encountered
	}
	fin.close();
}
void write_cityinfo(const char * file, vector <city> &v) {
	ofstream fout; fout.open (file);//city_info.txt
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

		//v2
		//void create_citygraph();
};


int costtable::compute_dist (float long1,float long2, float lat1, float lat2){
	double difflong = (long1-long2) * M_PI /180.0;//Havershine formula to compute dist
	double difflat  = (lat1 -lat2 ) * M_PI / 180.0;
	//convert to radians
	lat1 = (lat1) * M_PI / 180.0;
	lat2 = (lat2) * M_PI / 180.0;

	double a =pow (sin (difflat/2), 2) +  pow (sin (difflong /2), 2)* cos (lat1) * cos(lat2);
	float radius = 3982.0;//miles
	double C = 2 * asin (sqrt(a) );
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





void create_citygraph (vector <city> &c, vector <vector<int > > & adjacent_cities, costtable & table ){//mode 0 or 1
	//extract index list for each type of city (regional and gateway)
	vector <int> regional_indeces;
	vector <int> gateway_indeces;
	adjacent_cities.resize (c.size());

	for (unsigned int i=0; i< c.size(); ++i){
		if (c.at(i).get_type() == "REGIONAL") {
			regional_indeces.push_back(i);}
		else if (c.at(i).get_type() == "GATEWAY") {
			gateway_indeces.push_back (i);}
	}

	city cur_city, next_city;
	int index_for_cur_city;
	//first figure out which regional cities are adjacent
	for (int i=0; i < (int) regional_indeces.size(); ++i){
		cur_city = c.at(regional_indeces.at(i) );
		index_for_cur_city = regional_indeces.at(i);
		for (int j=0; j < int (regional_indeces.size()); ++j ){
			next_city = c.at(regional_indeces.at(j));
			if (cur_city.get_zone() == next_city.get_zone() && cur_city.get_name () != next_city.get_name()){//then they are adjacent
				adjacent_cities.at (index_for_cur_city).push_back (regional_indeces.at(j) );
				//then city at j is also adjacent to city at i
				adjacent_cities.at (regional_indeces.at(j)).push_back (index_for_cur_city );
			}
		}

		//find min dist SAME ZONES every regional city is adjacent to the NEAREST gateway
		float nearest_GW_dist = 5200.0;
		int index_GW_city = -1;
		float dist = 30000.0;

		for (int j=0; j < (int)gateway_indeces.size(); ++j){
			next_city = c.at (gateway_indeces.at(j) );
			if (cur_city.get_zone () == next_city.get_zone () ){//calculate distance and compare to the minimum one
				dist = table (0, index_for_cur_city, gateway_indeces.at(j) );
				if (dist < nearest_GW_dist) { 
					nearest_GW_dist = dist;//will eventually give the closest gw city
					index_GW_city = gateway_indeces.at(j);
				}	
			}	
		}
		//update the closest GW city 
		if (index_GW_city != -1) {//found a close GW city
			adjacent_cities.at (index_for_cur_city).push_back (index_GW_city);
			adjacent_cities.at (index_GW_city).push_back (index_for_cur_city);
			}	
	}


	//now figure out the GATEWAY cities adjacency
	for (int i=0; i < (int) gateway_indeces.size(); ++i){
		//adj to all other gateways in the same zone
		cur_city = c.at(gateway_indeces.at(i) );
		index_for_cur_city = gateway_indeces.at(i);
		for (int j=0; j < (int) gateway_indeces.size(); ++j){
			next_city = c.at (gateway_indeces.at(j));
			//GATEWAY same zone, diff name
			if (cur_city.get_zone () == next_city.get_zone() && cur_city.get_name() != next_city.get_name() ) {
				adjacent_cities.at(index_for_cur_city). push_back (gateway_indeces.at(j) );
				adjacent_cities.at(gateway_indeces.at(j)).push_back (index_for_cur_city);
			}
		}

		for (int z =0; z < 7; ++z){
			float min_distance = 5200.0;
			float dist_ij =30000.0; //distance between two points
			int index_min_dist = 0;
			if (z != cur_city.get_zone() ){//find the distance to that zone if one can get there (less than 5200 miles)
				for (int j=0; j < (int) gateway_indeces.size() ; ++j) {//wawlk through every gateway city
					next_city = c.at(gateway_indeces.at(j));
					if (next_city.get_zone() == z) {//if the zone is the one I am looking for minimal distance to
						dist_ij = table (0, index_for_cur_city, gateway_indeces.at(j) );//calculate the distance to it
						if (dist_ij < min_distance) {
							min_distance = dist_ij; //update the smaller distance if found
							index_min_dist = gateway_indeces.at(j);
						}
					}
				}
				//if min_distance to the zone is found, add it when done with the zone search
				if (min_distance < 5200.0) {//an ACCEPTABLE GATEWAY CITY outside the current zone is found
					adjacent_cities.at (index_for_cur_city).push_back (index_min_dist);
					adjacent_cities.at (index_min_dist).push_back (index_for_cur_city);
				}
			}
		}
	} 
	//get rid of duplicates and sort the cities by index
	vector <int>::iterator last_unique;
	for (int i=0; i <(int) adjacent_cities.size(); ++i){
		sort (adjacent_cities.at(i).begin(), adjacent_cities.at(i).end() );
		last_unique = unique (adjacent_cities.at(i).begin(), adjacent_cities.at(i).end() );
		adjacent_cities.at(i).resize (last_unique - adjacent_cities.at(i).begin() );
	}

}//end function create_g()


void write_citygraph (vector <city> &c, vector <vector<int> > & adjacent_cities, costtable &table){
	ofstream fout;
	fout.open ("MY_city_graph.txt");

	int index =0;
	fout<<"CITY GRAPH:"<<'\n';
	for (int i=0; i <(int) c.size() ; ++i) { 
		fout<<'\n'
			<<setw (3)<<right << i <<" " << c.at(i).get_name()<<'\n';

		for (int k=0; k < (int)adjacent_cities.at(i).size(); ++k ) {
			index = adjacent_cities.at(i).at(k);
			fout <<setw (6) <<right <<index << " "<< left<<setw (20) <<c.at (index).get_name()
				<<fixed<< setprecision (1) <<right<<setw(6)<< table (0,index,i) <<" miles" 
				<<setw(6)<<right << table (1,index,i) <<" hours"
				<<'\n';
		}
	}
	fout.close();
}//end write



typedef enum {WHITE, BLACK} vcolor_t;
void dijkstra_route (vector<city> &V, vector <vector <int> > &E, costtable &table, int mode, int source, int sink ){

	vector <vcolor_t> vcolor;
	vector <float> vdist;
	vector <int> vlink;

	vcolor.assign (V.size(), WHITE);
	vdist.assign (V.size(), numeric_limits<float>::max());
	vdist[source] = 0;

	vlink.assign (V.size(), -1);
	vlink[source] = source;

	while (1) {
		int next_i = -1;
		float minweight = numeric_limits<float>::max();

		for (int i=0; i <(int) vcolor.size(); ++i) {
			if (vcolor[i] == WHITE && vdist[i] < minweight) {
				next_i = i;
				minweight = vdist[i];
			}
		}
		int i = next_i;
		if (i == -1) return;
		vcolor[i] = BLACK;
		if (i == sink) break;
		for (int k=0; k < (int) E[i].size(); k++) {
			int j = E[i][k];
			float weight_ij = table (mode, i, j);//dist from i to j
			if (vcolor[j] == WHITE) {
				if (vdist[j] >vdist[i] + weight_ij) {
					vdist[j] = vdist[i] + weight_ij;
					vlink[j] = i;

				}
			}
		}
	}

	//SHOW_ROUTE -> WILL print the outcomes of dijkstra
	if (vdist[sink] == INT_MAX) {
		cout << "No path from\n";
		return;}

	stack <int> S;
	int prev_i,i =0;

	for (int i= sink; i!= source; i = vlink[i]){
		S.push(i);}
	S.push(source);

	int line =0;
	while (!S.empty()) {
		prev_i = i;
		i = S.top(); 
		S.pop();

		float tot_time, tot_dist = 0.0;//total travel. will use whenever can't access time/dist the same way as I do the method
										//selected -dist|-time
		if (line ==0) {//print the source city
			cout<< fixed<< setw(7) <<right <<setprecision(1)<< 0.0 << " miles"
				<<setw(6)<<right <<0.0<<" hours : "
				<<i <<" " <<V[i].get_name()<<'\n';
			++line;
			continue;
		}

		if (mode == 0) {//dist
			cout<< fixed<< setw(7) <<right <<setprecision(1)<< vdist[i] << " miles"; 
			tot_time += table (1,i,prev_i);

			cout<<setw(6)<<right  <<tot_time <<" hours :" ;
		}

		else if (mode ==1) {//time
			tot_dist += table (0,i,prev_i);//tracks the total distance from source to sink
			cout<< fixed<< setw(7) <<right <<setprecision(1) << tot_dist <<" miles" 
				<<setw(6)<<right  <<vdist[i]<<" hours :" ;
		}

		cout<<setw(3)<<right<< i <<" "
			<<setw(20)<<left<< V[i].get_name(); 
		cout<<setw (6)<<table (0,i,prev_i )<<" miles"//individual distances
			<<setw (6)<<right<<table (1,i,prev_i )<<" hours";//individual times
		cout<<'\n';
	}
}


int main(int argc, char *argv[]){
	if ( (argc != 2 ) || (argc==2 && string(argv[1]) != "-graphinfo" && string (argv[1]) != "-time" && string (argv[1]) != "-distance") ) {
		//-graphinfo produces 4 other files with distances and travel times between cities, city_graph, and travel times between cities
		cerr <<"usage: ./Citysim -graphinfo|-distance|-time\n";
		return 0;}

	map <string, int> key_map;//key-to-index map

	vector <city> all_cities;
	read_cityinfo ("city_list.txt",all_cities, key_map);
	costtable table (all_cities) ;//makes a table of all distances between cities as well 
	//as the time it takes to move between i and j (move those dist)

	vector <vector <int> > adj_cities;
	create_citygraph(all_cities, adj_cities, table);

	int mode = -1;
	if (argc ==2 && string (argv[1] ) == "-distance") mode = 0;
	if (argc ==2 && string (argv[1] ) == "-time") mode = 1;
	if (mode == -1) {
		cerr << "incorrect mode\n";
		return 0;
	}

	if (argc == 2 && string (argv[1]) == "-graphinfo") {	
		write_cityinfo ("MY_city_info.txt", all_cities);
		table.write_traveldistance(all_cities);
		table.write_traveltime (all_cities);
		write_citygraph (all_cities, adj_cities, table);
	}

	
	string city1, city2;
	int source, sink;
	cout<<"Enter> ";

	while (cin>>city1>>city2){
		source = key_map[city1];
		sink = key_map[city2];
		dijkstra_route (all_cities, adj_cities, table, mode, source, sink );
		cout<<'\n';

		cout<<"Enter> ";
	}

}//end of main
