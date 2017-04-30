/*
 * pcp_i_policy.h
 *
 *  Created on: Apr 17, 2017
 *      Author: arcleife
 */

#ifndef PCP_I_POLICY_H
#define PCP_I_POLICY_H

#include "decision_policy.h"

/*
* progressive caching policy (intermediate): Pretty much like lcd but with more parameter
* to cache data based on number of aggregated requests
*/
class PCP_I: public DecisionPolicy{
    public:
	PCP_I(int theta, core_layer *core){
		this->theta = theta;
		this->core = core;
	}

	virtual bool data_to_cache(ccn_data *msg){
	    double d = msg->getHops();
		long face = core->get_requesting_face(msg->getChunk());
		std::bitset<sizeof(size_t) * CHAR_BIT> b(face); // list of faces that requested the data
		int dp = b.count(); // number of faces that requested the data
	    if ( d==1 || dp >= theta )
		return true;
	    return false;
	}

    private:
	int theta;
	core_layer *core;
};

#endif /* PCP_I_POLICY_H */
