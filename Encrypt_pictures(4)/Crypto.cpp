#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

#include "PPM.h"
#include "Rnumgen.h"

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
		//cout<<"Char received"<<'\n';
		//int shift = 0;
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
			//shift LSB once to the right to get the bit to the left.
			//++shift;
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

	//cout<<count<<"- number bits encoded";



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



int main(int argc, char *argv[]) {

	string mode;
	string filename;
	if (argc != 3 && argc !=4) {cerr<< "usage: -encode|-decode image.ppm [option=value] \n"; return 0;}
	mode = string (argv[1] );
	if (mode != "-encode" && mode != "-decode" ) {cerr <<"Incorrect mode \n"; return 0;}

	filename = string (argv[2]) ; 
	/*
	   int seed;
	   string seed_s;
	   if (argc == 4) {
	   seed_s = string (argv[2]);

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
	   sin>>seed;
	//add unknown OPTION CHECK
	}
	*/
	PPM img;
	img.read( filename);
	vector <pixel> pixel_list;//declare pixel_list

	set_pixel_list (img, pixel_list);//set_pixel_list(pixel_list);

	if (mode == "-encode") {
		encode (img, pixel_list);
		img.write (filename);
	}
  else if (mode == "-decode"){
      decode (img, pixel_list);
      }
}
