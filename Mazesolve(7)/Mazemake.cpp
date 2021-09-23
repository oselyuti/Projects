#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "dset.h"
struct cell { 
	int row;
	int col;
	cell (int i=0, int j=0) {row = i; col = j;}
};

void swap(cell &c1, cell &c2) { 
	int row= c1.row;
	int col= c1.col;

	c1.row = c2.row; c1.col = c2.col;
	c2.row = row; c2.col = col;
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf ("Usage: ./Mazemake nrow ncol maze.txt\n");//fprintf (stderr);//print usage error message to stderr
		return 0;
	}

	int Nrow = atoi(argv[1]);
	int Ncol = atoi(argv[2]);

	// Grid wall:     (i0,j0) | (i1,j1)
	// Horizontal wall: (i,j) | (i+1,j)
	// Vertical wall:   (i,j) | (i,j+1)

	int N = ((Nrow-1) * Ncol) + ((Ncol-1) * Nrow);//int N = number of interior grid walls
	cell wall[N][2];//internal walls between cells

	//populate wall[][] with interior grid walls
	int k=0;
	for (int i=0; i < Ncol-1; ++i){
		for (int j = 0; j < Nrow; ++j){
			
			wall[k][0] = cell (i,j);//current cell
			wall[k][1] = cell (i+1, j);//cell to the right
			k++;
		}
	}
	for (int i=0; i < Ncol; ++i){
		for (int j = 0; j < Nrow-1; ++j){
			wall[k][0] = cell (i,j);//current cell
			wall[k][1] = cell (i, j+1);//cell below
			k++;
		}
	}

	srand (time (NULL));
	//randomly perturb list order: swap based
	for (int i=N-1; i >0; --i) {//i is row
		int ran = int (rand () % (i+1)) ;//generate random number
		swap (wall[i][0], wall [ran][0] );
		swap (wall[i][1], wall [ran][1] );
	}


	//ofstream fout;
	FILE *maze;
	maze = fopen( argv[3], "w");//fout.open ("file.txt");	//open output file for writing (argv[3])
	dset ds(Nrow * Ncol);
//	write MAZE, Nrow, Ncol header 
	fprintf (maze, "MAZE %d %d\n", Nrow, Ncol);//fout <<"MAZE " << Nrow << " "<< Ncol <<'\n';
	int i, ii,jj;//linear representation of [i][j]
	for (i =0; i < N; ++i) {
		ii = wall [i][0].row + (wall[i][0].col * Ncol);
		jj = wall [i][1].row + (wall[i][1].col * Ncol);

		if (ds.find (ii)  != ds.find (jj)  ) ds.merge (ii,jj);
		
		else {fprintf (maze, "%*d%*d%*d%*d\n", 4,wall[i][0].col,4, wall[i][0].row,
												  4,wall[i][1].col,4, wall[i][1].row);
		}
		//else {fout << wall[i][0].col << " " << wall [i][0].row<< " "
		//		   << wall[i][1].col << " " << wall[i][1].row << '\n';}

		if (ds.size () == 1) {//if all cells belong to the same set
			break;}
	}
	i++;//make sure i is incremented (for loop does not do it last time)

	for ( ; i < N ; ++i) {
		fprintf (maze, "%*d%*d%*d%*d\n", 3,wall[i][0].col,4, wall[i][0].row,
												  4,wall[i][1].col,4, wall[i][1].row);
		//fout << wall[i][0].col << " " << wall [i][0].row<< " "
		//	<< wall[i][1].col << " " << wall[i][1].row << '\n';
	}
		

	fclose(maze);//fout.close();
		
	//		for (k=0; k<N; k++) {
//if: pair of cells given by wall[k][] are not
//		connected (belong to different disjoint sets),
//				  merge them
//else: write wall location to file as two pairs 
//	of cell indices: i0 j0 i1 j1
//		if: all cells belong to the same set, break
//		}
//	write any remaining walls to file
//		close output file
//
//
}//END OF MAIN
