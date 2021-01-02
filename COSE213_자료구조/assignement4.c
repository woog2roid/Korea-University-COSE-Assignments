#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_ELEMENTS 200
#define HEAP_FULL(n) (n==MAX_ELEMENTS-1)
#define HEAP_EMPTY(n) (!n)
typedef struct {
	int from;
	int to;
	int weight;
}element;
element heap[MAX_ELEMENTS];
int n = 0;
void push(element item, int* n);
element pop(int* n);

int parent[MAX_ELEMENTS];
int count[MAX_ELEMENTS];
int simpleFind(int i);
void weightedUnion(int i, int j);

int main(int argc, char* argv[]) {
	FILE* inputFile = fopen(argv[1], "r");
	FILE* outputFile = fopen(argv[2], "w");

	int node, edge;
	fscanf(inputFile, "%d", &node);
	fscanf(inputFile, "%d", &edge);
	for (int i = 0; i < edge; i++) {
		element tmp;
		fscanf(inputFile, "%d", &tmp.from); fscanf(inputFile, "%d", &tmp.to); fscanf(inputFile, "%d", &tmp.weight);
		push(tmp, &n);
	}
	for (int i = 0; i < node; i++) { parent[i] = -1; count[i] = -1; }

	for (int i = 0; i < edge; i++) {
		element tmp = pop(&n);
		int from = tmp.from;
		int to = tmp.to;
		int weight = tmp.weight;

		if (simpleFind(from) != simpleFind(to)) {
			weightedUnion(simpleFind(from), simpleFind(to));
			fprintf(outputFile, "%d %d\n", from, to);
		}
	}
	return 0;
}

void push(element item, int* n) {
	int i;
	if (HEAP_FULL(*n)) {
		fprintf(stderr, "The Heap is full. \n");
		exit(EXIT_FAILURE);
	}
	i = ++(*n);
	while ((i != 1) && (item.weight < heap[i / 2].weight)) {
		heap[i] = heap[i / 2];
		i /= 2;
	}
	heap[i] = item;
}

element pop(int* n) {
	int parent, child;
	element item, temp;
	if (HEAP_EMPTY(*n)) {
		fprintf(stderr, "The heap is empty\n");
		exit(EXIT_FAILURE);
	}
	item = heap[1];
	temp = heap[(*n)--];
	parent = 1;
	child = 2;
	while (child <= *n) {
		if ((child < *n) && (heap[child].weight > heap[child + 1].weight)) child++;
		if (temp.weight <= heap[child].weight)	break;
		heap[parent] = heap[child];
		parent = child;
		child *= 2;
	}
	heap[parent] = temp;
	return item;
}

int simpleFind(int i) {
	for (; parent[i] >= 0; i = parent[i]);
	return i;
}

void weightedUnion(int i, int j) {
	int temp = count[i] + count[j];
	if (count[i] > count[j]) {
		parent[i] = j;
		count[j] = temp;
	}
	else {
		parent[j] = i;
		count[i] = temp;
	}
}