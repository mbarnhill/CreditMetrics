#ifndef __monte_h__
#define __monte_h__

#include "issuers.h"
#include "scenario.h"

/*! Monte
Performs a Monte Carlo simulation by creating a vector of N scenarios
*/
class Monte
{
public:
	Monte(int, IssuerData& issuerData);
	vector<Scenario> scenarios;
};

#endif