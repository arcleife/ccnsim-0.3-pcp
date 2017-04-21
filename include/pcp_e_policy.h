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
	PCP_E(int rp){
		this->rp = rp;
	}

	virtual bool data_to_cache(ccn_data *msg){
	    double d = msg->getHops();
	    int theta = msg->getTheta();
	    if ( d==1 || theta >= rp )
		return true;
	    return false;
	}

    private:
	int rp;
};

#endif /* PCP_E_POLICY_H */
