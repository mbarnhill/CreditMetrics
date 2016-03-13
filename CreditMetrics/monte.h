#ifndef __monte_h__
#define __monte_h__

#include <vector>
#include "issuers.h"
#include "scenario.h"
#include "industries.h"
#include "rand.h"
#include "transition_matrix.h"

/*! Monte
Performs a Monte Carlo simulation by creating a vector of N scenarios
*/
class Monte
{
public:
	Monte(int, NormalRandomNumberGenerator&, IssuerData&, IndustryData&, TransitionMatrix&);
	vector<Scenario> scenarios;
};


#endif