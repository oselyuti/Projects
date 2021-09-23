#include <iostream>
#include <vector>
#include <iomanip>
//#include <something> //for transform
#include <cstdlib>
#include <set>
#include <algorithm>//for std::sort, unique
//for unique
#include <fstream>

using namespace std;


void set_oldfriends(vector <string> &name, vector <vector <int> >  &friends, int M0, int M1) {
	int N = (int)name.size();
	friends.resize (N);//initialize 2D-vector-array friends (hint: use vector::assign())
	//sparse matrix grows the columns
	int M; //random number of friends
	int j;//random friend index
	for (int i=0; i<N; i++) {//for each name
		set <int > doknow ;  //declare std::set called doknow
		//doknow will have indices of the friends (at least 1 friend)
		M = (M0 + rand () %M1 );//initialize M (random number of friends: function of M0, M1)

		while ( (int)doknow.size() < M) {
			do {	
				j = rand() %N;//compute j (random friend index: hint j != i)
			} while (j==i);

			doknow.insert(j);
		}

		set <int> :: iterator it = doknow.begin();//use iterator to sweep thru doknow set
		//doknow might have between 1 and 3 random indeces
		int J;
		while (it != doknow.end() ) {
			J = *it;

			friends.at(i).push_back (J);
			friends.at(J).push_back (i);
			//eliminate duplicates in [J]
			sort (friends.at(J).begin (), friends.at(J).end()) ;
			vector <int>::iterator last_unique;
			last_unique = unique (friends.at(J).begin(), friends.at(J). end() );
			friends.at(J).resize (last_unique - friends.at(J).begin() );
			
			++it;
		}	//update the corresponding pairs of friends entries


		sort (friends.at(i).begin (), friends.at(i).end()) ;
		vector <int>:: iterator last_unique;
		last_unique = unique (friends.at(i).begin(), friends.at(i). end() );
		friends.at(i).resize (last_unique - friends.at(i).begin() );
	}
}

void set_newfriends(vector <vector <int> >  &friends, vector <vector <int> > &new_friends) {
	int N = (int)friends.size();
	new_friends.resize (N);//initialize 2D-vector-array new_friends
	vector<int>::iterator j,k;
	for (int i=0; i<N; i++) {
		for (j = friends.at(i).begin(); j != friends.at(i).end(); ++j){//for (each adjacent friend: friends[i][j] == 1) {
			//if i and j are friends
			int J = *j; //index of a friend
			
			for (k = friends.at(J).begin(); k != friends.at(J).end(); ++k) {
				int K = *k;//index of a possible friend (friend of a friend)
				//if (k-is-not-i && k-is-not-friend-of-i)
				bool friends_now = false;	
				for (unsigned int l=0; l < friends.at(i).size(); ++l ) {
					if (friends.at(i).at(l) == K ) {
						friends_now = true;
					}
				}
				if (i != K && !(friends_now) ){
					new_friends.at(i).push_back (K);
					new_friends.at(K).push_back (i);
				}
			}
		}
	}
	vector <int>:: iterator last_unique;//make sure all the new_friends are sorted
										//eliminate duplicates
	for (int i=0; i < N; ++i){
		sort (new_friends.at(i).begin (), new_friends.at(i).end()) ;
		last_unique = unique (new_friends.at(i).begin(), new_friends.at(i).end() );
		new_friends.at(i).resize (last_unique - new_friends.at(i).begin() );
	}
	
}//done


void writetofile(const char *fname, vector<string> &name, vector <vector <int> >  &friends) {
	ofstream fout;
	fout.open (fname);//open file
	int N = (int)name.size();

	string n; int max_namelength=0; //determine max name length
	for (int i=0; i< N; ++i) {
		if (int (name.at(i).length()) > max_namelength ){
			max_namelength = name.at(i).length ();}
	}

	for (int i=0; i<N; i++) {
		fout <<setw(max_namelength)<<left << name[i]<< " :" ;

		vector <int>::iterator j;

		int count_8 = 0;//will signal when 8 names per line limit reached
		for (j = friends.at(i).begin(); j != friends.at(i).end(); ++j) {
			if (count_8 == 8) {//8 is allowed, 9 is too much
				fout<<'\n'<<setw (max_namelength) << left<<  name[i]<<" :" ; 
				count_8 =0;}

			count_8++;
			fout << " "<< setw (max_namelength) <<left << name[*j];//* deref
		}
		fout<<'\n';	
	}
		fout.close();//close file
}//done


int main(int argc, char *argv[]) {
	if (argc != 1 && (argc != 3 || string(argv[1]) != "-seed" )) {cerr<<"usage: ./Friendnet2 [-seed N] \n"; return 0;}//usage message and exit if invalid
	int seed =0;//for random gen

	if (argc == 3 && string (argv[1]) == "-seed"){//seed given
		seed = atoi (argv[2]);}//set the seed

	srand (seed);//seed with a input seed
	if (argc ==1 ) srand (time(NULL) );	//	seed random number generator

	vector<string> name;
	string inp;
	while (cin>> inp){	//read strings from stdin into name vector
		name.push_back(inp);}

	int M0 = 1; // min number of friends
	int M1 = 3; // max number of friends

	vector <vector<int> > friends;//	declare 2D-vector-array called friends
	vector <vector<int> > new_friends; //declare 2D-vector-array called new_friends

	set_oldfriends(name, friends, M0, M1);
	writetofile("doknow2.txt", name, friends);

	set_newfriends(friends, new_friends);
	writetofile("mightknow2.txt", name, new_friends);
}
