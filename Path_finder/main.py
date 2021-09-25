
from os import stat_result
import sys
import getopt
import math


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
        self.distance_to_goal = math.inf
    
def print_path (visited):
    i =1
    print ("Path: [", sep = "", end = '')
    n = node()
    for n in visited:
        if (i == len (visited)):
            print ((n.i, n.j), end='')
        else:
            print ((n.i, n.j) ,",", sep = '' , end = '')#add a comma between the tuples
        i+=1
    print (']', sep ='')
    print ("Traversed_to_goal:", len(visited))



class PathPlanner:
    def bfs(root, grid, node_grid, num_rows, num_cols,goal_node):
        found_path = False

        queue = []
        queue.append(root)
        traversed = 0
        cur_node = root

        # cur_node.i == goal_node.i and cur_node.j == goal_node.j) == False and
        while len(queue) > 0:
            cur_node = queue.pop(0)
            cur_node.visited = 1
            node_grid[cur_node.i][cur_node.j] = cur_node
            traversed += 1

            if (cur_node.i == goal_node.i and cur_node.j == goal_node.j) == True:
                found_path= True
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

        n = node_grid[goal_node.i][goal_node.j]
        route = []
        while n:
            route.append(n)
            n = n.prev
        route.reverse()

        return found_path, route, traversed

        #for n in route:
        #    print(n.i, n.j)
        #print("BFS Number from stat to goal: ", traversed)

                

    def dfs (node_grid,root,goal_node):
        visited = []
        found_path = False
        traversed =0
        n = root #index use only
        stack = [node_grid[root.i][root.j] ]
        while len(stack) > 0: 
            n = stack.pop()
            print ("Now considering : ",n.i,n.j)
            
            n.visited = 1
            traversed+=1

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
                    print ("Stack appended: ", i.i, i.j, end= ' ')

            print ()
            #PathPlanner.dfs (node_grid, n, goal_node)

        return found_path, visited, traversed

    def deepening_dfs (node_grid, root, goal_node):
        start = node_grid[root.i][root.j]
        cur_depth = 1
        goal = False
        goal_i = goal_node.i; goal_j = goal_node.j
        
        list_of_neighbors_at_this_level = [start]
        start.visited = 1
        nodes_traversed = 0

        while (goal == False):  
            cur_depth+=1
            print(cur_depth)
            for individual in list_of_neighbors_at_this_level:
                #print ("\nIndividual:",individual.i, individual.j)
                #print ("Their neighbors:")
                #get their list of adjacent cells
                for neighbor_of_neighbor in individual.neighbors:
                    nodes_traversed+=1
                    if (neighbor_of_neighbor and neighbor_of_neighbor.visited == 0 and neighbor_of_neighbor.value ==0):
                        list_of_neighbors_at_this_level.append(neighbor_of_neighbor)
                        neighbor_of_neighbor.visited = 1
                        neighbor_of_neighbor.prev = individual
                        #print (neighbor_of_neighbor.i, neighbor_of_neighbor.j)
                        if neighbor_of_neighbor.i == goal_i and neighbor_of_neighbor.j == goal_j:
                            #print ("found\n")
                            goal=True
                            break
                list_of_neighbors_at_this_level.remove(individual)#make sure only the new connections are in the list
            #print(n.i, n.j)
            #the KEY IS TO GO THROUGH EVERY ELEMENT OF THE STACK AT ONE cur_depth INSTEAD OF GOING STEP BY STEP AND POPPING FROM THE STACK ONE BY ONE
            
            #one level at a time
        #print (nodes_traversed)   
        n = node_grid[goal_node.i][goal_node.j]
        route = []
        while n:
            route.append(n)
            n = n.prev
        route.reverse()

        return goal, route, nodes_traversed    


    def astar(node_grid,root, goal_node):#DOES SOME WEIRD SHIT 0,0 TO 5,4 GRID2
        visited = []
        found_path = False
        traversed=0
        n = root #index use only
        stack = [node_grid[root.i][root.j] ]
        while len(stack) > 0:
            n = stack.pop()
            n.visited = 1
            traversed+=1
            if n not in visited:
                visited.append(n)
            if (n.i == goal_node.i and n.j == goal_node.j):
                found_path = True
                break

            #prepare for A*
            list_of_cells = node_grid[n.i][n.j].neighbors
            for i in range(4):

                loc = list_of_cells[i]
                if loc != None and loc.visited != 1 and loc.value != 1:# cell exists has distance can be calculated
                    loc.distance_to_goal = math.sqrt( ((loc.i - goal_node.i)**2) + ((loc.j- goal_node.j)**2) )

            #rearrange the list of neighbors for the cell n to reflect the order in which to try neighboring cells
            #START STATE: left,top,right, bottom
            for i in range(4):
                low_priority = n.neighbors[i]
                if low_priority: #check whether the neighbor is within the grid
                    for k in range(4):
                        higher_priority = n.neighbors[k]
                        if higher_priority:
                            if (low_priority.distance_to_goal < higher_priority.distance_to_goal and i!=k and i<k):#if there is more than 1 optimal path, check the priority
                                #the node with higher priority will have a higher index
                                #swap places if the node at the start of the list has lesser distance to goal than the one at the end(bottom, for example)
                                #the element with the lowest priority and the lowest distance then bubbles up to the top of the neighbor list
                                tmp = higher_priority
                                n.neighbors[k] = low_priority
                                n.neighbors[i] = tmp
    
            
            
            list_of_cells = node_grid[n.i][n.j].neighbors
            #the cells to be explored in order of the smallest (g+f) first, meaning they will pushed on stack last
            list_of_cells = node_grid[n.i][n.j].neighbors
            for i in list_of_cells:
                if (i and i.visited == 0 and i.value ==0):
                    stack.append(i)

        return found_path, visited, traversed
            
        





def main(argv):
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
            n.bottom=None
            n.right = None
            n.top = None
            n.left = None
            if (n.i+1) < num_rows:
                n.bottom = node_grid[n.i+1][n.j]
            if (n.i-1) >=0:
                n.top = node_grid[n.i-1][n.j]
            if (n.j+1) < num_cols:
                n.right = node_grid[n.i][n.j+1]
            if (n.j-1) >=0:
                n.left = node_grid[n.i][n.j-1]
            
            
            n.neighbors = [n.left, n.top, n.right, n.bottom]
            if (search_type =="DFS"):
                n.neighbors = [n.bottom,n.right,n.top,n.left]
            
        
    if search_type == "BFS":
        result = PathPlanner.bfs(root, grid, node_grid, num_rows, num_cols,goal_node)
    #start dfs
    elif search_type == "DFS":
        result = PathPlanner.deepening_dfs(node_grid, root, goal_node)
        #discovered= [];count =0
        #PathPlanner.DFS (node_grid,  node_grid[root.i][root.j], discovered, count, goal_node)
        #result = PathPlanner.dfs(node_grid,root,goal_node)
    #A*
    #g(n) = 1 for all, so I will discard it
    elif search_type == "astar":
        result = PathPlanner.astar(node_grid,root,goal_node)
    else:
        print ("Unknown search type has been specified")
        sys.exit()
        


    path_exists = False
    visited = []
    total_nodes_traversed =0
    if (result[0] == True):
            path_exists = True
            visited = result[1]
            total_nodes_traversed = result[2]
    



    if path_exists:
        print_path (visited)
        print ("Traversed:", total_nodes_traversed)
    else:
        print("No path has been found to your goal node.")
        sys.exit()

if __name__ == "__main__":
    main(sys.argv[1:])