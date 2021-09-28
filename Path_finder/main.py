
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
    #print ("Traversed_to_goal:", len(visited))

    def print_grid(grid):
        for line in grid:
            for i in line:
                print (i, end=',')
            print()





class PathPlanner:

    def print_info (result):

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


    def create_node_grid (filename, num_rows, num_cols):
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

        return node_grid, grid
    
    def initialize_node_grid (node_grid, num_rows, num_cols, search_type):
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
                
                if search_type == "BFS":
                    n.neighbors = [n.left, n.top, n.right, n.bottom]
                if (search_type =="DFS" or search_type == "astar" or search_type == "ALL" ):
                    n.neighbors = [n.bottom,n.right,n.top,n.left]

        
    def compute_dist_for_all (node_grid, goal_node,):
        for line in node_grid:
            for element in line:
        #prepare for A*
                list_of_cells = node_grid[element.i][element.j].neighbors
                for i in range(4):
                    loc = list_of_cells[i]
                    if loc != None and loc.value == 0:# cell exists has distance can be calculated
                        loc.distance_to_goal = math.sqrt( ((loc.i - goal_node.i)**2) + ((loc.j- goal_node.j)**2) )

                #rearrange the list of neighbors for the cell n to reflect the order in which to try neighboring cells
                #START STATE: # (bottom->right->top->left)                

                
        return node_grid

    def bfs(root, grid, node_grid, num_rows, num_cols,goal_node):
        found_path = False
        if root == goal_node:
            found_path = True

        queue = [root]
        traversed = 0
        cur_node = root

        while len(queue) > 0:
            if (found_path):
                break
            cur_node = queue.pop(0)
            cur_node.visited = 1
            node_grid[cur_node.i][cur_node.j] = cur_node
            if (cur_node.i == goal_node.i and cur_node.j == goal_node.j) == True:
                found_path= True
                break

            traversed += 1
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

            if (cur_node.i - 1) >= 0 :
                #print ("try to go UP given that we haven't visited the Top from the bottom node")
                if (grid[cur_node.i - 1][cur_node.j] == 0):  # free to go
                    cur_node.top = node(cur_node.i - 1, cur_node.j)
                    if (node_grid[cur_node.i-1][cur_node.j].visited == 0):
                        cur_node.top.visited = 1
                        cur_node.top.bottom = cur_node
                        node_grid[cur_node.i-1][cur_node.j] = cur_node.top
                        queue.append(cur_node.top)
                        cur_node.top.prev = cur_node
                        #print ("went UP")

            if (cur_node.j - 1 >= 0) :
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

    def dfs (cell, visited, goal_node,found):
        if cell:
            visited.append (cell)
            print ( (cell.i,cell.j),end ='')
            if (cell.i == goal_node.i and cell.j == goal_node.j ):
                found = True
                cur_path = len(visited)
                print (" ",cur_path )
                return True
            visited.append (cell)

            #neighbor_copy.pop(0)
            for i in range (4):
                if i == 0:
                    for neighbor in cell.neighbors:
                        if neighbor not in visited :
                                if (PathPlanner.dfs (neighbor, visited, goal_node, found)):
                                        return True            
            return False


    """
    
    """
    def deepening_dfs (node_grid, root, goal_node):
        start = node_grid[root.i][root.j]
        cur_depth = 1
        
        goal = False
        goal_i = goal_node.i; goal_j = goal_node.j
        if (start.i == goal_i and start.j == goal_j):
            goal = True
        
        list_of_neighbors_at_this_level = [start,goal_node]
        start.visited = 0
        start.prev = None
        nodes_traversed = 0

        while (1):  
            cur_depth+=1
            if goal==True:
                break
            while (len(list_of_neighbors_at_this_level)>0):
                individual = list_of_neighbors_at_this_level[0]
                individual.visited = 1
                nodes_traversed+=1
                
                #get their list of adjacent cells
                if (individual and individual.value ==0):
                    for i in individual.neighbors:
                        if i :
                            nodes_traversed+=1
                            if i.visited == 0 and i.value != 1:
                                list_of_neighbors_at_this_level.append(i)
                                i.visited = 1
                                i.prev = individual
                                if i.i == goal_i and i.j == goal_j:
                                    goal=True
                list_of_neighbors_at_this_level.remove(individual)#make sure only the new connections are in the list


        #prepare info for printing
        n = node_grid[goal_node.i][goal_node.j]
        route = []
        while (n ):
            route.append(n)
            n = n.prev
        route.reverse()

        return goal, route, nodes_traversed    
            

    """
    
    """
    def deepening_dfs_astar (node_grid,root, goal_node):
        start = node_grid[root.i][root.j]
        goal = False
        goal_i = goal_node.i; goal_j = goal_node.j
        if (start.i == goal_i and start.j == goal_j):
            goal = True
        
        list_of_neighbors_at_this_level = [start]
        nodes_traversed = 0
        cur_lowest_distance_to_goal = 99999

        node_grid = PathPlanner.compute_dist_for_all (node_grid, goal_node)

        while (1):  
            if goal==True:
                break
            #for individual in list_of_neighbors_at_this_level:
            while (len(list_of_neighbors_at_this_level)>0):
                if goal==True:
                    break
                
                individual = list_of_neighbors_at_this_level[0]
                individual.visited = 1
                nodes_traversed+=1

                #get their list of adjacent cells
                if (individual and individual.value ==0):
                     #ADD SOME STUFF TO COME OUT OF IT IF GOING NOWHERE
                    not_better=0
                    for s in individual.neighbors:
                        if (s and s.distance_to_goal > cur_lowest_distance_to_goal ):
                            not_better+=1

                    if not_better==4:
                        individual.value = 1
                        cur_lowest_distance_to_goal = individual.prev.distance_to_goal
                        if individual.prev:
                            cur_lowest_distance_to_goal = individual.prev.distance_to_goal
                            list_of_neighbors_at_this_level.remove(individual)
                            list_of_neighbors_at_this_level.insert(0, individual.prev)
                            individual = individual.prev                            


                    for i in individual.neighbors: 
                        if i :
                            if i.visited == 0 and i.value != 1 :
                                i.visited = 1
                                i.prev = individual

                                if i.distance_to_goal >= cur_lowest_distance_to_goal:
                                    list_of_neighbors_at_this_level.append(i)
                                elif i.distance_to_goal <= cur_lowest_distance_to_goal :
                                    #print ("(Improving distance)",i.i,i.j, i.distance_to_goal)
                                    cur_lowest_distance_to_goal = i.distance_to_goal
                                    list_of_neighbors_at_this_level.insert(0,i)
                                    if i.i == goal_i and i.j == goal_j:
                                        goal=True  
                list_of_neighbors_at_this_level.remove(individual)#make sure only the new connections are in the list
        
        #prepare info for printing
        n = node_grid[goal_node.i][goal_node.j]
        route = []
        while (n ):
            route.append(n)
            n = n.prev
        route.reverse()

        return goal, route, nodes_traversed        




"""
    
"""
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
    if (s[0].isdigit() == False or int(s[0]) >= num_rows or (s[1].isdigit() == False or int(s[1]) >= num_cols)):
        print("Start coordinate is invalid or is beyond the size of the grid")
        sys.exit()
    root = node(int(s[0],), int(s[1]))

    s = goal_node.split(',')
    if (s[0].isdigit() == False or int(s[0]) >= num_rows or (s[1].isdigit() == False or int(s[1]) >= num_cols)):
        print("Goal coordinate is invalid or is beyond the size of the grid")
        sys.exit()
    goal_node = node(int(s[0],), int(s[1]))



    #Start working with a class
    node_grid, grid = PathPlanner.create_node_grid (filename, num_rows, num_cols)# create a grid
    # Consider a case when a goal_node is an obstacle. The program then gracefully quits
    if (grid[goal_node.i][goal_node.j]) == 1:
        print("The goal coordinate provided is an obstacle and cannot be reached")
        sys.exit()



    #PathPlanner.print_grid(grid)

    #initialize all of the grid nodes to know about their neighbors
    PathPlanner.initialize_node_grid (node_grid, num_rows, num_cols, search_type)
    if search_type == "BFS":
        result = PathPlanner.bfs(root, grid, node_grid, num_rows, num_cols,goal_node)
    elif search_type == "DFS":
        result = PathPlanner.deepening_dfs(node_grid, root, goal_node)        
    elif search_type == "astar": #g(n) = 1 for all, so I will discard it
        result = PathPlanner.deepening_dfs_astar(node_grid,root,goal_node)
 


    elif search_type == "ALL":
        #first one is ready to go
        result_bfs = PathPlanner.bfs(root, grid, node_grid, num_rows, num_cols,goal_node)

        #second need a newly made grid with no modifications
        node_grid, grid = PathPlanner.create_node_grid (filename, num_rows, num_cols)# create a grid
        PathPlanner.initialize_node_grid (node_grid, num_rows, num_cols, search_type)#initialize all of the grid nodes to know about their neighbors
        result_dfs = PathPlanner.deepening_dfs(node_grid, root, goal_node)

        node_grid, grid = PathPlanner.create_node_grid (filename, num_rows, num_cols)# create a grid
        PathPlanner.initialize_node_grid (node_grid, num_rows, num_cols, search_type)
        result_astar = PathPlanner.deepening_dfs_astar(node_grid, root,goal_node)
        results = [result_bfs, result_dfs, result_astar]

        for r in results:
            PathPlanner.print_info (r)
    else:
        print ("Unknown search type has been specified")
        sys.exit()

    if search_type != "ALL":
        PathPlanner.print_info(result)
        print ()

if __name__ == "__main__":
    main(sys.argv[1:])