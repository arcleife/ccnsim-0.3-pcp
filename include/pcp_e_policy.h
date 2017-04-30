/*
 * pcp_e_policy.h
 *
 *  Created on: Apr 17, 2017
 *      Author: arcleife
 */

#ifndef PCP_E_POLICY_H
#define PCP_E_POLICY_H

#include "decision_policy.h"

/*
* progressive caching policy (edge): Pretty much like lcd but with more parameter
* to cache data based on number of aggregated requests
*/
class PCP_E: public DecisionPolicy{
    public:
	PCP_E(int theta, core_layer *core){
		this->theta = theta;
		this->core = core;
	}

	virtual bool data_to_cache(ccn_data *msg){
	    double d = msg->getHops();
	    //int theta = msg->getTheta();
	    long face = core->get_requesting_face(msg->getChunk());
	    std::bitset<sizeof(size_t) * CHAR_BIT> b(face); // list of faces that requested the data
		int rp = b.count(); // number of faces that requested the data
	    if ( d==1 || rp >= theta )
		return true;
	    return false;
	}

    private:
	int theta;
	core_layer *core;
};

#endif /* PCP_E_POLICY_H */
