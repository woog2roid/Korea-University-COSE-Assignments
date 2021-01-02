#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <regex>
#include <vector>
using namespace std;

int main(void) {
	regex pattern("(\\s)(((98|99|00|01)(((0[13578]|1[02])(0[1-9]|[12]\\d|3[01]))|(0[469]|11)(0[1-9]|[12]\\d|30)))|(98|99|01)(02)(0[1-9]|1\\d|2[0-8])|(00)(02)(0[1-9]|1\\d|2[0-9]))-([1-4])(\\d{6})(\\s)|(\\s)([a-zA-Z0-9]+)@([a-zA-z]+)\\.ac\\.kr(\\s)");
	const string eof = "-1";
	string input;
	vector<string> list;

	printf("Enter the string. To quit, enter \"-1\":\n");

	getline(cin, input);
	while (input.compare(eof)) {
		sregex_iterator it(input.begin(), input.end(), pattern);
		sregex_iterator end;
		while (it != end)
		{
			smatch m = *it;
			regex mail("(\\s)([a-zA-Z0-9]+)@([a-zA-z]+)\\.ac\\.kr(\\s)");
			if (regex_match(m.str(0).c_str(), mail)) {
				regex mailPatt("@([a-zA-z])+\\.ac\\.kr");
				list.push_back(regex_replace(m.str(0).c_str(), mailPatt, "@XXXX.ac.kr"));
			}
			else {
				regex numPatt("-([1-4])\\d{6}");
				list.push_back(regex_replace(m.str(0).c_str(), numPatt, "-$1XXXXXX"));
			}
			++it;
		}
		getline(cin, input);
	}

	for (vector<string> ::iterator it = list.begin(); it != list.end(); ++it) {
		cout << *it << endl;
	}

	return 0;
}