#python main.py --input FILENAME --start START_NODE --goal GOAL_NODE --search SEARCH_TYPE.

import sys,getopt



class node ():
    def __init__(self, i = -1, j = -1) :
        self.i = i
        self.j = j
        self.down = None
        self.right = None
        self.up = None
        self.left = None
        self.next = None
        self.parent = None
    def move_up(self):
        self.







def main (argv) :
    filename = ''
    start_node = ''
    goal_node = ''
    search_type = ''
    
    try:
      opts, args = getopt.getopt(argv,"",["input=","start=", "goal=", "search="])
    except getopt.GetoptError:
      print ('python main.py --input FILENAME --start START_NODE --goal GOAL_NODE --search SEARCH_TYPE')
      sys.exit(2)
    if len (sys.argv) != 9 :
        #print (len(sys.argv))
        print ('python main.py --input FILENAME --start START_NODE --goal GOAL_NODE --search SEARCH_TYPE')
        sys.exit(2)

    for opt, arg in opts :
        if opt in '--input':
            filename = arg
        elif opt in '--start':
            start_node = arg
        elif opt in '--goal':
            goal_node = arg
        elif opt in '--search':
            search_type = arg
        else:
            print ("unknown command line argument detected", arg, '\n')
            sys.exit(3)
    #print ('Input file is', filename)
    #print ('start:', start_node)
    #print ('Goal:', goal_node)
    #print ('Search_type is', search_type)


    num_cols = 0
    num_rows = 0
    file = open (filename, 'r')
    for line in file:
        num_rows+=1
        #print (line)
        for i in line:
            if ( (i == '1' or i == '0') and num_rows==1):
                num_cols+=1
            if (i != '0' and i != '1' and i!= ',' and i != '\n'):
                print ("Grid file contains something other than 0, ,(comma), or 1")
                sys.exit (4)


    s = start_node.split(',')
    if (int (s[0]) >= num_rows or s[0].isdigit() == False):
        print ('Incorrect grid coordinate for rows :', s[0],'(not in the grid)')
        sys.exit()
    elif (int(s[1]) >= num_cols or s[1].isdigit() == False):
        print ('Incorrect grid coordinate for the number of rows (', s[1], ')')
        sys.exit()

    s = goal_node.split(',')
    if (int (s[0]) >= num_rows or s[0].isdigit() == False):
        print ('Incorrect grid coordinate for rows :', s[0],'(not in the grid)')
        sys.exit()
    elif (int(s[1]) >= num_cols or s[1].isdigit() == False):
        print ('Incorrect grid coordinate for the number of rows (', s[1], ')')
        sys.exit()



    if (search_type == 'BFS'):
        #run bfs
        i=1
    elif (search_type == 'DFS'):
        #run dfs
        i=2
    elif (search_type == "A*" or search_type == "astar"):
        #run
        i=3
    elif (search_type == "ALL"):
        i=4
    else: 
        print ('Known option for "--search" argument is not provided')

if __name__ == "__main__":
   main(sys.argv[1:])