#pragma once

#include <utility>
#include <vector>

#include "Beladys_algorithm.h"

std::vector<std::pair<int, char>> frameEntries();
template <typename Cache>
size_t processEntries(
	Cache cch, const std::vector<std::pair<int, char>>& entries
);
size_t processEntries(
	Beladys::cache<int, char> cch_B, \
	const std::vector<std::pair<int, char>>& entries
);



template <typename Cache>
size_t processEntries(
	Cache cch, const std::vector<std::pair<int, char>>& entries
) {
	size_t hits = 0;
	for (auto it = entries.begin(), ite = entries.end(); it != ite; ++it) {
		if (cch.entry(it->first, it->second)) {
			++hits;
		}
	}

	return hits;
}
