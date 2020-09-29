#include "tests.h"

#include <cstdlib>
#include <time.h>

using namespace std;

vector<pair<int, int>> burstRead() {
	vector<pair<int, int>> v;
	v.reserve(6'600);
	srand(time(0));

	for (int i = 0; i < 2'000; ++i) {
		v.push_back({rand() % 10, 0});
	}

	for (int i = 100; i < 300; ++i) {
		v.push_back({i, 0});
	}

	for (int i = 0; i < 2'000; ++i) {
		v.push_back({rand() % 10, 0});
	}

	for (int i = 100; i < 300; ++i) {
		v.push_back({i, 0});
	}

	for (int i = 100; i < 300; ++i) {
		v.push_back({i, 0});
	}

	for (int i = 0; i < 2'000; ++i) {
		v.push_back({rand() % 10, 0});
	}

	return v;
}


vector<pair<int, int>> loopRead() {
	vector<pair<int, int>> v;
	v.reserve(6'000);

	for (int i = 0; i < 30; ++i) {
		for (int j = 0; j < 200; ++j) {
			v.push_back({j, 0});
		}
	}

	return v;
}
