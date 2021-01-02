#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <regex>
#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {
	ifstream inputFile(argv[1]);
	if (!inputFile.is_open()) {
		printf("input file error");
		return 0;
	}

	int cntMail = 0, cntNumber = 0;
	regex number("(\\s)(((98|99|00|01)(((0[13578]|1[02])(0[1-9]|[12]\\d|3[01]))|(0[469]|11)(0[1-9]|[12]\\d|30)))|(98|99|01)(02)(0[1-9]|1\\d|2[0-8])|(00)(02)(0[1-9]|1\\d|2[0-9]))-([1-4])(\\d{6})(\\s)");
	regex mail("(\\s)([a-zA-Z0-9]+)@([a-zA-z]+)\\.ac\\.kr(\\s)");
	const string eof = "-1";
	string input;

	while (!inputFile.eof()) {
		getline(inputFile, input);
		auto startNumber = sregex_iterator(input.begin(), input.end(), number);
		auto end = sregex_iterator();
		while (startNumber != end) {
			cntNumber++;
			++startNumber;
		}
		auto startMail = sregex_iterator(input.begin(), input.end(), mail);
		while (startMail != end) {
			cntMail++;
			++startMail;
		}
	}

	printf("%d\n%d\n", cntNumber, cntMail);
	return 0;
}