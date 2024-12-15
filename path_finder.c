//Αναλυτική επεξήγηση του παρακάτω κώδικα υπάρχει στο αρχείο README.md
#include <stdio.h>
#include <stdlib.h>

int N;

typedef struct QItem{ //Η δομή για τα στοιχεία της ουράς
    int row;
    int col;
    int cameFrom;
    char move;
} QItem;


void reconstruct_path(QItem *q, int index){ //Κατασκευή και εκτύπωση του μονοπατιού
    
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

int find_path(char **grid, QItem source, QItem target){  //Εύρεση συντομότερου μονοπατιού

    QItem *q = malloc(sizeof(QItem) * N*N);
    if(!q) fprintf(stderr, "Error: Memory allocation failed.\n"), exit(1);
    
    int front=0, rear=0;
    q[rear++] = source;
    grid[source.row][source.col] = '1';
    
    while(front < rear){
        QItem p = q[front++];

        // Target found
        if(p.row==target.row && p.col==target.col){
            reconstruct_path(q, front-1);
            free(q);
            return 1;
        }

        // moving up
        if(p.row-1 >= 0 && grid[p.row-1][p.col] == '0'){
            q[rear++] = (QItem){p.row-1, p.col, front-1, 'U'};
            grid[p.row-1][p.col] = '1';
        
        }

        // moving down
        if(p.row+1 < N && grid[p.row+1][p.col] == '0'){
            q[rear++] = (QItem){p.row+1, p.col, front-1, 'D'};
            grid[p.row+1][p.col] = '1';
        }

        // moving left
        if(p.col-1 >= 0 && grid[p.row][p.col-1] == '0'){
            q[rear++] = (QItem){p.row, p.col-1,front-1, 'L'};
            grid[p.row][p.col-1] = '1';
        }

        // moving right
        if(p.col+1 < N && grid[p.row][p.col+1] == '0'){
            q[rear++] = (QItem){p.row, p.col+1, front-1, 'R'};
            grid[p.row][p.col+1] = '1';
        }
    }

    free(q);
    return 0;
}

int main(){
    
    if((scanf("%d", &N))!=1) fprintf(stderr, "Error: Invalid map size was given.\n"), exit(1);

    int zx, zy, tx, ty;
    if((scanf("%d %d %d %d", &zx, &zy, &tx, &ty))!=4) fprintf(stderr, "Error: Invalid coordinates given.\n"), exit(1);
    if(zx<0 || zx>=N) exit(1);
    if(zy<0 || zy>=N) exit(1);
    if(tx<0 || tx>=N) exit(1);
    if(ty<0 || ty>=N) exit(1);

    // char grid[N][N];
    char** grid = malloc(N * sizeof(char*));
    if(!grid) fprintf(stderr, "Error: Memory allocation failed.\n"), exit(1);
    for(int i=0; i<N; ++i){
        grid[i] = malloc(N * sizeof(char));
        if(!grid[i]) fprintf(stderr, "Error: Memory allocation failed.\n"), exit(1);
    }
    
    for(int i=0; i<N; ++i){
        for(int j=0; j<N; ++j){
            if((scanf(" %c", &grid[i][j]))!=1) fprintf(stderr, "Error: Invalid map data given.\n"), exit(1);
        }
    }
    
    if(grid[zx][zy]=='1' || grid[tx][ty] == '1') fprintf(stderr, "Error: Coordinates out of bounds.\n"), exit(1);
    
    QItem source = {zx, zy, -1, -1};
    QItem target = {tx, ty, -1, -1};
    
    if(!find_path(grid, source, target)) printf("0\n");

    for(int i=0; i<N; ++i) free(grid[i]);
    free(grid);

    return 0;
}
