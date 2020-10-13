#pragma once

#include <algorithm>
#include <list>
#include <unordered_map>

namespace LIRS {

	#define LIR 0
	#define R_HIR 1 // resident HIR
	#define NR_HIR 2 // non-resident HIR

	template <typename Key, typename Data>
	class cache {
	public:
		explicit cache(size_t size);
		size_t size() const;
		bool entry(Key key, const Data& data); // 1 if hit, 0 if miss
		void clear();

	private:
		struct node {
			node(Key key, int status, typename std::list<Data>::iterator cache_it);
			Key key_;
			int status_;
			typename std::list<Data>::iterator cache_it_;
		};

		typedef typename std::list<cache<Key, Data>::node>::iterator ListIt;
		typedef typename std::unordered_map<Key, ListIt>::iterator MapIt;

		void accessLIR(MapIt hash); // refers to hashmap_LIR_
		void accessResHIR(MapIt hash_Q); // refers to hashmap_R_HIR_Q_list<node>::iterator
		void accessNonResHIR(MapIt hash, const Data& data); // refers to hashmap_NR_HIR_
		void accessNew(Key key, const Data& data);
		void changeStatusQ_FrontResHIRtoNonResHIR();
		void changeStatusS_BottomLIRtoResHIR();
		void changeStatusSQ_ResHIRtoLIR(MapIt hash_Q, MapIt hash_S);
		void changeStatusS_NonResHIRtoLIR(MapIt hash, const Data& data);
		void pruneS_();

		std::unordered_map<Key, ListIt> hashmap_LIR_; // refers to S_
		std::unordered_map<Key, ListIt> hashmap_R_HIR_Q_; // refers to Q_
		std::unordered_map<Key, ListIt> hashmap_R_HIR_S_; // refers to S_
		std::unordered_map<Key, ListIt> hashmap_NR_HIR_; // refers to S_
		size_t HIRs_size_, LIRs_size_;
		std::list<cache<Key, Data>::node> S_, Q_;

		std::list<Data> cache_; // actual data storage aka cache
	};

	template <typename Key, typename Data>
	cache<Key, Data>::node::node(
		Key key, int status, typename std::list<Data>::iterator cache_it
	)
		: key_(key)
		, status_(status)
		, cache_it_(cache_it)
	{}

	template <typename Key, typename Data>
	cache<Key, Data>::cache(size_t size)
		: HIRs_size_((size >= 100)? (size / 100) : 1)
		, LIRs_size_((size >= 2)? (size - HIRs_size_) : 1)
	{}

	template <typename Key, typename Data>
	size_t cache<Key, Data>::size() const {
		return HIRs_size_ + LIRs_size_;
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::clear() {
		hashmap_LIR_.clear();
		hashmap_R_HIR_Q_.clear();
		hashmap_R_HIR_S_.clear();
		hashmap_NR_HIR_.clear();
		S_.clear();
		Q_.clear();
		cache_.clear();
	}

	template <typename Key, typename Data>
	bool cache<Key, Data>::entry(Key key, const Data& data) {
		auto if_hashed = hashmap_LIR_.find(key);
		if (if_hashed != hashmap_LIR_.end()) {
			accessLIR(if_hashed);
			return 1;
		}
		if_hashed = hashmap_R_HIR_Q_.find(key);
		if (if_hashed != hashmap_R_HIR_Q_.end()) {
			accessResHIR(if_hashed);
			return 1;
		}
		if_hashed = hashmap_NR_HIR_.find(key);
		if (if_hashed != hashmap_NR_HIR_.end()) {
			accessNonResHIR(if_hashed, data);
			return 0;
		} else {
			accessNew(key, data);
			return 0;
		}
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::accessLIR(MapIt hash) {
		S_.splice(S_.begin(), S_, hash->second);
		pruneS_();
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::accessResHIR(MapIt hash_Q) {
		auto if_hashed_S = hashmap_R_HIR_S_.find(hash_Q->first);

		if (if_hashed_S != hashmap_R_HIR_S_.end()) { // elem is in S_
			S_.splice(S_.begin(), S_, if_hashed_S->second);
			changeStatusSQ_ResHIRtoLIR(hash_Q, if_hashed_S);
			changeStatusS_BottomLIRtoResHIR();
			pruneS_();
		} else {
			S_.push_front(*(hash_Q->second));
			hashmap_R_HIR_S_.insert({hash_Q->first, S_.begin()});
			Q_.splice(Q_.end(), Q_, hash_Q->second);
		}
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::accessNonResHIR(MapIt hash, const Data& data) {
		changeStatusQ_FrontResHIRtoNonResHIR();
		S_.splice(S_.begin(), S_, hash->second);
		changeStatusS_NonResHIRtoLIR(hash, data);
		changeStatusS_BottomLIRtoResHIR();
		pruneS_();
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::accessNew(Key key, const Data& data) {
		cache_.push_front(data);
		if (S_.size() < LIRs_size_) {
			S_.push_front({key, LIR, cache_.begin()});
			hashmap_LIR_.insert({key, S_.begin()});
		} else {
			S_.push_front(node{key, R_HIR, cache_.begin()});
			hashmap_R_HIR_S_.insert({key, S_.begin()});
			if (Q_.size() == HIRs_size_) {
				changeStatusQ_FrontResHIRtoNonResHIR();
			}
			Q_.push_back(S_.front());
			hashmap_R_HIR_Q_.insert({key, prev(Q_.end())});
		}
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::changeStatusQ_FrontResHIRtoNonResHIR() {
		hashmap_R_HIR_Q_.erase((Q_.begin())->key_);
		auto hash_S = hashmap_R_HIR_S_.find((Q_.begin())->key_);
		if (hash_S != hashmap_R_HIR_S_.end()) {
			hashmap_NR_HIR_.insert(*hash_S);
			hashmap_R_HIR_S_.erase(hash_S);
		}
		cache_.erase((Q_.front()).cache_it_);
		Q_.pop_front();
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::changeStatusS_BottomLIRtoResHIR() {
		hashmap_LIR_.erase(prev(S_.end())->key_);
		prev(S_.end())->status_ = R_HIR;
		Q_.push_back(*prev(S_.end()));
		hashmap_R_HIR_Q_.insert({prev(Q_.end())->key_, prev(Q_.end())});
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::changeStatusSQ_ResHIRtoLIR(MapIt hash_Q, MapIt hash_S) {
		hash_S->second->status_ = LIR;
		hashmap_LIR_.insert(*hash_S);
		hashmap_R_HIR_S_.erase(hash_S);
		Q_.erase(hash_Q->second);
		hashmap_R_HIR_Q_.erase(hash_Q);
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::changeStatusS_NonResHIRtoLIR(MapIt hash, const Data& data) {
		cache_.push_front(data);
		hash->second->status_ = LIR;
		hash->second->cache_it_ = cache_.begin();
		hashmap_LIR_.insert(*hash);
		hashmap_NR_HIR_.erase(hash);
	}

	template <typename Key, typename Data>
	void cache<Key, Data>::pruneS_() {
		auto elem = find_if(
			S_.rbegin(), S_.rend(), \
			[](const node& elem) { return elem.status_ == LIR; }
		);
		for (auto it = S_.rbegin(); it != elem; ++it) {
			hashmap_R_HIR_S_.erase(it->key_);
			hashmap_NR_HIR_.erase(it->key_);
		}
		S_.erase(elem.base(), S_.end());
	}

	#undef LIR
	#undef R_HIR
	#undef NR_HIR

}
