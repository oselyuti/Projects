import sys, getopt



class PathPlanner:
    def bfs(self, root):
        if root.i == None and root.j == None:
            print ("root is not initialized")
            return
        stack = [root] # using list as a stack
        while len(stack) >0:
            cur_node = stack.pop()
            if cur_node.left != None:
                stack.append(cur_node.left)
            elif cur_node.top != None:
                stack.append(cur_node.top)
            elif cur_node.right != None:
                stack.append(cur_node.right)
            elif cur_node.bottom != None:
                stack.append(cur_node.bottom)

            


class node:
    def __init__(self, i = -1, j = -1):
        self.value = -1#0 if possible to move there 1 if not
        self.bottom = None
        self.right = None
        self.left=  None
        self.top  = None
        self.i = i
        self.j = j
        self.parent = None
        self.prev = None
        self.visited = 0



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
    root = node (int(s[0],), int(s[1]))

    s = goal_node.split(',')
    #create a class instance with (int)s[0],s[1]
    if (s[0].isdigit() ==False or int (s[0]) >= num_rows or (s[1].isdigit() == False or int (s[1]) >=num_cols)):
        print ("Goal coordinate is invalid or is beyond the size of the grid")
        sys.exit()
    goal_node = node (int(s[0],), int(s[1]))




    if search_type in "BFS":
        #execute bfs
        i=0
    elif search_type == "DFS":
        i=0
    elif search_type == "astar" or search_type == "A*":
        i=0
    elif search_type == "ALL":
        i=0
    else:
        print ("Unknown search_type has been specified. Exiting...")
        sys.exit()


    #build a tree


    #create a grid
    file = open (filename, 'r')
    row_index = 0; col_index = 0
    rows,cols = (num_rows,num_cols)
    n = node ()
    grid = [[0 for i in range(cols)] for j in range(rows)]
    node_grid = [[node () for i in range(cols)] for j in range(rows)]
    for line in file:
            for j in line:
                if j == ',' or j == '\n':
                    continue
                grid[row_index][col_index] = int(j)

                #make a grid of nodes too
                new_node = node (row_index, col_index)
                node_grid[row_index][col_index] = new_node



                col_index+=1
            row_index+=1
            col_index = 0
    file.close()



    #Consider a case when a goal_node is an obstacle. The program then gracefully quits
    if (grid[goal_node.i][goal_node.j]) == 1:
        print ("The goal coordinate provided is an obstacle and cannot be reached")
        sys.exit ()



    queue = []
    queue.append(root)
    number_traversed =0
    cur_node = root

    #cur_node.i == goal_node.i and cur_node.j == goal_node.j) == False and
    while len(queue)>0:
        cur_node = queue.pop(0)
        cur_node.visited = 1
        
        node_grid[cur_node.i][cur_node.j] = cur_node


        #print (cur_node.i,cur_node.j)
        number_traversed+=1


        if (cur_node.i == goal_node.i and cur_node.j == goal_node.j) == True:
            print ("Found the path to the goal")
            break


        if (cur_node.i +1) <num_rows :
            #print ("try to go down given that we haven't visited the bottom from the top node")
            if (grid[cur_node.i +1][cur_node.j] == 0):#free to go
                cur_node.bottom = node (cur_node.i +1, cur_node.j)
                if ( node_grid[cur_node.i+1][cur_node.j].visited == 0 ):
                    
                    cur_node.bottom.visited = 1
                    cur_node.bottom.top = cur_node
                    node_grid[cur_node.i+1][cur_node.j] = cur_node.bottom
                    queue.append (cur_node.bottom)
                    cur_node.bottom.prev = cur_node

                    #print ("went DOWN")
        
        if (cur_node.j +1 < num_cols):
            #print ("try to go right if the grid size allows and have not visited yet from this node")
            if (grid[cur_node.i][cur_node.j + 1] == 0 ):# see if i can go
                
                cur_node.right  = node (cur_node.i, cur_node.j+1)
                if (node_grid[cur_node.i][cur_node.j+1].visited == 0):
                    cur_node.right.visited = 1
                    cur_node.right.left = cur_node
                    node_grid[cur_node.i][cur_node.j+1] = cur_node.right
                    queue.append  (cur_node.right)
                    cur_node.right.prev = cur_node

                    #print ("went RIGHT")

        if (cur_node.i -1) >= 0 and ( (cur_node.top) and cur_node.top.visited == 0 ):
            #print ("try to go UP given that we haven't visited the Top from the bottom node")
            if (grid[cur_node.i -1][cur_node.j] == 0):#free to go
                cur_node.top = node (cur_node.i +1, cur_node.j)
                if (node_grid[cur_node.i-1][cur_node.j].visited == 0):
                    cur_node.top.visited = 1
                    cur_node.top.bottom = cur_node
                    node_grid[cur_node.i-1][cur_node.j] = cur_node.top
                    queue.append (cur_node.top)
                    cur_node.top.prev = cur_node
                    #print ("went UP")

        if (cur_node.j -1 >=0) and ( (cur_node.left) and cur_node.left.visited == 0):
            #print ("try to go left if the grid size allows and have not visited yet from this node")
            if (grid[cur_node.i][cur_node.j - 1] == 0 ):# see if i can go
                cur_node.left  = node (cur_node.i, cur_node.j-1)
                if (node_grid[cur_node.i][cur_node.j-1].visited == 0):
                    cur_node.left.visited = 1
                    cur_node.left.right = cur_node
                    node_grid[cur_node.i][cur_node.j-1] = cur_node.left
                    queue.append (cur_node.left)
                    cur_node.left.prev = cur_node

                    #print ("went RIGHT")        


    print ("Traversed : ",number_traversed)

    n == node()
    n = node_grid [goal_node.i] [goal_node.j]
    route = []
    traversed =0
    while n :
        traversed+=1
        route.append(n)
        n = n.prev
    route.reverse()
    
    for n in route :
        print(n.i, n.j)
    print ("Number from stat to goal: ",traversed)






















"""
    #look for a path from start to goal
    visited = []
    #queue = [root]
    cur_node = root

    #while len(queue) >0:
    while (cur_node.i == goal_node.i and cur_node.j == goal_node.j) == False:
        #queue.append(cur_node)
        #cur_node.visited =1
        prev_node = cur_node
        if cur_node:
            cur_node=queue.pop(0)

        cur_node.visited = 1
        number_traversed+=1
        print (cur_node.i,cur_node.j)

        if (cur_node.i == goal_node.i and cur_node.j == goal_node.j):
            print ("found the path to the goal")
            break
        print ("Number Traversed : ",number_traversed)
        
        if (cur_node.i +1) <num_rows and (cur_node.bottom == None ):
            print ("try to go down")
            if cur_node.bottom == None:
                cur_node.bottom = node (cur_node.i+1, cur_node.j)
                cur_node.bottom.visited = 1
                if (grid[cur_node.i+1][cur_node.j]) == 0 :
                    queue.append (node (cur_node.i+1,cur_node.j))
                    print ("went down")

        if (cur_node.j +1) <num_cols and (cur_node.right == None):
            print ("try to go right")
            cur_node.right = node (cur_node.i, cur_node.j+1)
            cur_node.right.visited=1
            if ((grid[cur_node.i][cur_node.j+1])) == 0 : 
                queue.append (node (cur_node.i,cur_node.j+1))
                print ("went right")

        if (cur_node.i -1) >=0 and (cur_node.top == None):
            print ("try to go up")

            cur_node.top = node (cur_node.i-1, cur_node.j)
            cur_node.top.visited = 1
            if grid[cur_node.i-1][cur_node.j] == 0 :
                    queue.append (node(cur_node.i-1, cur_node.j))
                    print ("went up\n")

        if (cur_node.j -1 >=0 ) and (cur_node.left == None):
            print ("try to go left")

            cur_node.left = node (cur_node.i, cur_node.j -1)
            if ((grid [cur_node.i][cur_node.j-1] == 0) ):#valid 
                queue.append(node (cur_node.i, cur_node.j -1))
                cur_node.left.visited =1

                print ("went left")
                """

if __name__ == "__main__":
    main(sys.argv[1:])
