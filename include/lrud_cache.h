/*
 * lrud_cache.h
 *
 *  Created on: May 8, 2017
 *      Author: arcleife
 */

#ifndef LRUD_CACHE_H_
#define LRUD_CACHE_H_

#include <boost/unordered_map.hpp>
#include "base_cache.h"
#include "ccnsim.h"


using namespace std;
using namespace boost;


//Indicate the position within the lru cache. In order to look-up for an
//element it just suffices removing the element from the current position and
//inserting it within the head of the list
struct lru_pos{
    //older and newer track the lru_position within the
    //lru cache
    lru_pos* older;
    lru_pos* newer;
    chunk_t k;
    simtime_t hit_time;
    int hops; // hop count for lrud
	//<aa>
	double cost; //meaningful only with cost aware caching
	//</aa>
};

//Defines a simple lru cache composed by a map and a list of position within the map.
class lrud_cache:public base_cache{
    friend class statistics;
    public:
		lrud_cache():base_cache(),actual_size(0),lru(0),mru(0){;}
		//<aa>
		lru_pos* get_mru();
		lru_pos* get_lru();
		const lru_pos* get_eviction_candidate();
		//</aa>

		bool full(); //<aa> moved from protected to public </aa>

    protected:
		void data_store(chunk_t, int, long);
		bool data_lookup(chunk_t, int, long);
		bool fake_lookup(chunk_t);

		void dump();


    private:
		uint32_t actual_size; //actual size of the cache
		lru_pos* lru; //least recently used item
		lru_pos* mru; //most recently used item

		unordered_map<chunk_t, lru_pos*> cache; //cache of values

};
#endif /* INCLUDE_LRUD_CACHE_H_ */
