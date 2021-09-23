use class12_hashtable2_handout.pdf

#include <ctype.h> //.h is global scope for???

hash fufnction with the string from the first page
remove template references : change Tkey to strings, no template needed

code inside hash_table class 
	struct key_lime {
		bool inuse();
		bool operator==(const string & ) const;

		string key;
		vector <int> lines;
	};

const vector <int> &find (const string &)
	private:
		

inside the constructor :
hash_table::hash_table(){
	int N = 23;

	...


char remove_punctuation (char c){
	if ispunct(c){
		return ' ';
	}
	return c;
}

in main() 
	...
	string line;
	std::transform (line.begin(), line.end(), line.begin(), remove_punctuation);
