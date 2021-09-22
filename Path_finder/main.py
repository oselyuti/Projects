import sys, getopt


'''
class PathPlanner:
    i=0'''


class node:
    def __init__(self, i = None, j = None):
        self.bottom = None
        self.right = None
        self.left=  None
        self.top  = None
        self.i = i
        self.j = j
        self.parent = None


def main (argv):
    filename = ''
    start_node = ''
    goal_node = ''
    search_type = ''

    try :
        opts,args = getopt.getopt(argv, "", ["input=", "start=", "goal=", "search="])
    except getopt.GetoptError:
        print ("main.py --input -start --goal --search")
        sys.exit(2)
    if (len(sys.argv) != 9):
        print ("Incorrect number of arguments")
        sys.exit(2)
    
    #print (argv[1])

    #print (argv[2])
    #print (argv[3])

    for opt, arg in opts:
        if opt in "--input":
            filename = arg
        elif opt in ("--start"):
            start_node = arg    
        elif opt in ("--goal"):
            goal_node = arg
        elif opt in ("--search"):
            search_type = arg
        else :
            print ("Usage: main.py --input -start --goal --search")
            sys.exit(3)
    

    num_rows = 0; num_cols = 0
    file = open (filename, 'r')
    for line in file:
       num_rows +=1
       for i in line:
            if (num_rows == 1 and i != ',' and (i =='0' or i == '1')):
               num_cols+=1
            if (i != '1' and i != '0' and i != ',' and i != '\n'):
                print ('Grid contains unnaceptable symbols (not 1, comma, or 0)')
                sys.exit()
    
    file.close()

    
    s = start_node.split(',')
    #create a class instance with s[0],s[1]
    if (s[0].isdigit() ==False or int (s[0]) >= num_rows or (s[1].isdigit() == False or int (s[1]) >=num_cols)):
        print ("Start coordinate is invalid or is beyond the size of the grid")
        sys.exit()
    #use s[0],s[1] to set the root
    s = goal_node.split(',')
    #create a class instance with (int)s[0],s[1]
    if (s[0].isdigit() ==False or int (s[0]) >= num_rows or (s[1].isdigit() == False or int (s[1]) >=num_cols)):
        print ("Goal coordinate is invalid or is beyond the size of the grid")
        sys.exit()






    if search_type == "BFS":
        #execute bfs
        i=0
    if search_type == "DFS":
        i=0
    if search_type == "astar" or search_type == "A*":
        i=0
    if search_type == "ALL":
        i=0
    else:
        print ("Unknown search_type has been specified. Exiting...")
        sys.exit()



    file = open (filename, 'r')
    for i in file:
            print (i)
    file.close()


if __name__ == "__main__":
    main(sys.argv[1:])
