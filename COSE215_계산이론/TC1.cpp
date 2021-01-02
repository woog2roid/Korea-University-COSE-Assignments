#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <regex>
#include <string>
using namespace std;

typedef struct {
	regex re;
	char ch;
}list;
list arr[56];

void initialize(void);

int main(int argc, char* argv[]) {
	FILE* inputFile = fopen(argv[1], "r");

	printf("Enter the string. To quit, enter \"-1\":\n");
	initialize();

	fseek(inputFile, 0, SEEK_END);
	int end = ftell(inputFile);
	fseek(inputFile, 0, SEEK_SET);

	char tmp[10];
	fscanf(inputFile, "%s", tmp);

	while (ftell(inputFile) != end) {
		string str(tmp);
		for (int i = 0; i < 56; i++) {
			if (regex_match(str, arr[i].re)) {
				printf("%c", arr[i].ch);
				break;
			}
		}
		fscanf(inputFile, "%s", tmp);
	}
	return 0;
}

void initialize(void) {
	arr[0].re = ("[.][-]");	arr[0].ch = 'A';
	arr[1].re = ("[-][.]{3}");	arr[1].ch = 'B';
	arr[2].re = ("[-][.][-][.]");	arr[2].ch = 'C';
	arr[3].re = ("[-][.]{2}");	arr[3].ch = 'D';
	arr[4].re = ("[.]");	arr[4].ch = 'E';
	arr[5].re = ("[.]{2}[-][.]");	arr[5].ch = 'F';
	arr[6].re = ("[-]{2}[.]");	arr[6].ch = 'G';
	arr[7].re = ("[.]{4}");	arr[7].ch = 'H';
	arr[8].re = ("[.]{2}");	arr[8].ch = 'I';
	arr[9].re = ("[.][-]{3}");	arr[9].ch = 'J';
	arr[10].re = ("[-][.][-]");	arr[10].ch = 'K';
	arr[11].re = ("[.][-][.]{2}");	arr[11].ch = 'L';
	arr[12].re = ("[-]{2}");	arr[12].ch = 'M';
	arr[13].re = ("[-][.]");	arr[13].ch = 'N';
	arr[14].re = ("[-]{3}");	arr[14].ch = 'O';
	arr[15].re = ("[.][-]{2}[.]");	arr[15].ch = 'P';
	arr[16].re = ("[-]{2}[.][-]");	arr[16].ch = 'Q';
	arr[17].re = ("[.][-][.]");	arr[17].ch = 'R';
	arr[18].re = ("[.]{3}");	arr[18].ch = 'S';
	arr[19].re = ("[-]");	arr[19].ch = 'T';
	arr[20].re = ("[.]{2}[-]");	arr[20].ch = 'U';
	arr[21].re = ("[.]{3}[-]");	arr[21].ch = 'V';
	arr[22].re = ("[.][-]{2}");	arr[22].ch = 'W';
	arr[23].re = ("[-][.]{2}[-]");	arr[23].ch = 'X';
	arr[24].re = ("[-][.][-]{2}");	arr[24].ch = 'Y';
	arr[25].re = ("[-]{2}[.]{2}");	arr[25].ch = 'Z';

	arr[26].re = ("[-]{5}");	arr[26].ch = '0';
	arr[27].re = ("[.][-]{4}");	arr[27].ch = '1';
	arr[28].re = ("[.]{2}[-]{3}");	arr[28].ch = '2';
	arr[29].re = ("[.]{3}[-]{2}");	arr[29].ch = '3';
	arr[30].re = ("[.]{4}[-]");	arr[30].ch = '4';
	arr[31].re = ("[.]{5}");	arr[31].ch = '5';
	arr[32].re = ("[-][.]{4}");	arr[32].ch = '6';
	arr[33].re = ("[-]{2}[.]{3}");	arr[33].ch = '7';
	arr[34].re = ("[-]{3}[.]{2}");	arr[34].ch = '8';
	arr[35].re = ("[-]{4}[.]");	arr[35].ch = '9';

	arr[36].re = ("[.][-][.][-][.][-]");	arr[36].ch = '.';
	arr[37].re = ("[-]{2}[.]{2}[-]{2}");	arr[37].ch = ',';
	arr[38].re = ("[.]{2}[-]{2}[.]{2}");	arr[38].ch = '?';
	arr[39].re = ("[.][-]{4}[.]");	arr[39].ch = '\'';
	arr[40].re = ("[-][.][-][.][-]{2}");	arr[40].ch = '!';
	arr[41].re = ("[-][.]{2}[-][.]");	arr[41].ch = '/';
	arr[42].re = ("[-][.][-]{2}[.]");	arr[42].ch = '(';
	arr[43].re = ("[-][.][-]{2}[.][-]");	arr[43].ch = ')';
	arr[44].re = ("[.][-][.]{3}");	arr[44].ch = '&';
	arr[45].re = ("[-]{3}[.]{3}");	arr[45].ch = ':';
	arr[46].re = ("[-][.][-][.][-][.]");	arr[46].ch = ';';
	arr[47].re = ("[-][.]{3}[-]");	arr[47].ch = '=';
	arr[48].re = ("[.][-][.][-][.]");	arr[48].ch = '+';
	arr[49].re = ("[-][.]{4}[-]");	arr[49].ch = '-';
	arr[50].re = ("[.]{2}[-]{2}[.][-]");	arr[50].ch = '_';
	arr[51].re = ("[.][-][.]{2}[-][.]");	arr[51].ch = '"';
	arr[52].re = ("[.]{3}[-][.]{2}[-]");	arr[52].ch = '$';
	arr[53].re = ("[.][-]{2}[.][-][.]");	arr[53].ch = '@';
	arr[54].re = ("[.]{2}[-][.][-]");	arr[54].ch = 'вп';
	arr[55].re = ("[-]{2}[.]{3}[-]");	arr[55].ch = 'во';
}