#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

enum dir {
	left, up, right, down};

struct cell {
	int row; int col;
	cell (int i=0,int j =0) {row = i; col = j;}	
};


bool solve (bool ***wall, int **cells_visited, cell *stack, int &stack_size, cell &current, cell &sink) {
	cell cur = current;
	cell next;

	cells_visited[cur.col][cur.row] = 1;
	stack[stack_size] = cur;
	stack_size++;

	if ( (sink.col == cur.col) && (sink.row == cur.row) ) {//done
		//printf ("Stop recursion when i = %d and j = %d", cur.col, cur.row);
		return true;//stop recursion

	}

	int direction; //needs to be known outside of loop for next cell 
	for (direction=0; direction< 4; direction++) { //for every direction from current cell
		if (wall[cur.col][cur.row][direction] == true ){//a wall in this direction exists
			continue; //go on until there is no wall and a next cell can be updated
		}

		//here if the next cell is known
		if (direction == left ){	next.col = cur.col - 1; next.row = cur.row;}
		else if (direction == right) {next.col = cur.col + 1; next.row = cur.row;}

		else if (direction == up) {next.col = cur.col; next.row = cur.row - 1;}
		else if (direction ==down) {next.col = cur.col; next.row = cur.row +1;}

		if (cells_visited[next.col][next.row] == (1) ) {continue;}
		if (solve ( wall, cells_visited, stack, stack_size, next, sink) == true) {// if solution found
			return true;}

	}


	cells_visited[cur.col][cur.row] = false;
	stack[stack_size-1] =cell();//pop the stack
	stack_size -=1;
	return false;

}

int get_wall_direction (int c1, int r1, int c2, int r2){//gives direction from cell (col1,row1) to cell (col2,row2)
	if (c1 == c2){//cols the same
		if (r1 < r2) return down;
		if (r1 > r2) return up;
	}

	else if (r1 ==r2) {//must be the case because the two cells are never the same
		if (c1 < c2)  return right;
		if (c1 > c2)  return left;
	}

	return -1;//if fails
	printf ("Could not get wall direction");
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf ("usage: ./Mazesolve maze.txt path.txt\n");//print usage error message to stderr
		return 0;
	} 

	int Nrow=0;
	int Ncol=0;

	FILE * maze = fopen ("maze.txt", "r");//open input file for reading ("r")
	fscanf (maze, "MAZE %d %d", &Nrow, &Ncol);//determine Nrow,Ncol from input file
	if (maze == NULL) { printf ("no such file."); return 0;}

	FILE * path = fopen ("path.txt", "w");//open output file for writing

	//create array of walls for each grid cell
	//initialize to have boundary walls set and
	//interior walls unset


	bool *** wall; //bool wall[Ncol][Nrow][4];
	wall = new bool ** [Ncol];//dynamically allocate memory for convenient argument passage

	for (int i=0; i < Ncol; ++i){
		wall[i] = new bool * [Nrow];
		for (int j=0; j < Nrow; ++j) {
			wall[i][j] = new bool [4];
		}
	}

	for (int i = 0; i < Ncol; ++i ){//no walls exist yet
		for (int j=0 ; j < Nrow; ++j){
			if (i != 0)	wall[i][j][left] = false;
			else {		wall[i][j][left] = true;}//leftmost very first column

			if (j != 0)		wall[i][j][up] = false;
			else {	wall[i][j][up] = true;}//first row

			if (i != Ncol-1)	wall[i][j][right] = false;
			else {	wall[i][j][right] = true;} //last column

			if (j != Nrow-1)	wall[i][j][down] = false;
			else {	wall[i][j][down] = true;}//last row
		}
	}


	//read input file, set interior walls
	int c1=0,r1=0,c2=0,r2 =0;
	while ( fscanf (maze, "%d %d %d %d", &r1, &c1, &r2, &c2 ) == 4 ) {
		//determine the direction of the wall with regard to (y1,x1)
		wall [c1][r1] [get_wall_direction (c1,r1, c2, r2) ]	= true;
		wall [c2][r2] [get_wall_direction (c2,r2, c1, r1) ]	= true;
	}


	//initalize DFS path computation
	cell source (0,0);
	cell sink (Nrow-1,Ncol-1);

	cell *stack = new cell [Nrow*Ncol] ;//data structure to track the path
	int stack_size =0;//.size()

	int ** cell_visited;//int cell_visited[Ncol][Nrow];//will be 0 initially. If visited, will be changed to 1
	cell_visited = new int *[Ncol];
	for (int j=0; j < Ncol; ++j){
		cell_visited[j] = new int [Nrow];
	}
	for (int i=0; i< Ncol; ++i){
		for (int j=0; j < Nrow; ++j) {
			cell_visited[i][j] = 0;}
	}


	//carry out DFS source-to-sink computation
	solve (wall, cell_visited, stack, stack_size, source, sink);

	//file header
	fprintf (path, "PATH %d %d\n", Nrow, Ncol);


	int i=0;//printing source to sink
	while (i < stack_size) {
		int row1 = stack[i].row;
		int col1 = stack[i].col;
		++i;

		fprintf (path, "%d %d\n", row1, col1);	
	}



	delete [] stack;//deallocate stack

	for (int i=0; i < Ncol; ++i) {//deallocate 3D array
		for (int j=0; j < Nrow; ++j) {
			delete [] wall[i][j];}
		delete [] wall[i];}
	delete[] wall;




	fclose (maze);
	fclose (path);//close open files
}//END OF MAIN
