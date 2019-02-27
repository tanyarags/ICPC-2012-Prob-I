Problem Statement:
Given a grid with mirrors, if laser beam enters from upper left hand corner, check if it can exit from lower right corner by adding an additional mirror.
Also, return the number of positions where such mirror can be added.

Intuition (some facts about grid and mirrors):
  Laser can only travel along x or y axis.
  There cannot be a loop condition. (Beam getting trapped)
  Laser beam can traverse only a single path from starting point and ending point.
  Path from starting point and path taken by retracing beam from end point must not meet.
  Mirror must change path of beam by 90 degrees.
  If path starting from start point and path from end point can meet by placing a mirror, they both intersect orthogonally.
  All such points where they intersect, will always be a solution.
  A mirror can have only 4 neighbours.

Areas to focus on (reduce computations)
1. Making maze should be a light task i.e.. adding new mirror to maze must be inexpensive task.
2. Traversal in maze must be inexpensive, since this will be done for N mirrors, any cost addition rises N times.
3. Finding intersection points between paths starting from start point and end point must also be inexpensive.

Algorithm:
Keeping in view above focus areas, this solution has been structured in 3 parts.
1. Maze construction (O(NlogN):
   To add new mirror to maze, we must place it at particular (row,column). 
   Although maintaining a maze grid array provided O(1) insertion(for single mirror), it required memory of O(N^2). Adding it to O(N^2) task.
   Hence, pointers are used. Access and insertion to these pointers must also be inexpensive, AVL trees provide just the solution.
   Insertion and searching are O(log(n)) and space complexity is O(N). Adding it to O(Nlog(N)).

2. Maze Traversal:
   Any mirror when added to grid can affect atmost 4 neighbours. Hence, these neighbours were added dynamically while adding a new mirror to maze.
   Hence, maintaining pointers to neighbours, while constructing the maze, helps traversal from a mirror to next mirror/end in O(1) complexity.
   This helps us maintain overall complexity of O(N).

3. Finding Intersection points.
   This has to be performed only between horizontal line segments from path 1 and vertical line segments from path 2.
   Checking all possible combinations these line segments would cost us O(N^2) and would also undo above maintained complexity of O(NlogN).
   AVLTrees have been used to store these line segments. All horizontal lines are compared to vertical lines only when a vertical line segment is encountered.
   Since finding range of points is O(logN) because of AVL tree structure, for O(N) horizontal line segment makes it O(NlogN) times expensive.