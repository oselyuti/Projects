// list all headers needed 
#include <algorithm> //contains std::stable_sort()
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;


struct fruit {
	//overload operator <
	bool operator< ( const fruit & f_2) const  {
		if (name < f_2.name) {return true;}
		return false;
	}
	bool operator == (const fruit & f_2)const {
		if (name == f_2.name) return true;
		return false;
	}
	//data declarations
	string name;
	float quan;//quantity
	float price;//per pound
	float total_item_price;
};

void print_list ( vector <fruit> & listdata){

	float total_order_price=0.0;
	for (int i=0; i<listdata.size(); i++){// pretty-print array list content to stdout
		total_order_price += listdata[i].total_item_price;

		cout<<fixed<<left<<setprecision(2)<<setfill('.')<< setw(20)<<listdata[i].name<<"  ";
		cout<<setfill(' ')<<setw(5)<<listdata[i].quan
      <<"x "<<setw(4)<< listdata[i].price<<" = "
			<<setw(7)<<right<<listdata[i].total_item_price<< "  : "
			<<setw(8)<<right<<total_order_price<<endl;
	}
}

void compute_total_item_price (fruit & f) { f.total_item_price = f.quan * f.price;}

void sortone( vector <fruit> & listdata){//eliminate repetitions
	for (int i=0; i < listdata.size()-1; i++){
		while (listdata[i] == listdata[i+1] ){
			listdata[i].quan = listdata[i].quan + listdata[i+1].quan ;
			listdata.erase (listdata.begin() + (i+1) ) ;
		}
		compute_total_item_price(listdata[i]);
	}
}

int main(int argc, char *argv[])
{
	string prog_name = string (argv[0]);
	//prog -inorder|sortall|sortone file.txt
	if (argc != 3) {
		cerr <<"usage: "<<prog_name<< "-inorder|sortall|sortone "<<"file.txt"<<endl;
		return 0;}
	string mode = string (argv[1]);

	if (mode != "-inorder" && mode != "-sortall" && mode != "-sortone"){ //|| mode != "-sortall" || mode != "-sortone"){ 
		cerr <<"usage: "<<prog_name<< " -inorder|-sortall|-sortone "<<"file.txt"<<endl;
		return 0;}
	// set program mode from command line arg

	vector <fruit> listdata;//declare array_list<fruit>	
	ifstream fin; istringstream sin;
	fin.open (argv[2]);//open file

	string n,line; float q,p;

	while (getline (fin, line) ){//while (reading more data
		sin.str(line);
		sin >> n >> q >> p ;
		fruit f;
		f.name = n; f.quan = q; f.price = p;
		f.total_item_price = q * p;

		sin.clear();

		listdata.push_back(f);
	} //store data INORDER

	fin.close();//finished file read

	if (mode == "-sortall" || mode == "-sortone")//if (mode == SORTALL || mode == SORTONE) apply std::stable_sort
		stable_sort (listdata.begin(), listdata.end() );

	if (mode == "-sortone") {//combine fruit quanties with same name
		sortone (listdata);}

	print_list (listdata);
	// free array list memory as necessary
	
}//end of main
