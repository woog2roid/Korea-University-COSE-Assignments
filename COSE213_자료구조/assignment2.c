#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 15000

typedef struct {
	short int N;
	short int M;
}Move;
Move move[8];

typedef struct {
	int N;
	int M;
	int dir;
}Data;

struct stack {
	Data data;
	struct stack* link;
};
typedef struct stack Stack;
Stack* top;
void push(int N, int M, int dir);
Data pop();
bool isEmpty();


int chk[MAX][MAX];
int map[MAX][MAX];
int START_N, START_M, EXIT_N, EXIT_M;

bool solve();

int main(int argc, char* argv[]) {
	FILE* inputFile = fopen(argv[1], "r");
	if (inputFile == NULL) {
		printf("Input File Error!");
		return 0;
	}

	FILE* outputFile = fopen(argv[2], "w");
	if (outputFile == NULL) {
		printf("Output File Error!");
		return 0;
	}

	move[0].N = -1;	move[0].M = 0;	move[1].N = -1;	move[1].M = 1;
	move[2].N = 0;	move[2].M = 1;	move[3].N = 1;	move[3].M = 1;
	move[4].N = 1;	move[4].M = 0;	move[5].N = 1;	move[5].M = -1;
	move[6].N = 0;	move[6].M = -1;	move[7].N = -1;	move[7].M = -1;
	for (int i = 0; i < MAX; i++)	for (int j = 0; j < MAX; j++)	map[i][j] = 1;

	fseek(inputFile, 0, SEEK_END);
	int size = ftell(inputFile);
	fseek(inputFile, 0, 0);

	fscanf(inputFile, "%d %d %d %d ", &START_N, &START_M, &EXIT_N, &EXIT_M);

	char tmp[MAX - 2];
	int before = ftell(inputFile);
	fgets(tmp, MAX - 2, inputFile);
	int after = ftell(inputFile);
	int length = (after - before) / 2;
	fseek(inputFile, before - after, SEEK_CUR);

	int line = 1;
	while (ftell(inputFile) != size) {
		for (int i = 1; i < length + 1; i++) {
			fscanf(inputFile, "%d", &map[line][i]);
		}
		line++;
	}

	fprintf(outputFile, "%d %d\n", START_N, START_M);
	if (solve()) {
		int arr[MAX][2];
		int cnt = 0;
		while (!isEmpty()) {
			Data tmp = pop();
			arr[cnt][0] = tmp.N;
			arr[cnt][1] = tmp.M;
			cnt++;
		}
		for (int i = cnt - 2; i >= 0; i--) {
			fprintf(outputFile, "%d %d\n", arr[i][0], arr[i][1]);
		}
	}

	fclose(inputFile);
	fclose(outputFile);
	return 0;
}

bool solve() {
	push(START_N, START_M, 0);
	chk[START_N][START_M] = 1;
	while (!isEmpty()) {
		Data data = pop();
		int N = data.N, M = data.M, dir = data.dir;
		while (dir < 8) {
			int nextN = N + move[dir].N;
			int nextM = M + move[dir].M;
			if (nextN == EXIT_N && nextM == EXIT_M) {
				push(N, M, dir);
				push(nextN, nextM, dir);
				return true;
			}
			else if (!map[nextN][nextM] && !chk[nextN][nextM]) {
				chk[nextN][nextM] = 1;
				push(N, M, dir + 1);
				N = nextN, M = nextM, dir = 0;
			}
			else dir++;
		}
	}
	return false;
}

void push(int N, int M, int dir) {
	Stack* tmp;
	tmp = (Stack*)malloc(sizeof(Stack));
	tmp->data.N = N;
	tmp->data.M = M;
	tmp->data.dir = dir;
	tmp->link = top;
	top = tmp;
}

Data pop() {
	Stack* tmp = top;
	Data data;
	data = tmp->data;
	top = tmp->link;
	free(tmp);
	return data;
}

bool isEmpty() {
	return top == NULL;
}