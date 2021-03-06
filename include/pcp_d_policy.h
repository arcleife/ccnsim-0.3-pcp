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
	    int dp = b.count(); // number of faces that requested the data
	    int n_face = node->gateSize("face") - 1; //number of faces connected to node
	    int theta;
	    if (n_face <= 3){
	    	theta = 2; //node will never aggregate request if theta = 1
	    } else {
	    	theta = n_face - 1;
	    }
	    if ( d==1 || dp >= theta )
		return true;
	    return false;
	}

    private:
	int id;
};


#endif /* INCLUDE_PCP_D_POLICY_H_ */
