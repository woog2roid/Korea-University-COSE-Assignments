#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define compareINT(x,y) (((x)<(y))?-1:((x)==(y))? 0:1)
#define MALLOC(ptr,size) if ((ptr = malloc(size)) == NULL) {printf("MALLOC Error!!\n");exit(EXIT_FAILURE);}

#define MAX_SIZE 50
#define MAX_INDEX 64

typedef enum {head, entry} tagfield;
typedef struct matrixNode* matrixPointer;
typedef struct{
	int row;
	int col;
	int value;
}entryNode;
int compare(entryNode tmp1,entryNode tmp2);
struct matrixNode{
	matrixPointer down;
	matrixPointer right;
	tagfield tag;
	union {
		matrixPointer next;
		entryNode entry;
	}u;
};
matrixPointer hdnode[MAX_SIZE];
matrixPointer matrix[MAX_INDEX + 1];

matrixPointer newNode();
matrixPointer mread(void);
void mwrite(matrixPointer node);
void merase(matrixPointer* node);
matrixPointer madd(matrixPointer node1, matrixPointer node2);
matrixPointer mmult(matrixPointer node1, matrixPointer node2);
matrixPointer mtranspose(matrixPointer node);

bool isEmpty();

int main(void) {
	int input, indexList[3];
	while (1) {
		printf("1. Read\n2. Write\n3. Erase\n4. Add\n5. Multiply\n6. Transpose\n7. Exit\n");
		scanf(" %d", &input);

		switch (input) {
		case 1:
			printf("Save index: ");
			while (true) {
				scanf(" %d", &indexList[0]);
				if (MAX_INDEX < indexList[0] || indexList[0] < 1)
					printf("index must be 1 to %d. Enter the correct index: ", MAX_INDEX);
				else
					break;
			}
			matrix[indexList[0]] = mread();
			break;

		case 2:
			if (isEmpty()) {
				printf("There is no saved matrix.\n");
				break;
			}

			printf("Print index: ");
			while (true) {
				scanf(" %d", &indexList[0]);
				if (MAX_INDEX < indexList[0] || indexList[0] < 1)
					printf("index must be 1 to %d. Enter the correct index: ", MAX_INDEX);
				else if(matrix[indexList[0]] == NULL)
					printf("This index is empty matrix. Enter the correct index: ");
				else
					break;
			}
			mwrite(matrix[indexList[0]]);
			break;

		case 3:
			if (isEmpty()) {
				printf("There is no saved matrix.\n");
				break;
			}

			printf("Erase index: ");
			while (true) {
				scanf(" %d", &indexList[0]);
				if (MAX_INDEX < indexList[0] || indexList[0] < 1)
					printf("index must be 1 to %d. Enter the correct index: ", MAX_INDEX);
				else if (matrix[indexList[0]] == NULL)
					printf("This index is already empty matrix. Enter the correct index: ");
				else
					break;
			}
			merase(&matrix[indexList[0]]);
			break;

		case 4:
			if (isEmpty()) {
				printf("There is no saved matrix.\n");
				break;
			}

			printf("First, second, save index: ");
			while (true) {
				scanf(" %d %d %d", &indexList[0], &indexList[1], &indexList[2]);
				if (MAX_INDEX < indexList[0] || indexList[0] < 1 || MAX_INDEX < indexList[1] || indexList[1] < 1 || MAX_INDEX < indexList[2] || indexList[2] < 1)
					printf("index must be 1 to %d. Enter the correct index: ", MAX_INDEX);
				else if (matrix[indexList[0]] == NULL || matrix[indexList[1]] == NULL)
					printf("This index is empty matrix. Enter the correct index: ");
				else if (matrix[indexList[0]]->u.entry.col != matrix[indexList[1]]->u.entry.col || matrix[indexList[0]]->u.entry.row != matrix[indexList[1]]->u.entry.row)
					printf("The sizes of the two matrices are different. Enter the correct indices: ");
				else 
					break;
			}
			matrix[indexList[2]] = madd(matrix[indexList[0]], matrix[indexList[1]]);
			break;
			
		case 5:
			if (isEmpty()) {
				printf("There is no saved matrix.\n");
				break;
			}

			printf("First, second, save index: ");
			while (true) {
				scanf(" %d %d %d", &indexList[0], &indexList[1], &indexList[2]);
				if (MAX_INDEX < indexList[0] || indexList[0] < 1 || MAX_INDEX < indexList[1] || indexList[1] < 1 || MAX_INDEX < indexList[2] || indexList[2] < 1)
					printf("index must be 1 to %d. Enter the correct index: ", MAX_INDEX);
				else if (matrix[indexList[0]] == NULL || matrix[indexList[1]] == NULL)
					printf("This index is empty matrix. Enter the correct index: ");
				else if (matrix[indexList[0]]->u.entry.col != matrix[indexList[1]]->u.entry.row)
					printf("Can not multiply the matrices. Enter the correct index: ");
				else
					break;
			}
			matrix[indexList[2]] = mmult(matrix[indexList[0]], matrix[indexList[1]]);
			break;

		case 6:
			if (isEmpty()) {
				printf("There is no saved matrix.\n");
				break;
			}

			printf("Transpose index, save index: ");
			while (true) {
				scanf(" %d %d", &indexList[0], &indexList[1]);
				if (MAX_INDEX < indexList[0] || indexList[0] < 1 || MAX_INDEX < indexList[1] || indexList[1] < 1)
					printf("index must be 1 to %d. Enter the correct index: ", MAX_INDEX);
				else if (matrix[indexList[0]] == NULL)
					printf("This index is empty matrix. Enter the correct index: ");
				else
					break;
			}
			matrix[indexList[1]] = mtranspose(matrix[indexList[0]]);
			break;

		case 7:
			return 0;

		default:
			printf("Input Error! Enter the Integer 1 to 7.\n");
			break;
		}

	}
}

matrixPointer mread(void) {
	/*read in a matrix and set up its linked representation.
	An auxiliary global array hdnode is used.*/
	matrixPointer tmp = newNode();
	int numRows, numCols, numTerms, numHeads, i;
	int row, col, value, currentRow;
	matrixPointer temp, last, node;

	printf("Enter the number of rows, columns and number of nonzero terms: ");
	scanf("%d%d%d", &numRows, &numCols, &numTerms);
	numHeads = (numCols > numRows) ? numCols : numRows;
	/*set up header node for the list of header nodes*/
	node = newNode(); node->tag = entry;
	node->u.entry.row = numRows;
	node->u.entry.col = numCols;
	node->u.entry.value = numTerms;

	if (!numHeads)	node->right = node;
	else {
		for (i = 0; i < numHeads; i++) {
			temp = newNode();
			hdnode[i] = temp;
			hdnode[i]->tag = head;
			hdnode[i]->right = temp;
			hdnode[i]->u.next = temp;
		}
		currentRow = 0;

		last = hdnode[0]; /*last node in current row*/
		for (i = 0; i < numTerms; i++) {
			printf("Enter row, column and value: ");
			scanf(" %d %d %d", &row, &col, &value);
			if (row > currentRow) {/*close current row*/
				last->right = hdnode[currentRow];
				currentRow = row;
				last = hdnode[row];
			}
			MALLOC(temp, sizeof(*temp));
			temp->tag = entry;
			temp->u.entry.row = row;
			temp->u.entry.col = col;
			temp->u.entry.value = value;
			last->right = temp; /*link into row list*/
			last = temp;
			/* link into column list*/
			hdnode[col]->u.next->down = temp;
			hdnode[col]->u.next = temp;
		}
		/*close last row*/
		last->right = hdnode[currentRow];
		/*close all column lists*/
		for (i = 0; i < numCols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		/*link all header nodes together*/
		for (i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = node;
		node->right = hdnode[0];
	}
	return node;
}

void mwrite(matrixPointer node) {
	/* print out the matrix in row major form */
	int i;
	matrixPointer temp, head = node->right;
	/* matrix dimensions */
	printf("numRows = %d, numCols = %d \n", node->u.entry.row, node->u.entry.col);
	printf("The matrix by <row, column, and value>: \n");
	for (i = 0; i < node->u.entry.row; i++) {
		/*print out the entries in each row*/
		for (temp = head->right; temp != head; temp = temp->right) {
			printf("              <%7d,%7d,%7d> \n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
		}
		head = head->u.next; //next row
	}
}

void merase(matrixPointer* node) {
	/*erase the matrix, return the nodes to the heap*/
	matrixPointer x, y, head = (*node)->right;
	int i;
	/*free the entry and header nodes by row*/
	for (i = 0; i < (*node)->u.entry.row; i++) {
		y = head->right;
		while (y != head) {
			x = y; y = y->right; free(x);
		}
		x = head; head = head->u.next; free(x);
	}
	/*free remaining header nodes*/
	y = head;
	while (y != *node) {
		x = y; y = y->u.next; free(x);
	}
	free(*node); *node = NULL;
}

matrixPointer madd(matrixPointer node1, matrixPointer node2) {
	matrixPointer head1 = node1->right, head2 = node2->right;

	matrixPointer resNode = newNode(), last, temp;
	int numRows = node1->u.entry.row, numCols = node1->u.entry.col;
	int numHeads = (numCols > numRows) ? numCols : numRows;
	resNode->tag = entry;	resNode->u.entry.row = numRows;	resNode->u.entry.col = numCols;	resNode->u.entry.value = 0;

	if (!numHeads)	resNode->right = resNode;
	else {
		for (int i = 0; i < numHeads; i++) {
			temp = newNode();
			hdnode[i] = temp;
			hdnode[i]->tag = head;
			hdnode[i]->right = temp;
			hdnode[i]->u.next = temp;
		}
		last = hdnode[0];
		int row = 0, col = 0, value = 0;

		while (head1 != node1 && head2 != node2) {
			matrixPointer tmp1 = head1->right, tmp2 = head2->right;
			while (tmp1 != head1 && tmp2 != head2) {
				switch (compare(tmp1->u.entry, tmp2->u.entry)) {
				case -1:
					row = tmp1->u.entry.row, col = tmp1->u.entry.col, value = tmp1->u.entry.value;

					MALLOC(temp, sizeof(*temp));
					temp->tag = entry;
					temp->u.entry.row = row;
					temp->u.entry.col = col;
					temp->u.entry.value = value;

					last->right = temp;
					last = temp;
					hdnode[col]->u.next->down = temp;
					hdnode[col]->u.next = temp;

					tmp1 = tmp1->right;
					resNode->u.entry.value++;
					break;

				case 0:
					row = tmp1->u.entry.row, col = tmp1->u.entry.col, value = tmp1->u.entry.value + tmp2->u.entry.value;
					
					MALLOC(temp, sizeof(*temp));
					temp->tag = entry;
					temp->u.entry.row = row;
					temp->u.entry.col = col;
					temp->u.entry.value = value;
					
					last->right = temp;
					last = temp;
					hdnode[col]->u.next->down = temp;
					hdnode[col]->u.next = temp;

					tmp1 = tmp1->right;
					tmp2 = tmp2->right;
					resNode->u.entry.value++;
					break;

				case 1:
					row = head2->u.entry.row, col = head2->u.entry.col, value = head2->u.entry.value;

					MALLOC(temp, sizeof(*temp));
					temp->tag = entry;
					temp->u.entry.row = row;
					temp->u.entry.col = col;
					temp->u.entry.value = value;

					last->right = temp;
					last = temp;
					hdnode[col]->u.next->down = temp;
					hdnode[col]->u.next = temp;

					tmp2 = tmp2->right;
					resNode->u.entry.value++;
					break;
				}
			}
			for (; tmp1 != head1; tmp1 = tmp1->right) {
				MALLOC(temp, sizeof(*temp));
				temp->tag = entry;
				temp->u.entry.row = tmp1->u.entry.row;
				temp->u.entry.col = tmp1->u.entry.col;
				temp->u.entry.value = tmp1->u.entry.value;

				last->right = temp;
				last = temp;

				hdnode[col]->u.next->down = temp;
				hdnode[col]->u.next = temp;

				resNode->u.entry.value++;
			}
			for (; tmp2 != head2; tmp2 = tmp2->right) {
				MALLOC(temp, sizeof(*temp));
				temp->tag = entry;
				temp->u.entry.row = tmp2->u.entry.row;
				temp->u.entry.col = tmp2->u.entry.col;
				temp->u.entry.value = tmp2->u.entry.value;

				last->right = temp;
				last = temp;

				hdnode[col]->u.next->down = temp;
				hdnode[col]->u.next = temp;

				resNode->u.entry.value++;
			}
			head1 = head1->u.next;
			head2 = head2->u.next;
			last->right = hdnode[row];
			last = hdnode[++row];
		}
		
		for (int i = 0; i < numCols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		for (int i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = resNode;
		resNode->right = hdnode[0];
	}
	return resNode;
}

matrixPointer mmult(matrixPointer node1, matrixPointer node2) {
	matrixPointer head1 = node1->right, head2;

	matrixPointer resNode = newNode(), last, temp;
	int numRows = node1->u.entry.row, numCols = node1->u.entry.col;
	int numHeads = (numCols > numRows) ? numCols : numRows;
	resNode->tag = entry;	resNode->u.entry.row = numRows;	resNode->u.entry.col = numCols;	resNode->u.entry.value = 0;

	if (!numHeads)	resNode->right = resNode;
	else {
		for (int i = 0; i < numHeads; i++) {
			temp = newNode();
			hdnode[i] = temp;
			hdnode[i]->tag = head;
			hdnode[i]->right = temp;
			hdnode[i]->u.next = temp;
		}

		last = hdnode[0];

		for (int i = 0; i < node1->u.entry.row; i++) {
			head2 = node2->right;
			for (int j = 0; j < node2->u.entry.col; j++) {
				matrixPointer tmp1 = head1->right, tmp2 = head2->down;

				MALLOC(temp, sizeof(*temp));
				temp->tag = entry;
				temp->u.entry.value = 0;

				while (tmp1 != head1 && tmp2 != head2) {
					switch (compareINT(tmp1->u.entry.col, tmp2->u.entry.row)) {
					case -1:
						tmp1 = tmp1->right;	break;
					case 0:
						temp->u.entry.row = tmp1->u.entry.row;
						temp->u.entry.col = tmp2->u.entry.col;
						temp->u.entry.value += tmp1->u.entry.value * tmp2->u.entry.value;
						tmp1 = tmp1->right;
						tmp2 = tmp2->right;
						break;
					case 1:
						tmp2 = tmp2->down;	break;
					}
				}

				if (temp->u.entry.value == 0)	free(temp);
				else {
					last->right = temp;
					last = temp;
					hdnode[temp->u.entry.col]->u.next->down = temp;
					hdnode[temp->u.entry.col]->u.next = temp;
				}
				head2 = head2->u.next;
			}
			last->right = hdnode[i];
			last = hdnode[i + 1];
			head1 = head1->u.next;
		}
		
		for (int i = 0; i < numCols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		for (int i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = resNode;
		resNode->right = hdnode[0];
	}
	return resNode;
}

matrixPointer mtranspose(matrixPointer transNode) {
	matrixPointer transHeadNode = transNode->right;
	int numTerms = 0;

	matrixPointer resNode = newNode(), last, temp;
	int numRows = transNode->u.entry.col, numCols = transNode->u.entry.row, row, col, value;
	int numHeads = (numCols > numRows) ? numCols : numRows;
	resNode->tag = entry;	resNode->u.entry.row = numRows;	resNode->u.entry.col = numCols; resNode->u.entry.value = transNode->u.entry.value;
	
	if (!numHeads)	resNode->right = resNode;
	else {
		for (int i = 0; i < numHeads; i++) {
			temp = newNode();
			hdnode[i] = temp;
			hdnode[i]->tag = head;
			hdnode[i]->right = temp;
			hdnode[i]->u.next = temp;
		}

		row = 0, col = 0, value = 0;
		last = hdnode[0]; /*last node in current row*/
		for (int i = 0; i < transNode->u.entry.col; i++) {
			for (matrixPointer tmp = transHeadNode->down; tmp != transHeadNode; tmp = tmp->down) {
				row = tmp->u.entry.col, col = tmp->u.entry.row, value = tmp->u.entry.value;
				MALLOC(temp, sizeof(*temp));
				temp->tag = entry;
				temp->u.entry.row = row;
				temp->u.entry.col = col;
				temp->u.entry.value = value;
				last->right = temp;
				last = temp;
				hdnode[col]->u.next->down = temp;
				hdnode[col]->u.next = temp;
			}
			transHeadNode = transHeadNode->u.next;
			last->right = hdnode[row];
			last = hdnode[++row];
		}
		for (int i = 0; i < numCols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		for (int i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = resNode;
		resNode->right = hdnode[0];
	}
	return resNode;
}

matrixPointer newNode() {
	matrixPointer tmp;
	MALLOC(tmp, sizeof(*tmp));
	return tmp;
}

bool isEmpty() {
	for (int i = 0; i <= MAX_INDEX; i++) {
		if (matrix[i] != NULL) return false;
	}
	return true;
}

int compare(entryNode tmp1, entryNode tmp2) {
	if (tmp1.row < tmp2.row)	return -1;
	if (tmp1.row == tmp2.row) {
		if (tmp1.col < tmp2.col)	return -1;
		if (tmp1.col == tmp2.col)	return 0;
		if (tmp1.col > tmp2.col)	return 1;
	}
	if (tmp1.row > tmp2.row)	return 1;
}