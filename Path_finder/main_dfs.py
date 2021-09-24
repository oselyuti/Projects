
import sys
import getopt


class node:
    def __init__(self, i=-1, j=-1):
        self.value = -1  # 0 if possible to move there 1 if not
        self.bottom = None
        self.right = None
        self.left = None
        self.top = None
        self.i = i
        self.j = j
        self.neighbors = []
        self.prev = None
        self.visited = 0



class PathPlanner:
    #def __init__(self, root, node_grid, ):
        
    def dfs(self, root):
        if root.i == -1 and root.j == -1:
            print("root is not initialized")
            return
        stack = [root]  # using list as a stack
        while len(stack) > 0:
            cur_node = stack.pop()
            if cur_node.left != None:
                stack.append(cur_node.left)
            elif cur_node.top != None:
                stack.append(cur_node.top)
            elif cur_node.right != None:
                stack.append(cur_node.right)
            elif cur_node.bottom != None:
                stack.append(cur_node.bottom)


"""
    def bfs(self, root, grid, node_grid):

        if root.i == -1 and root.j == -1:
            print("root is not initialized")
            return
        stack = [root]  # using list as a stack
        while len(stack) > 0:
            cur_node = stack.pop()
            if cur_node.left != None:
                stack.append(cur_node.left)
            elif cur_node.top != None:
                stack.append(cur_node.top)
            elif cur_node.right != None:
                stack.append(cur_node.right)
            elif cur_node.bottom != None:
                stack.append(cur_node.bottom)
"""

""" 
    def bfs(self, root):

        queue = []
        queue.append(root)
        number_traversed = 0
        cur_node = root

        # cur_node.i == goal_node.i and cur_node.j == goal_node.j) == False and
        while len(queue) > 0:
            cur_node = queue.pop(0)
            cur_node.visited = 1

            node_grid[cur_node.i][cur_node.j] = cur_node

            #print (cur_node.i,cur_node.j)
            number_traversed += 1

            if (cur_node.i == goal_node.i and cur_node.j == goal_node.j) == True:
                print("Found the path to the goal")
                break

            if (cur_node.i + 1) < num_rows:
                #print ("try to go down given that we haven't visited the bottom from the top node")
                if (grid[cur_node.i + 1][cur_node.j] == 0):  # free to go
                    cur_node.bottom = node(cur_node.i + 1, cur_node.j)
                    if (node_grid[cur_node.i+1][cur_node.j].visited == 0):

                        cur_node.bottom.visited = 1
                        cur_node.bottom.top = cur_node
                        node_grid[cur_node.i+1][cur_node.j] = cur_node.bottom
                        queue.append(cur_node.bottom)
                        cur_node.bottom.prev = cur_node

                        #print ("went DOWN")

            if (cur_node.j + 1 < num_cols):
                #print ("try to go right if the grid size allows and have not visited yet from this node")
                if (grid[cur_node.i][cur_node.j + 1] == 0):  # see if i can go

                    cur_node.right = node(cur_node.i, cur_node.j+1)
                    if (node_grid[cur_node.i][cur_node.j+1].visited == 0):
                        cur_node.right.visited = 1
                        cur_node.right.left = cur_node
                        node_grid[cur_node.i][cur_node.j+1] = cur_node.right
                        queue.append(cur_node.right)
                        cur_node.right.prev = cur_node

                        #print ("went RIGHT")

            if (cur_node.i - 1) >= 0 and ((cur_node.top) and cur_node.top.visited == 0):
                #print ("try to go UP given that we haven't visited the Top from the bottom node")
                if (grid[cur_node.i - 1][cur_node.j] == 0):  # free to go
                    cur_node.top = node(cur_node.i + 1, cur_node.j)
                    if (node_grid[cur_node.i-1][cur_node.j].visited == 0):
                        cur_node.top.visited = 1
                        cur_node.top.bottom = cur_node
                        node_grid[cur_node.i-1][cur_node.j] = cur_node.top
                        queue.append(cur_node.top)
                        cur_node.top.prev = cur_node
                        #print ("went UP")

            if (cur_node.j - 1 >= 0) and ((cur_node.left) and cur_node.left.visited == 0):
                #print ("try to go left if the grid size allows and have not visited yet from this node")
                if (grid[cur_node.i][cur_node.j - 1] == 0):  # see if i can go
                    cur_node.left = node(cur_node.i, cur_node.j-1)
                    if (node_grid[cur_node.i][cur_node.j-1].visited == 0):
                        cur_node.left.visited = 1
                        cur_node.left.right = cur_node
                        node_grid[cur_node.i][cur_node.j-1] = cur_node.left
                        queue.append(cur_node.left)
                        cur_node.left.prev = cur_node
                        #print ("went RIGHT")
        print("Traversed : ", number_traversed)

        n == node()
        n = node_grid[goal_node.i][goal_node.j]
        route = []
        traversed = 0
        while n:
            traversed += 1
            route.append(n)
            n = n.prev
        route.reverse()

        for n in route:
            print(n.i, n.j)
        print("BFS Number from stat to goal: ", traversed)

"""




def main_dfs(argv):
    filename = ''
    start_node = ''
    goal_node = ''
    search_type = ''

    try:
        opts, args = getopt.getopt(
            argv, "", ["input=", "start=", "goal=", "search="])
    except getopt.GetoptError:
        print("main.py --input -start --goal --search")
        sys.exit(2)
    if (len(sys.argv) != 9):
        print("Incorrect number of arguments")
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
        else:
            print("Usage: main.py --input -start --goal --search")
            sys.exit(3)

    num_rows = 0
    num_cols = 0
    file = open(filename, 'r')
    for line in file:
        num_rows += 1
        for i in line:
            if (num_rows == 1 and i != ',' and (i == '0' or i == '1')):
                num_cols += 1
            if (i != '1' and i != '0' and i != ',' and i != '\n'):
                print('Grid contains unnaceptable symbols (not 1, comma, or 0)')
                sys.exit()

    file.close()

    s = start_node.split(',')
    # create a class instance with s[0],s[1]
    if (s[0].isdigit() == False or int(s[0]) >= num_rows or (s[1].isdigit() == False or int(s[1]) >= num_cols)):
        print("Start coordinate is invalid or is beyond the size of the grid")
        sys.exit()
    # use s[0],s[1] to set the root
    root = node(int(s[0],), int(s[1]))

    s = goal_node.split(',')
    # create a class instance with (int)s[0],s[1]
    if (s[0].isdigit() == False or int(s[0]) >= num_rows or (s[1].isdigit() == False or int(s[1]) >= num_cols)):
        print("Goal coordinate is invalid or is beyond the size of the grid")
        sys.exit()
    goal_node = node(int(s[0],), int(s[1]))

    if search_type in "BFS":
        # execute bfs
        i = 0
    elif search_type == "DFS":
        i = 0
    elif search_type == "astar" or search_type == "A*":
        i = 0
    elif search_type == "ALL":
        i = 0
    else:
        print("Unknown search_type has been specified. Exiting...")
        sys.exit()

    

    # create a grid
    file = open(filename, 'r')
    row_index = 0
    col_index = 0
    rows, cols = (num_rows, num_cols)
    n = node()
    grid = [[0 for i in range(cols)] for j in range(rows)]
    node_grid = [[node() for i in range(cols)] for j in range(rows)]
    for line in file:
        for j in line:
            if j == ',' or j == '\n':
                continue
            grid[row_index][col_index] = int(j)

            # make a grid of nodes too
            new_node = node(row_index, col_index)
            new_node.value = int(j)

            
            
            node_grid[row_index][col_index] = new_node
            

            col_index += 1
        row_index += 1
        col_index = 0
    file.close()

    # Consider a case when a goal_node is an obstacle. The program then gracefully quits
    if (grid[goal_node.i][goal_node.j]) == 1:
        print("The goal coordinate provided is an obstacle and cannot be reached")
        sys.exit()



    #initialize all of the grid nodes to know about their neighbors
    
    n = node()
    for line in node_grid:
        for n in line:
            #print (n.i,n.j)


            if (n.i+1) < num_rows:
                n.bottom = node_grid[n.i+1][n.j]
            if (n.i-1) >=0:
                n.top = node_grid[n.i-1][n.j]
            if (n.j+1) < num_cols:
                n.right = node_grid[n.i][n.j+1]
            if (n.j-1) >=0:
                n.left = node_grid[i][n.j-1]
            
            n.neighbors = [n.left, n.top, n.right, n.bottom]
            #n.neighbors.append (n.left);n.neighbors.append (n.top);n.neighbors.append(n.right);n.neighbors.append(n.bottom)            

            
        
            
    #start dfs
    if search_type == "DFS":
        visited = []
        found_path = False
        n = root #index use only
        stack = [node_grid[root.i][root.j] ]
        while len(stack) > 0:
            n = stack.pop()
            n.visited = 1

            if n not in visited:
                visited.append(n)
            
            if (n.i == goal_node.i and n.j == goal_node.j):
                found_path = True
                break

            #push the neighboring cells that are viable options on the stack to be considered
            list_of_cells = node_grid[n.i][n.j].neighbors
            i = node ()
            for i in list_of_cells:
                if (i and i.visited == 0 and i.value ==0):
                    stack.append(i)
            
        
        if found_path:
            print ("Path: [", sep = "", end = '')
            n = node()
            for n in visited:
                print ("(",n.i,", ", n.j, ")", sep = '', end = '')#add a comma between the tuples
            print (']', sep ='')
            print ("Traversed_to_goal:", len(visited))
        else:
            print("No path has been found to your goal node.")
            sys.exit()
            

if __name__ == "main_dfs":
    main_dfs(sys.argv[1:])

