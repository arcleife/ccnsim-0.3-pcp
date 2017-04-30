/*
 * pcp_i_cache.h
 *
 *  Created on: Apr 17, 2017
 *      Author: arcleife
 */

#ifndef PCP_I_CACHE_H_
#define PCP_I_CACHE_H_
#include <boost/unordered_map.hpp>
#include "base_cache.h"
#include "ccnsim.h"

using namespace std;
using namespace boost;

struct chunk_info{
	int h;
	int hops;
	long s1;
	long s2;
};

typedef std::list<chunk_t> chunk_queue;

class pcp_i_cache : public base_cache{
	friend class statistics;

public:
	bool full();
	void print_cache ();

protected:
	void data_store(chunk_t, int, long);
	bool data_lookup(chunk_t, int, long);
	bool fake_lookup(chunk_t);

private:
	unordered_map<chunk_t, chunk_info> cache_index; // index of chunk to it's H
	map <int, chunk_queue> cache; // cache of queue of data, sorted based on it's H

	chunk_t getChunkGivenPos(chunk_queue&, int);
};

#endif /* PCP_I_CACHE_H_ */
