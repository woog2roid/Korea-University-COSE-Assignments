/*
	INPUT
	The first line is given number of nodes, n (1  <= n <= 100) and the second line is given number of edges, m (1 <= m <= 100,000).

	And the following line is given from the third line to the m+2 line.
	First, the starting index of the edge is given. The edge's information consists of the start index a, arrival index b, and the required cost c.
	The start node and the arrival node are not the same. The cost is less than or equal to 100,000 natural numbers.

	The edge connecting the start node and the arrival node may not be one.

	OUTPUT
	N lines shall be printed. The jth printed on the i-line is the minimum cost required to go from node i to j.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <limits.h>
#define MAX 100
#define INF INT_MAX

int map[MAX + 1][MAX + 1], node, edge;
void floyd();

int main(void) {
	//input
	scanf("%d %d ", &node, &edge);
	for (int i = 1; i <= node; i++)		for (int j = 1; j <= node; j++)	map[i][j] = INF;
	for (int i = 0; i < edge; i++) {
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c);
		if(map[a][b] > c)	map[a][b] = c;
	}

	//compute
	floyd();

	//output
	printf("THE OUTPUT IS :\n");
	for (int i = 1; i <= node; i++) {
		for (int j = 1; j <= node; j++) {
			if (map[i][j] == INF) printf("X ");
			else printf("%d ", map[i][j]);
		}
		printf("\n");
	}
}

void floyd() {
	for (int via = 1; via <= node; via++) {
		for (int from = 1; from <= node; from++) {
			if (map[from][via] == INF)	continue;

			for (int to = 1; to <= node; to++) {
				if (map[via][to] == INF)	continue;
				if (from == to)	continue;

				if (map[from][to] > map[from][via] + map[via][to])	map[from][to] = map[from][via] + map[via][to];
			}
		}
	}
}