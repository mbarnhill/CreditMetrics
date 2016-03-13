#include "monte.h"

Monte::Monte(int N, NormalRandomNumberGenerator& randGen, IssuerData& issuerData, IndustryData& industryData, TransitionMatrix& transitionMatrix) :
	scenarios()
{
	for (size_t i = 0; i < N; i++)
		scenarios.push_back(Scenario(randGen,issuerData,industryData,transitionMatrix));
}
