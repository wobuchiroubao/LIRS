#pragma once

#include <list>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Beladys {

	template <typename Key, typename Data>
	class cache {
	public:
		explicit cache(size_t size);
		size_t size() const;
		std::vector<bool> entry_trace(const std::vector<std::pair<Key, Data>>& data);
																									// whole history of entries
		void clear();

	private:
		typedef typename std::vector<std::pair<Key, Data>> DataVect;
		typedef typename std::vector<std::pair<Key, Data>>::const_iterator DataVectIt;
		typedef typename std::list<Data>::iterator ListIt;

		Key findElemEvict(DataVectIt begin, DataVectIt end);

		std::unordered_map<Key, ListIt> hashmap_;
		size_t size_;
		std::list<Data> cache_; // actual data storage aka cache
	};

	template <typename Key, typename Data>
	cache<Key, Data>::cache(size_t size)
		: size_(size)
	{}

	template <typename Key, typename Data>
	size_t cache<Key, Data>::size() const {
		return size_;
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::clear() {
		hashmap_.clear();
		cache_.clear();
	}

	template <typename Key, typename Data>
	std::vector<bool> cache<Key, Data>::entry_trace(const DataVect& data) {
		std::vector<bool> hits;
		hits.reserve(data.size());

		for (size_t i = 0; i < data.size() ; ++i) {
			if (hashmap_.find(data[i].first) != hashmap_.end()) {
				hits.push_back(1);
			} else {
				if (cache_.size() < size_) {
					cache_.push_front(data[i].second);
					hashmap_.insert({data[i].first, cache_.begin()});
					hits.push_back(0);
				} else {
					hashmap_.insert({data[i].first, cache_.end()}); // don't refer to actual data
					Key key_evict = findElemEvict(data.begin() + i + 1, data.end());

					if (key_evict != data[i].first) {
						cache_.erase(hashmap_.find(key_evict)->second);
						cache_.push_front(data[i].second);
						(hashmap_.find(data[i].first))->second = cache_.begin();
					}
					hashmap_.erase(key_evict);
					hits.push_back(0);
				}
			}
		}
		return hits;
	}

	template <typename Key, typename Data>
	Key cache<Key, Data>::findElemEvict(DataVectIt begin, DataVectIt end) {
		std::unordered_map<Key, ListIt> search_map(hashmap_);
		for (auto it = begin; it < end && search_map.size() > 1 ; ++it) {
			auto if_in = search_map.find(it->first);
			if (if_in != search_map.end()) {
				search_map.erase(if_in);
			}
		}

		return search_map.begin()->first;
	}

}
