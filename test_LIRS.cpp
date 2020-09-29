#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include "Beladys_algorithm.h"
#include "LIRS.h"
#include "tests.h"

using namespace std;

template <typename AccessPattern>
void logHits(AccessPattern pttrn, string f_name);

int main() {
	logHits(burstRead, "burst_read.txt");
	logHits(loopRead, "loop_read.txt");

	return 0;
}

template <typename AccessPattern>
void logHits(AccessPattern pttrn, string f_name) {
	vector<pair<int, int>> access_pattern(pttrn());
	ofstream os(f_name);

	for (size_t i = 5; i <= 300; i += 5) {
		Beladys::cache<int, int> cch_B(i);
		LIRS::cache<int, char> cch_L(i);
		os << i << '\t';

		vector<int> v(cch_B.entry_trace(access_pattern));
		os << count(v.begin(), v.end(), 1) << '\t';

		size_t hits_L = 0;
		for (auto it = access_pattern.begin(); it != access_pattern.end(); ++it) {
			if (cch_L.entry(it->first, it->second) == 1) {
				++hits_L;
			}
		}
		os << hits_L << '\n';
	}

	os.close();
}
