#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <set>
#include <fstream>

using namespace std;

void set_oldfriends(vector <string> &name, vector <vector <int> >  &friends, int M0, int M1) {
	int N = (int)name.size();
	friends.assign ( N, vector <int> (N ,0));//initialize 2D-vector-array friends (hint: use vector::assign())
	//friends is my dense matrix
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
		while (it != doknow.end() ) {
			int J = *it;
			friends.at(i).at(J) = 1;//update [i][j] to 1
			friends.at(J).at(i) = 1;//update [j][i] to 1
			//now the friends will have 1 at their intersection location (row,col)
			++it;}	//update the corresponding pairs of friends entries
	}
}

void set_newfriends(vector <vector <int> >  &friends, vector <vector <int> > &new_friends) {
	int N = (int)friends.size();
	new_friends.assign (N, vector <int> (N,0) );//initialize 2D-vector-array new_friends (hint: use vector::assign())
	for (int i=0; i<N; i++) {
		for (int j=0; j < N; ++j){//for (each adjacent friend: friends[i][j] == 1) {
			if (friends[i][j]) {//if i and j are friends
				for (int k = 0; k < N; ++k) {//for (each adjacent friend-of-friend: friends[j][k] == 1) {
					if (friends[j][k] == 1) {//friend of a j is at k. i might know k.
						if ( i != k && ! (friends[i][k] )){//if (k-is-not-i && k-is-not-friend-of-i)
							new_friends[i][k] = 1;
							new_friends[k][i] = 1;
							//update the corresponding pairs of new_friends entries
						}
					}
				}
			}
		}
	}
}

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

		int count_8 = 0;//will signal when 8 names per line limit reached
		for (int j=0; j<N; ++j){	//for (each adjacent friend: friends[i][j] == 1) {
			if (friends[i][j] == 1 ) {
				if (count_8 == 8) {//8 is allowed, 9 is too much
					fout<<'\n'<<setw (max_namelength) << left<<  name[i]<<" :" ; 
					count_8 =0;}

				count_8++;
				fout << " "<< setw (max_namelength) <<left << name[j];
			}
		}

		fout<<'\n';	
	}
		fout.close();//close file
}


int main(int argc, char *argv[]) {
	if (argc != 1 && argc != 3) {cerr<<"Usage: ./Friendnet1 [-seed N] \n"; return 0;}
	//print usage message and exit if invalid
	int seed =0;
	if (argc == 3 && string (argv[1]) == "-seed"){//seed given
		seed = atoi (argv[2]);//set the seed
	}	

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
	writetofile("doknow1.txt", name, friends);

	set_newfriends(friends, new_friends);
	writetofile("mightknow1.txt", name, new_friends);
}
