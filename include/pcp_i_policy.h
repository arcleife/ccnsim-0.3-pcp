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
	PCP_I(int dp){
		this->dp = dp;
	}

	virtual bool data_to_cache(ccn_data *msg){
	    double d = msg->getHops();
	    int theta = msg->getTheta();
	    if ( d==1 || theta >= dp )
		return true;
	    return false;
	}

    private:
	int dp;
};

#endif /* PCP_I_POLICY_H */
