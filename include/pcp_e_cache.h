/*
 * pcp_e_cache.h
 *
 *  Created on: Apr 17, 2017
 *      Author: arcleife
 */

#ifndef PCP_E_CACHE_H_
#define PCP_E_CACHE_H_
#include <boost/unordered_map.hpp>
#include "base_cache.h"
#include "ccnsim.h"

using namespace std;
using namespace boost;

typedef std::list<chunk_t> chunk_queue;

class pcp_e_cache : public base_cache{
	friend class statistics;

public:
	pcp_e_cache():base_cache(),L(0){;}
	bool full();
	void print_cache ();

protected:
	void data_store(chunk_t, int, long);
	bool data_lookup(chunk_t, int, long);
	bool fake_lookup(chunk_t);

private:
	int L; // offset
	unordered_map<chunk_t, int> cache_index; // index of chunk to it's H
	map <int, chunk_queue> cache; // cache of queue of data, sorted based on it's H
};



#endif /* PCP_E_CACHE_H_ */
