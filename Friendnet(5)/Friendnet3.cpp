#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <set>
#include <algorithm>//for std::sort, unique
#include <fstream>

using namespace std;


void set_oldfriends(vector <string> &name, vector <set <int> >  &friends, int M0, int M1) {
	int N = (int)name.size();
	friends.resize (N);//initialize 2D-vector-array friends
	//sparse matrix grows the columns
	int M; //random number of friends
	int j;//random friend index
	for (int i=0; i<N; i++) {//for each name
		set <int > doknow ;  //declare std::set called doknow
		//doknow will have indices of the friends (at least 1 friend)
		// doknow is a sorted set
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
			friends.at(i).insert (J); //friends.at(i).at(J) = 1;//update [i][j] to 1
			friends.at(J).insert (i);//friends.at(J).at(i) = 1;//update [j][i] to 1
			
			++it;
		}	//update the corresponding pairs of friends entries
	}
}//done

void set_newfriends(vector <set <int> >  &friends, vector <set <int> > &new_friends) {
	int N = (int)friends.size();
	new_friends.resize (N);//initialize 2D-vector-array new_friends
	
	set<int>::iterator j,k;
	for (int i=0; i<N; i++) {
		for (j = friends[i].begin(); j != friends[i].end(); ++j){//for (each adjacent friend: friends[i][j] == 1) {
			//if i and j are friends
			int J = *j; //index of a friend
			
			for (k = friends[J].begin(); k != friends[J].end(); ++k) {
				int K = *k;//index of a possible friend (friend of a friend)
				
				bool friends_now = false;//will determine whether one is already friends with the other
				set<int>:: iterator it;
				for (it = friends.at(i).begin(); it != friends.at(i).end(); ++it ) {
					if ( (*it) == K ) friends_now = true;
				}
				if (i != K && !(friends_now) ){
					new_friends.at(i).insert (K);
					new_friends.at(K).insert (i);
				}
			}
		}
	}
}//done


void writetofile(const char *fname, vector<string> &name, vector <set <int> >  &friends) {
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
		
		set<int>::iterator j;
		int count_8 = 0;//will signal when 8 names per line limit reached
		
		for (j = friends[i].begin(); j != friends[i].end(); ++j) {
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

	vector <set<int> > friends;//	declare 2D-vector-array called friends
	vector <set<int> > new_friends; //declare 2D-vector-array called new_friends

	set_oldfriends(name, friends, M0, M1);
	writetofile("doknow3.txt", name, friends);

	set_newfriends(friends, new_friends);
	writetofile("mightknow3.txt", name, new_friends);
}
