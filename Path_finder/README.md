Overview:
The program contains a function that allows the user to specify the file(--input) with zeros, comas, and ones, the source and a destination as a coordinate (ex 1,0), and a search type. The arguments to that function are to be put in in the following order:
"main.py --input -start --goal --search"
An example of a readable file is below:
0,0,0,0,0
0,0,0,0,0
0,1,0,0,0
0,0,0,0,0

The file can be opened with a text editor.
The program then makes use of Class "PathFinder" to organize the data in the input files as cells in the grid by utilizing a simple node class. Majority of the program's functionality has its source in this class.
Then, one of the 3 functions is called to find the shortest path between the source and the sink using the method specified by the user. The nodes are to be traversedThe methods can be "BFS", "DFS", and "astar". All of the search types will have certain priorities when traversing the cells and choosing next ones. If all of the 4 nodes around a particular node are a valid option, the bottom one(on the grid) will be picked first, followed by the right one, top one, and lastly the left one. That allows us to standardize the results. The traditional DFS has been modified to a version similar to iterative deepening in order to find the shortest path. Classical DFS is not a great option for running a shortest path search.
If one wants to run the 3 methods together, one can use "ALL" for the search_type argument to run all three methods in the order above. The results are then are being output on the screen to stdout with the path and the number of total nodes traversed while looking for an optimal path. 
Various error messages will appear if incorrect # of user arguments has been specified, the unknown search type has been specified, or the file does not contain the data of proper analyzable format. The order of the command-line arguments does not necessarily matter. There is also a function that will print the grid in the command line to facilitate the process of verifying a shortest path manually but is currectly not active.
PEP 257 convention has been followed for code documentation. 

Instructions for running the code locally:
Visual Studio Code was used when developing and testing this program. Python 3.9.6. 
The program requires the input file that can be named anything.
Make sure the input file is located in the same directory as the program itself. There is no output file.
Python will have to be installed in order for the execution of this program to happen. Go to https://www.python.org/ to download it.
The program main.py then can be opened in Python IDLE and run in the Shell with the appropriate arguments as mentioned above.
Running the program in terminal will work as well.

Examples of possible commands to run the program:
% python3 main.py --input grid1.dat --start 0,0 --goal 7,6 --search ALL  
% python3 main.py --input grid1.dat --start 0,0 --goal 7,6 --search BFS
% python3 main.py --input grid1.dat --start 0,0 --goal 7,6 --search DFS
% python3 main.py --input grid1.dat --start 0,0 --goal 7,6 --search astar    