#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

#include "PPM.h"
#include "Rnumgen.h"


//
//
//This is CRYPTO2. RANDOM pixels
//
//
//
const char ETX = 0x3;

struct pixel {
	pixel (){x=0;y=0;}
	int y;//row
	int x;//column
};

void set_pixel_list( PPM &img, vector <pixel> &pixel_list) {
	int row = img.get_Nrows();
	int col = img.get_Ncols();
	pixel p;
	for (int i = 0; i< row; i+=2){
		for (int j=0; j < col; j+=2){
			p.y = i; p.x = j;

			pixel_list.push_back(p);
		}
	} 
}

void encode(PPM &img, vector <pixel> &pixel_list) {
	char c=0x0;
	unsigned int i=0;//serves to continue encoding with the next char
	int count = 0;
	unsigned char LSB;
	while (cin.get(c) ){//encode the pixels from pixel_list
		//encode all pixel_list members
		//if (c == 0x0A) {break;} //will avoid encoding "enter"
		//only b6-b0 of c need to be changed
		for (unsigned int k=0; k < 7; k++){
			LSB = c >> k ;//extract the  bit from c
			LSB &= 0x1; //extract bit from c
			//replace the LSB of img[][] with the text bit

			if (count %3 == 0) {
				img[pixel_list[i].y][pixel_list[i].x].R &= 0xFE; //zero out the LSB
				img[pixel_list[i].y][pixel_list[i].x].R |= (LSB);//sets the last bit to LSB
			}

			if (count %3 == 1) {
				img[pixel_list[i].y][pixel_list[i].x].G &= 0xFE; //zero out the LSB
				img[pixel_list[i].y][pixel_list[i].x].G |= (LSB);//sets the last bit to LSB
			}

			if (count %3 == 2){
				img[pixel_list[i].y][pixel_list[i].x].B &= 0xFE; //zero out the LSB
				img[pixel_list[i].y][pixel_list[i].x].B |= (LSB);//sets the last bit to LSB
			}
			++i;
			++count;


			if (i ==pixel_list.size() ) {cerr<<"Message too large.\n";return;}

		}
	}//end of input
	//cout<<count<<"- number bits encoded";

	//ETX
	for (int k=0; k<7; k++){
		LSB = ETX >> k ;//extract the  bit from c
		LSB &= 0x1; //extract bit from c

		if (count %3 == 0) {
			img[pixel_list[i].y][pixel_list[i].x].R &= 0xFE; //zero out the LSB
			img[pixel_list[i].y][pixel_list[i].x].R |= (LSB);//sets the last bit to LSB
		}

		if (count %3 == 1) {
			img[pixel_list[i].y][pixel_list[i].x].G &= 0xFE; //zero out the LSB
			img[pixel_list[i].y][pixel_list[i].x].G |= (LSB);//sets the last bit to LSB
		}

		if (count %3 == 2){
			img[pixel_list[i].y][pixel_list[i].x].B &= 0xFE; //zero out the LSB
			img[pixel_list[i].y][pixel_list[i].x].B |= (LSB);//sets the last bit to LSB
		}

		i++;
		count++;
	} 
}


void decode(PPM &img, vector <pixel> &pixel_list) {
	unsigned char d=0x0;
	int count_bits =0;
	for (unsigned int i=0; i< pixel_list.size(); ++i) {
		int row = pixel_list[i].y;
		int col = pixel_list[i].x;
		unsigned char temp = 0x0;


		if (i%3 ==0) {
			temp = ((img[pixel_list[i].y] [pixel_list[i].x].R) & (0x1));
		}
		else if (i%3 ==1) {
			temp = (img[row][col].G & 0x1);}
		else if (i%3 ==2) {
			temp = (img[row][col].B & 0x1);}

		temp = (temp<<count_bits);//adjust
		d = (temp | d );//conjoin the bits

		count_bits++;
		if (count_bits ==7) {//char decoded add 0?
			count_bits =0;
			if (d == ETX) {
				return;}
			cout.put(d);
			d &= 0x0;

		}

	}//end of for

}


void set_pixel_target( PPM &img, int seed, vector <pixel> &pixel_list) {
	//compute histogram for all pixels
	vector <int> histogram (4096,0);//size 4096 0x1000, all values = 0
	int row = img.get_Nrows(); int col = img.get_Ncols();
	unsigned char R,B,G = 0x0;
	int target_int = 0x0;

	for (int i=0; i < row; ++i){
		for (int j=0; j < col; ++j) {//0x78 = 120 = 01111000
			R = (img[i][j].R & 0x78) >>3 ;//0 for all bits that are not within b6...b3
			G = (img[i][j].G & 0x78) >>3 ;
			B = (img[i][j].B & 0x78) >>3 ;
			target_int = (R<<8) | (G<<4) | (B);

			++histogram[target_int];
		}
	}

	//Histogram works properly

	if (seed == 0) seed = time(NULL);

	rnumgen RNG (seed, histogram);

	for (int i = pixel_list.size()-1;  i>0; --i) {

		int first = RNG.rand ();
		int second= RNG.rand ();
		int combined = (first<<12) | (second);//24 - bit 
		swap (pixel_list[i], pixel_list[combined % (i+1)]);
	}   

}



int main(int argc, char *argv[]) {

	string mode;
	string filename;
	if (argc != 3 && argc !=4) {cerr<< "usage: -encode|-decode image.ppm [option=value] \n"; return 0;}
	mode = string (argv[1] );
	if (mode != "-encode" && mode != "-decode" ) {cerr <<"Incorrect mode \n"; return 0;}

	string how_encode = "";
	if (argc == 3){//CRYPTO1
		how_encode = "even";
		filename = string (argv[2]) ; 
	}

	int seed;
	string seed_s;
	if (argc == 4) {//CRYPTO 2
		how_encode = "random";
		seed_s = string (argv[2]);
		filename = string (argv[3]);

		int size =0; int o =0;
		char c = '\0';
		while (seed_s[o] >> c) {++size; ++o;}

		string s, num = "";
		for (int i=0; i<size; i++){
			s+= seed_s[i];
			if (i == 5 && s != "-seed=") {cerr<<"-seed=# is the proper format for the second arg \n"; return 0;}

			if (i>5 && seed_s[i] != '\n') {num +=seed_s[i]; }
		}

		istringstream sin (num);
		if (! (sin>>seed) ) {cerr <<"-seed=some integer \n"; return 0;}
		sin>>seed;
		sin.clear();
	}


	PPM img;
	img.read( filename);

	vector <pixel> pixel_list;//declare pixel_list

	set_pixel_list (img, pixel_list);//set_pixel_list(pixel_list);

	//if crypto 2, i will know by the # of argsi
	if (how_encode == "random") {
		set_pixel_target (img,seed, pixel_list);}


	if (mode == "-encode") {
		encode (img, pixel_list);
		img.write (filename);
	}
	else if (mode == "-decode"){
		decode (img, pixel_list);
	}
}
