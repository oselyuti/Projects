#ifndef __PPM_H__
#define __PPM_H__

#include <iostream>

struct RGB { 
	RGB() {
		R = '\0'; 
		G = '\0'; 
		B = '\0'; 
	}

	unsigned char R, G, B;
};

class PPM { 
	public:
		PPM(int rows =0, int cols=0) { 
			maxv = 255;
			type = "P6";
			Nrows=rows; Ncols =cols;
			img = new RGB * [Nrows];
			img[0] = new RGB [Nrows*Ncols];
			for (int i=1; i < Nrows ; i++){
				img[i] = img[i-1] + Ncols;
			}
		}
		~PPM() {
       if (img) {
			    delete [] img[0];
          delete [] img;
      }
		}

		void read(string filename);
		void write(string filename);

		RGB *operator[](int i) { return img[i]; }

		int get_Nrows() { return Nrows; }
		int get_Ncols() { return Ncols; }

	private:
		//data members
		int maxv;
		string type;
		int Nrows, Ncols;

		RGB **img;
};



#endif
