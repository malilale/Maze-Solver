#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define SPACE ' '
#define PATH ' '
#define START 'b'
#define END 'c'
#define STAR '*'
#define APPLE 'o'

char** readMaze(const char[],int*,int*,int*,int*);
void printMaze(char **, int, int,int**,int*);
void DFS(int **, char **, int, int, int, int, int*);
void putRandomApples(char **, int, int);
int isVisited(int **, int, int);
void setVisited(int **, int, int);

int main(){
	int rows=0, cols=0, start_row, start_col, points=0, i;
	int **visited; //An array to check if the cells are visited before
	char **matrix;
	matrix = readMaze("maze.txt",&rows,&cols, &start_row, &start_col);  //read maze to matrix from file
	
	visited =(int**) malloc((rows-1)/2*sizeof(int*)); //the size of the visited matrix is (rows-1)/2  because it represents only free spaces only
	for(i =0;i<((rows-1)/2);i++){
		visited[i] =(int*) calloc((cols-1)/2,sizeof(int));
	}
	putRandomApples(matrix, rows, cols); //puts apples in maze randomly
	DFS(visited, matrix, rows, cols, start_row, start_col, &points); //DFS algorithm to find the path

	//free matrix
	for(i=0; i<rows; i++){
		free(matrix[i]); 
	}
	free(matrix);
	
	return 0;
}

char** readMaze(const char file_name[], int *rows, int *cols, int *start_row, int *start_col){
	char line[255];
	char **matrix;
	int i, j;
	FILE* fp = fopen(file_name, "r");
	if (fp==NULL){
		printf("File could not open! ");
		exit(1);
	}
	
	while(fgets(line,255,fp)!=NULL){ //read  lines
		(*rows)++; //update the number of rows
	}
	*cols = strlen(line); //get number of columns

	matrix =(char**) malloc((*rows)*sizeof(char*));
	for(i =0;i<*rows;i++){
		matrix[i] =(char*) malloc((*cols)*sizeof(char));
	}
	fseek(fp, 0, SEEK_SET); //go to the first line of the file
	i=0; j=0;
	
	for(i=0; i<*rows;i++){
		fgets(line,255,fp);
		for(j=0; j<*cols; j++){
			matrix[i][j] = line[j];
			if(matrix[i][j] == START){ //get the starting point of maze
				*start_row = i;
				*start_col = j;
			}
		}
	}
	fclose(fp); //close file
	return matrix;
}


void DFS(int **visited, char **matrix, int rows, int cols, int x, int y, int *points){
	static int found=0; //to check if the path is founded
	printMaze(matrix,rows,cols,visited, points);
	
	if( matrix[x][y] == END){ //if the current cell is the END (if the path is completed
		printMaze(matrix,rows,cols,visited, points);
		printf("\nFOUNDED! \n");
		found=1;
	}else if(matrix[x][y] != START){ //if the current cell is not the starting point
		if(matrix[x][y] == APPLE) //if there is an apple in the current cell
			*points += 10; 
		matrix[x][y] = STAR; //put a star to the cell to tshow the path
	}
		
	while(!found){ //while the path is not founded
		if(matrix[x][y+1]==PATH && !isVisited(visited,x,y+2)){ //if there is a path at the right and it is not visited yet
			setVisited(visited,x,y); //set the cell it is visited
			DFS(visited, matrix, rows, cols, x, y+2, points);
		}else if(visited, matrix[x-1][y]==PATH && !isVisited(visited,x-2,y)){ //if there is a path above and it is not visited yet
			setVisited(visited,x,y);
			DFS(visited, matrix, rows, cols, x-2, y, points);
		}else if(matrix[x][y-1]==PATH && !isVisited(visited,x,y-2)){ //if there is a path at the left and it is not visited yet
			setVisited(visited,x,y);
			DFS(visited, matrix, rows, cols, x, y-2, points);
		}else if(matrix[x+1][y]==PATH && !isVisited(visited,x+2,y)){ //if there is a path below and it is not visited yet
			setVisited(visited,x,y);
			DFS(visited, matrix, rows, cols, x+2, y, points);
		}else{ //if there is not any path that not visited (if it is a deadend)
			
			matrix[x][y] = SPACE; //to remove the star
			if(!isVisited(visited,x,y)){ //if the cell is visited for the first time 
				setVisited(visited,x,y);
				*points -= 5;
			}
			printMaze(matrix,rows,cols,visited, points);
			return;
		}
	}
}

void printMaze(char **matrix, int rows, int cols, int **visited, int *points){
	Sleep(100); //wait 0.1 seconds (100 miliseconds) before printing
	system("CLS");
	int i, j;
	
	for(i=0; i<rows;i++){
		for(j=0; j<cols; j++){
			printf("%c",matrix[i][j]);
		}
		printf("\n");
	}
	printf("PUAN: %d", *points);
}


void putRandomApples(char **matrix, int rows, int cols){
	int i, rnd_x, rnd_y, apples;
	srand(time(NULL));
	apples = rows*cols/100; //the nummber of apples
	for(i=0; i<apples; i++){
		rnd_x = rand()%(rows-1);
		if(rnd_x %2 == 0) //the numbers should be odd
			rnd_x++;
		rnd_y = rand()%(cols-1);
		if(rnd_y %2 == 0)
			rnd_y++;
		printf("%d %d\n", rnd_x, rnd_y);
		matrix[rnd_x][rnd_y] = APPLE;
	}
}

int isVisited(int **visited, int x, int y){
	return visited[(x-1)/2][(y-1)/2];
}

void setVisited(int **visited, int x, int y){
	visited[(x-1)/2][(y-1)/2] = 1;
}
