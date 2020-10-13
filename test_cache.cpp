#include <iostream>
#include <utility>
#include <vector>

#include <string.h>

#include "Beladys_algorithm.h"
#include "LIRS.h"
#include "io_handle.h"

using namespace std;

int processBeladys();
int processLIRS();

int main(int argc, char *argv[]) {
	if (argc == 1) {
		cout << "too few arguments\n";
		return 1;
	} else if (strcmp(argv[1], "Beladys") == 0) {
		processBeladys();
	} else if (strcmp(argv[1], "LIRS") == 0) {
		processLIRS();
	} else {
		cout << "unknown token\n";
		return 2;
	}

	return 0;
}

int processBeladys(){
	size_t cache_size = 0;
	cin >> cache_size;
	Beladys::cache<int, char> cch_B(cache_size);
		// key type is int, data type - char

	vector<pair<int, char>> entries = frameEntries();
	size_t hits_B = processEntries(move(cch_B), entries);

	cout << hits_B << '\n';

	return 0;
}

int processLIRS() {
	size_t cache_size = 0;
	cin >> cache_size;
	LIRS::cache<int, char> cch_L(cache_size);
		// key type is int, data type - char

	vector<pair<int, char>> entries = frameEntries();
	size_t hits_L = processEntries(move(cch_L), entries);

	cout << hits_L << '\n';

	return 0;
}
