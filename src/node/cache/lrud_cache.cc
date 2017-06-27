/*
 * lrud_cache.cc
 *
 *  Created on: May 8, 2017
 *      Author: arcleife
 */

#include <iostream>
#include "lrud_cache.h"

//<aa>
#include "error_handling.h"
//</aa>

Register_Class(lrud_cache);


void lrud_cache::data_store(chunk_t elem, int hops, long f){
    //When the element is already stored within the cache, simply update the
    //position of the element within the list and exit
    if (data_lookup(elem, hops, f))
	return;

    lru_pos *p = (lru_pos *)malloc (sizeof(lru_pos)); //position for the new element
										//<aa> i.e. datastructure for the new element </aa>
    //lru_pos *p = new lru_pos();
    p->k = elem;
    p->hit_time = simTime();
    p->hops = hops;
    p->newer = 0;
    p->older = 0;

    //The cache is empty. Add just one element. The mru and lru element are the
    //same
    if (actual_size == 0){
        actual_size++;
        lru = mru = p;
        cache[elem] = p;
        //nyoba
        //std::cout << "node " << getIndex() << " | " << cache.size() << " | latest data " << elem << endl;
        return;
    }

    //The cache is not empty. The new element is the newest. Add in the front
    //of the list
    p->older = mru; // mru swaps in second position (in terms of utilization rank)
    mru->newer = p; // update the newer element for the secon newest element
    mru = p; //update the mru (which becomes that just inserted)

    if (actual_size==get_size()){
        //if the cache is full, check the last two chunks
        //
    	if (lru->hops <= lru->newer->hops){
    		// lru has smaller hops than chunk after lru
			chunk_t k = lru->k;
			lru_pos *tmp = lru;
			lru = tmp->newer; //the new lru is the element before the least recently used

			lru->older = 0; //as it is still in memory for a while set the actual lru point to null (CHECK this)
			tmp->older = 0;
			tmp->newer = 0;

			free(tmp);
			cache.erase(k); //erase from the cache the most unused element
    	} else {
    		// lru has higher hops than chunk after lru
    		chunk_t k = lru->newer->k;
    		lru_pos *tmp = lru->newer;
    		lru->newer = tmp->newer; // connect lru to lru + 2
    		lru->newer->older = lru; // connect lru + 2 to lru

    		tmp->older = 0;
    		tmp->newer = 0;
    		free(tmp);
    		cache.erase(k); //erase from the cache
    	}
    }else
        //otherwise do nothing, just update the actual_size of the cache
        actual_size++;

    cache[elem] = p; //store the new element together with its position
    //nyoba
    //std::cout << "node " << getIndex() << " | " << cache.size() << " | latest data " << elem << endl;


}

//<aa>
lru_pos* lrud_cache::get_mru(){
	return mru;
}
lru_pos* lrud_cache::get_lru(){
	#ifdef SEVERE_DEBUG
	if (lru != NULL){
		// To see if a seg fault arises due to the access to a forbidden area
		// To use with valgrind software
		chunk_t test = lru->k;
	} //else the cache is empty
	#endif

	return lru;
}

const lru_pos* lrud_cache::get_eviction_candidate(){
	if ( full() )
		return get_lru();
	else return NULL;
}

//</aa>

bool lrud_cache::fake_lookup(chunk_t elem){
//    if (getIndex()==12)
//	return true;
    unordered_map<chunk_t,lru_pos *>::iterator it = cache.find(elem);
    //look for the elements
    if (it==cache.end()){
	//if not found return false and do nothing
	return false;

    }else
	return true;
}

bool lrud_cache::data_lookup(chunk_t elem, int hops, long f){
    //updating an element is just a matter of manipulating the list
    unordered_map<chunk_t,lru_pos *>::iterator it = cache.find(elem);

    //
    //look for the elements
    if (it==cache.end()){
	//if not found return false and do nothing
	return false;

    }

    lru_pos* pos_elem = it->second;
    if (pos_elem->older && pos_elem->newer){
        //if the element is in the middle remove the element from the list
        pos_elem->newer->older = pos_elem->older;
        pos_elem->older->newer = pos_elem->newer;
    }else if (!pos_elem->newer){
        //if the element is the mru
        return true; //do nothing, return true
    } else{
        //if the element is the lru, remove the element from the bottom of the list
        lru = pos_elem->newer;
        lru->older = 0;
    }


    //Place the elements as in front of the position list (it's the newest one)
    pos_elem->older = mru;
    pos_elem->newer = 0;
    mru->newer = pos_elem;

    //update the mru
    mru = pos_elem;
    mru->hit_time = simTime();
    return true;
}


void lrud_cache::dump(){
    lru_pos *it = mru;
    int p = 1;
    while (it){
	cout<<p++<<" ]"<< __id(it->k)<<"/"<<__chunk(it->k)<<endl;
	it = it->older;
    }
}

bool lrud_cache::full(){
    return (actual_size==get_size());
}
