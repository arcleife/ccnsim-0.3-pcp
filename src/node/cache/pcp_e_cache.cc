/*
 * pcp_e_cache.cc
 *
 *  Created on: Apr 17, 2017
 *      Author: arcleife
 */

#include <iostream>
#include "pcp_e_cache.h"

#include "error_handling.h"

Register_Class(pcp_e_cache);

void pcp_e_cache::data_store(chunk_t elem, int hops, long f){
	// if data is already there, do nothing
	if (data_lookup(elem, hops, f))
		return;

	// if eviction is needed
	if (full()){
		L = cache.begin()->first;
		if (cache.begin()->second.size()>1){
			chunk_queue::iterator it = cache.begin()->second.begin();
			cache_index.erase(*it);
			cache.begin()->second.erase(it); // smallest H has > 1 chunks, erase the head
		} else {
			map<int, chunk_queue>::iterator it = cache.begin();
			cache_index.erase(*it->second.begin());
			cache.erase(it); // smallest H has only 1 chunk
		}
	}

	// add the data into cache
	cache[L + hops].push_back(elem);
	cache_index[elem] = L + hops;
}

bool pcp_e_cache::data_lookup(chunk_t elem, int hops, long f){
	unordered_map<chunk_t, int>::iterator it = cache_index.find(elem);

	if (it == cache_index.end()){
		// data is not found on the cache, return false
		return false;
	}

	// data is found, it = chunk_stat, H = it->second.h
	// check if Q[H] has more than one elements
	if (cache[it->second].size() > 1){
		// remove the head
		cache[it->second].remove(elem); // remove from the cache
	} else {
		// Q[H] has only one element, remove the Q[H]
		map<int, chunk_queue>::iterator hq = cache.find(it->second);
		cache.erase(hq); // remove from the cache
	}

	// update the data H
	it->second = L + hops; // update h_elem = cp + L
	cache[it->second].push_back(elem);
	cache_index[elem] = it->second;
	return true;
}

bool pcp_e_cache::fake_lookup(chunk_t elem){
	unordered_map<chunk_t, int>::iterator it = cache_index.find(elem);

	if (it == cache_index.end()){
		// data is not found on the cache, return false
		return false;
	}

	// data is found
	return true;
}

bool pcp_e_cache::full(){
	return (cache_index.size() == get_size());
}

void pcp_e_cache::print_cache(){
	cout << "---------------\n";
	cout << "node ";
	cout << getIndex();
	cout << " (";
	cout <<	cache_index.size();
	cout << "/";
	cout << get_size();
	cout << ")\n";
	cout << "L = ";
	cout << L << endl;
	cout << "index";
	for (unordered_map<chunk_t, int>::iterator it = cache_index.begin(); it != cache_index.end();it++){
		cout << "(chunk ";
		cout << it->first;
		cout << ", H ";
		cout << it->second;
		cout << ") ";
	}
	cout << endl;
	for (map<int, chunk_queue>::iterator it = cache.begin(); it != cache.end(); it++){
		cout << "H ";
		cout << it->first;
		cout << " | ";
		for (chunk_queue::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++){
			cout << *it2;
			cout << " ";
		}
		cout << endl;
	}
	cout << "---------------\n";
}
