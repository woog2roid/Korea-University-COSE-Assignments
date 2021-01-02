#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <regex>
using namespace std;

int main(void) {
	int cntPhone = 0, cntCard = 0;
	
	regex phone("\\s010-\\d{4}-\\d{4}\\s");
	regex card("\\s\\d{4}-\\d{4}-\\d{4}-\\d{4}\\s");

	const string eof = "-1";
	string input;

	printf("Enter the string. To quit, enter \"-1\":\n");
	getline(cin, input);
	while (input.compare(eof)) {
		auto startPhone = sregex_iterator(input.begin(), input.end(), phone);
		auto end = sregex_iterator();
		while (startPhone != end) {
			cntPhone++;
			++startPhone;
		}
		auto startCard = sregex_iterator(input.begin(), input.end(), card);
		while (startCard != end) {
			cntCard++;
			++startCard;
		}
		getline(cin, input);
	}

	printf("%d\n%d\n", cntPhone, cntCard);
	return 0;
}