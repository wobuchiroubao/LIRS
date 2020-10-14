#include <iostream>

#include "io_handle.h"

using namespace std;

vector<pair<int, char>> frameEntries() {
	size_t entry_trace_size = 0;
	cin >> entry_trace_size;

	vector<pair<int, char>> entries; // key type is int, data type - char
	entries.reserve(entry_trace_size);
	int entry = 0;
	for (size_t i = 0; i < entry_trace_size; ++i) {
		cin >> entry;
		entries.push_back({entry, '\0'});
	}

	return entries;
}

size_t processEntries(
	Beladys::cache<int, char> cch_B, const vector<pair<int, char>>& entries
) {
	size_t hits_B = 0;
	vector<bool> v_hits_B = cch_B.entry_trace(entries);
	for (auto it = v_hits_B.begin(), ite = v_hits_B.end(); it != ite; ++it) {
		if (*it) {
			++hits_B;
		}
	}

	return hits_B;
}
