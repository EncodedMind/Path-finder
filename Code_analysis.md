# Solving the Problem

"Zoomba" is a robot that tries to reach its target in the grid
The task involves reading data, validating it, and finding the shortest "path" from one cell of a grid (matrix) to another, with obstacles in between.

## Reading Data

The task requires reading an integer for the grid size, and four integers for the coordinates of the "source" (zoomba) and the "target". Then, we validate all values to ensure they are within the grid, dynamically create the grid, read it while validating the correct number of values, confirm that the coordinates of the zoomba and target are not on obstacles, find the shortest path, and finally free the memory allocated for the grid.

### Summary:

```c
int main(){
    
    if((scanf("%d", &N)) != 1) fprintf(stderr, "Error: Invalid map size was given.\n"), exit(1);

    int zx, zy, tx, ty;
    if((scanf("%d %d %d %d", &zx, &zy, &tx, &ty)) != 4) fprintf(stderr, "Error: Invalid coordinates given.\n"), exit(1);
    if(zx < 0 || zx >= N) exit(1);
    if(zy < 0 || zy >= N) exit(1);
    if(tx < 0 || tx >= N) exit(1);
    if(ty < 0 || ty >= N) exit(1);

    // char grid[N][N];
    char** grid = malloc(N * sizeof(char*));
    if(!grid) fprintf(stderr, "Error: Memory allocation failed.\n"), exit(1);
    for(int i = 0; i < N; ++i){
        grid[i] = malloc(N * sizeof(char));
        if(!grid[i]) fprintf(stderr, "Error: Memory allocation failed.\n"), exit(1);
    }
    
    for(int i = 0; i < N; ++i){
        for(int j = 0; j < N; ++j){
            if((scanf(" %c", &grid[i][j])) != 1) fprintf(stderr, "Error: Invalid map data given.\n"), exit(1);
        }
    }
    
    if(grid[zx][zy] == '1' || grid[tx][ty] == '1') fprintf(stderr, "Error: Coordinates out of bounds.\n"), exit(1);
    
    // Something for the path

    for(int i = 0; i < N; ++i) free(grid[i]);
    free(grid);

    return 0;
}
```

Next, we will build a function `find_path`, which will accept the grid, the source, and the target, and attempt to find the shortest path between them. The function will return 0 or 1, depending on whether it found a path. If it returns 0, we make sure to output "0". Otherwise, before returning, we will print the path using a helper function. The `main` function will be updated as follows:

```c
QItem source = {zx, zy, -1, -1};
QItem target = {tx, ty, -1, -1};
    
if (!find_path(grid, source, target)) printf("0\n");
```

## What is `QItem`?

The implementation of the shortest path algorithm relies on an algorithm ideal for trees and graphs where all edges are equal (unweighted), called [BFS (Breadth-First Search)](https://en.wikipedia.org/wiki/Breadth-first_search).

*Note: Applications like Google Maps, which perform this process continuously, rely on algorithms such as A\* or Dijkstra, as they are efficient for weighted graphs, where each path has multiple parameters to consider (distance, traffic, accidents, etc.). However, for a grid, we only care about moving from one cell to the next. Thus, it is an unweighted graph, and BFS is preferred.*

This algorithm uses an array to track whether a cell has been visited and a queue to manage the sequence of cells to examine. In the queue, we store a grid element along with its row, column, the move we made to reach it (U-Up, D-Down, R-Right, L-Left), and the previous cell we came from. To avoid using four parallel queues (one for each element), we define a `QItem` structure for the queue elements:

```c
typedef struct QItem{
    int row;
    int col;
    int cameFrom;
    char move;
} QItem;
```

## The `find_path` Function

Initially, we define the queue as an array of type `QItem`, of size N*N (it may need to hold all grid elements), and its pointers, `front` and `rear`.

We insert the source into the queue as its first element and mark the grid position as visited by setting its value to 1.

The logic involves moving up, down, left, or right, depending on whether a cell has been visited or not. Pre-existing obstacles are treated as already visited cells. If a move is valid, we add it to the queue and mark the cell to prevent infinite loops. For each cell added, we repeat the process until we reach the target. At that point, BFS stops. Naturally, we remove each cell from the queue as we process it. 

Thus, as long as the queue is not empty (`front < rear`):

We dequeue the first element, check the valid moves, and for each valid move, add it to the queue and mark the grid. For example, the "Down" move is described as follows:

```c
// moving down
if (p.row+1 < N && grid[p.row+1][p.col] == '0') {
    q[rear++] = (QItem){p.row+1, p.col, front-1, 'D'};
    grid[p.row+1][p.col] = '1';
}
```

- Valid move: We are not at the grid's edge (`p.row + 1 < N`) and the cell below is not marked (`grid[p.row+1][p.col] == '0`).
- Add to the queue a `QItem` with the row, column, previous element, and direction: `q[rear++] = (QItem){p.row+1, p.col, front-1, 'D'};`
- Mark the cell: `grid[p.row+1][p.col] = '1';`

Similarly, for the other directions:

```c
// moving up
if (p.row-1 >= 0 && grid[p.row-1][p.col] == '0') {
    q[rear++] = (QItem){p.row-1, p.col, front-1, 'U'};
    grid[p.row-1][p.col] = '1';        
}

// moving down
if (p.row+1 < N && grid[p.row+1][p.col] == '0') {
    q[rear++] = (QItem){p.row+1, p.col, front-1, 'D'};
    grid[p.row+1][p.col] = '1';
}

// moving left
if (p.col-1 >= 0 && grid[p.row][p.col-1] == '0') {
    q[rear++] = (QItem){p.row, p.col-1,front-1, 'L'};
    grid[p.row][p.col-1] = '1';
}

// moving right
if (p.col+1 < N && grid[p.row][p.col+1] == '0') {
    q[rear++] = (QItem){p.row, p.col+1, front-1, 'R'};
    grid[p.row][p.col+1] = '1';
}
```

However, if at some point the examined element is the target (`p.row == target.row && p.col == target.col`), we must build the path used to reach it, free the queue, and return 1. Assume a function `reconstruct_path`, which builds the path and requires the queue and the previous element of the queue (i.e., the element from which we reached the target). Thus, we add another check:

```c
// Target found
if (p.row == target.row && p.col == target.col) {
    reconstruct_path(q, front-1);
    free(q);
    return 1;
}
```

If the queue is emptied without finding the target, we free the queue memory and return 0.

Here is the final function:

```c
int find_path(char **grid, QItem source, QItem target) {

    QItem *q = malloc(sizeof(QItem) * N*N);
    if(!q) fprintf(stderr, "Error: Memory allocation failed.\n"), exit(1);
    
    int front = 0, rear = 0;
    q[rear++] = source;
    grid[source.row][source.col] = '1';
    
    while (front < rear) {
        QItem p = q[front++];

        // Destination found;
        if (p.row==target.row && p.col==target.col) {
            reconstruct_path(q, front-1);
            free(q);
            return 1;
        }

        // moving up
        if (p.row-1 >= 0 && grid[p.row-1][p.col] == '0') {
            q[rear++] = (QItem){p.row-1, p.col, front-1, 'U'};
            grid[p.row-1][p.col] = '1';
        }

        // moving down
        if (p.row+1 < N && grid[p.row+1][p.col] == '0') {
            q[rear++] = (QItem){p.row+1, p.col, front-1, 'D'};
            grid[p.row+1][p.col] = '1';
        }

        // moving left
        if (p.col-1 >= 0 && grid[p.row][p.col-1] == '0') {
            q[rear++] = (QItem){p.row, p.col-1, front-1, 'L'};
            grid[p.row][p.col-1] = '1';
        }

        // moving right
        if (p.col+1 < N && grid[p.row][p.col+1] == '0') {
            q[rear++] = (QItem){p.row, p.col+1, front-1, 'R'};
            grid[p.row][p.col+1] = '1';
        }
    }

    free(q);
    return 0;
}
```
- Important Note
In the queue implementation, when we process the first element, it is removed by advancing the front pointer. Although this element technically remains in memory, it is treated as outside the queue’s bounds. However, this implementation takes advantage of the queue design by tracing backward from the target to the source using the cameFrom pointer.

## The reconstruct_path Function
Starting from the target and working back to the source, the moves are stored in a temporary buffer. This buffer is printed in reverse order to display the path correctly.

```c
// char buffer[N*N];
char* buffer = malloc(N*N * sizeof(char));
if(!buffer) fprintf(stderr, "Error: Memory allocation failed.\n"), exit(1);

int count = 0;
```
For each step, the move is added to the buffer, and the index is updated to trace back to the previous step. When index becomes -1, the trace is complete:

```c
while(index >= 0){
    buffer[count++] = q[index].move;
    index = q[index].cameFrom;
}
```
Finally, the path is printed in reverse (excluding the last character, which represents the source) and the memory is freed:

```c
for(int i=count-2; i>=0; --i){
    printf("%c", buffer[i]);
}
printf("\n");

free(buffer);
```

Full Implementation of reconstruct_path

```c
void reconstruct_path(QItem *q, int index) {
    
    // char buffer[N*N];
    char* buffer = malloc(N*N * sizeof(char));
    if(!buffer) fprintf(stderr, "Error: Memory allocation failed.\n"), exit(1);

    int count = 0;
    
    while(index >= 0){
        buffer[count++] = q[index].move;
        index = q[index].cameFrom;
    }
    
    for(int i=count-2; i>=0; --i){
        printf("%c", buffer[i]);
    }
    printf("\n");

    free(buffer);
}
```
