/*
 * pcp_d_policy.h
 *
 *  Created on: Apr 21, 2017
 *      Author: arcleife
 */

#ifndef PCP_D_POLICY_H_
#define PCP_D_POLICY_H_

#include <omnetpp.h>
#include "decision_policy.h"

/*
* progressive caching policy (dynamic): PCP with dynamic threshold to cache data
* based on the number of face that connected to the node
*/
class PCP_D: public DecisionPolicy{
    public:
	PCP_D(int id){
		this->id = id;
	}

	virtual bool data_to_cache(ccn_data *msg){
	    double d = msg->getHops();
	    cModule *node = cSimulation::getActiveSimulation()->getModule(id);
	    long face = ((core_layer *) node->getModuleByPath(".core_layer"))->get_requesting_face(msg->getChunk());
	    std::bitset<sizeof(size_t) * CHAR_BIT> b(face); // list of faces that requested the data
	    int theta = b.count(); // number of faces that requested the data
	    int n_face = node->gateSize("face") - 1; //number of faces connected to node
	    //cout << theta << endl;
	    //cout << b << endl;
	    //cout << gate_number << endl;
	    //dpnya tentuin dulu
	    if ( d==1 || theta >= 2 ) // entar si 2 nya ganti sama dynamic dp
		return true;
	    return false;
	}

    private:
	int id;
};


#endif /* INCLUDE_PCP_D_POLICY_H_ */
