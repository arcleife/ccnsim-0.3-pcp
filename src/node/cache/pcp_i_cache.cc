/*
 * pcp_i_cache.cc
 *
 *  Created on: Apr 17, 2017
 *      Author: arcleife
 */

#include <iostream>
#include "pcp_i_cache.h"

#include "error_handling.h"

Register_Class(pcp_i_cache);

void pcp_i_cache::data_store(chunk_t elem, int hops, long f){
	// if data is already there, do nothing
	if (data_lookup(elem, hops, f))
		return;

	// if eviction is needed
	if (full()){
		if (cache.begin()->second.size()>1){
			// find random chunk with smallest H
			// cache.begin()->second[pos]
			int pos = intrand(cache.begin()->second.size());
			cache_index.erase(getChunkGivenPos(cache.begin()->second, pos));
			cache.begin()->second.remove(getChunkGivenPos(cache.begin()->second, pos)); // smallest H has > 1 chunks, erase random chunk from Q[H]
		} else {
			map<int, chunk_queue>::iterator it = cache.begin();
			cache_index.erase(*it->second.begin());
			cache.erase(it); // smallest H has only 1 chunk
		}
	}

	// add the data into cache
	int dp = getAncestorPar("dp");
	cache[dp * hops].push_back(elem);
	cache_index[elem] = {dp * hops, 0, 0};
	//print_cache();
}

bool pcp_i_cache::data_lookup(chunk_t elem, int hops, long f){
	unordered_map<chunk_t, chunk_info>::iterator it = cache_index.find(elem);

	if (it == cache_index.end()){
		// data is not found on the cache, return false
		return false;
	}

	// data is found, it = chunk_stat, H = it->second
	// check if Q[H] has more than one elements
	if (cache[it->second.h].size() > 1){
		// remove the head
		cache[it->second.h].remove(elem); // remove from the cache
	} else {
		// Q[H] has only one element, remove the Q[H]
		map<int, chunk_queue>::iterator hq = cache.find(it->second.h);
		cache.erase(hq); // remove from the cache
	}

	// check if f is already in s1 (bit for s1 is the same as f)
	// s1 == s1 & f
	if (it->second.s1 == (it->second.s1 & f)){
		it->second.h -= hops; // update the h on the chunk_info
		if (it->second.s2 != (it->second.s2 & f)){
			it->second.s2 = it->second.s2 | f; // update the s2p
		}
	} else {
		it->second.h += hops; // update the h on the chunk_info
		it->second.s1 = it->second.s1 | f; // update the s1p
	}

	// update the H on the chunk_index and the cache

	return true;
}

bool pcp_i_cache::fake_lookup(chunk_t elem){
	unordered_map<chunk_t, chunk_info>::iterator it = cache_index.find(elem);

	if (it == cache_index.end()){
		// data is not found on the cache, return false
		return false;
	}

	// data is found
	return true;
}

bool pcp_i_cache::full(){
	return (cache_index.size() == get_size());
}

chunk_t pcp_i_cache::getChunkGivenPos(chunk_queue& queue, int pos){
	if (queue.size() > (unsigned int) pos)
	{
	    chunk_queue::iterator it = queue.begin();
	    std::advance(it, pos);
	    // 'it' points to the element at index pos
	    return *it;
	}
	return 0;
}
