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
		std::vector<bool> entry_trace(
			const std::vector<std::pair<Key, Data>>& data
		); // whole history of entries
		void clear();

	private:
		typedef typename std::vector<std::pair<Key, Data>> DataVect;
		typedef typename std::vector<std::pair<Key, Data>>::const_iterator \
										 DataVectConstIt;
		typedef typename std::list<Data>::iterator ListIt;

		void missNotFullCache(DataVectConstIt elem);
		void missFullCache(DataVectConstIt elem, DataVectConstIt end);
		Key findElemEvict(DataVectConstIt begin, DataVectConstIt end) const;

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

		for (
			auto elem = data.begin(), elem_e = data.end(); elem != elem_e ; ++elem
		) {
			if (hashmap_.find(elem->first) != hashmap_.end()) {
				hits.push_back(1);
			} else {
				if (cache_.size() < size_) {
					missNotFullCache(elem);
					hits.push_back(0);
				} else {
					missFullCache(elem, data.end());
					hits.push_back(0);
				}
			}
		}
		return hits;
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::missNotFullCache(DataVectConstIt elem) {
		cache_.push_front(elem->second);
		hashmap_.insert({elem->first, cache_.begin()});
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::missFullCache(
		DataVectConstIt elem, DataVectConstIt end
	) {
		hashmap_.insert({elem->first, cache_.end()});
			// don't refer to actual data
		Key key_evict = findElemEvict(next(elem), end);

		if (key_evict != elem->first) {
			cache_.erase(hashmap_.find(key_evict)->second);
			cache_.push_front(elem->second);
			(hashmap_.find(elem->first))->second = cache_.begin();
		}
		hashmap_.erase(key_evict);
	}

	template <typename Key, typename Data>
	Key cache<Key, Data>::findElemEvict(
		DataVectConstIt begin, DataVectConstIt end
	) const {
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
