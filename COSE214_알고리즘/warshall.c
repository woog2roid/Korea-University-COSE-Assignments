/*
INPUT
The first line is given number of nodes, n(1 <= n <= 100) and the second line is given number of edges, m(1 <= m <= 100, 000).

And the following line is given from the third line to the m + 2 line.
First, the starting index of the edge is given. The edge's information consists of the start index a, arrival index b, and the label c.
The start node and the arrival node are not the same.The label is 0 or 1.

The edge connecting the start node and the arrival node may not be one.

OUTPUT
N lines shall be printed. Print 1 on ith line, jth index if there is path from node i to j. Otherwise, print 0 on that position.
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#define MAX 100

int map[MAX + 1][MAX + 1], node, edge;
void warshall();
int main(void) {
	//input
	scanf("%d %d ", &node, &edge);
	for (int i = 0; i < edge; i++) {
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c);

		if (map[a][b] == 1)	continue;
		else map[a][b] = c;
	}

	//compute
	warshall();

	//output
	printf("THE OUTPUT IS :\n");
	for (int i = 1; i <= node; i++) {
		for (int j = 1; j <= node; j++) {
			printf("%d ", map[i][j]);
		}
		printf("\n");
	}
}

void warshall() {
	for (int via = 1; via <= node; via++) {
		for (int from = 1; from <= node; from++) {
			for (int to = 1; to <= node; to++) {
				map[from][to] = map[from][to] || (map[from][via] && map[via][to]);
			}
		}
	}
}