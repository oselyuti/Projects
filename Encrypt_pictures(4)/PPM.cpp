#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

#include "PPM.h"

void PPM::read(string file) { 
	ifstream fin;
	fin.open (file.c_str());
	if (fin.fail() ){ cerr<< "Failed to open input file.\n "; exit(1);}

	fin>>type;
	if (type != "P6") { cerr << "Not P6 \n"; exit(1);}

	fin>>Ncols >> Nrows >> maxv;
	if (maxv >255) {cerr <<" Max value is too high\n"; exit(1);} 

	int num_bytes = Nrows * Ncols; 


	while (fin.get() != '\n' ) {};//skip the trailing whitespace


	img = new RGB * [Nrows];
	img[0] = new RGB [Nrows*Ncols];
	for (int i=1; i < Nrows ; i++){
		img[i] = img[i-1] + Ncols;}

	fin.read( (char *)img[0], 3*num_bytes); 

	fin.close();
}

void PPM::write(string file) { 
	char c='\0';
	int size =0;
	int o=0;
	while (file[o] >> c) {size++; o++;}

	string new_file; 

	for (int i=0; i< size; i++ ){
		if (file [i] =='.') {break;}
		new_file +=file[i];
	}	

	new_file += "_wmsg.ppm";
	//cout<<new_file<<'\n';

	ofstream fout (new_file.c_str(), ios::out | ios::binary);

	if (!fout.is_open()) {cerr<<"Failed to open the new file. \n" ; exit(1);}

	fout << type << '\n'<< Ncols << ' ' <<Nrows << '\n' << maxv <<'\n';

	fout.write ( (char * ) img[0], Nrows*Ncols * 3);
	//cout<<"success";


}
