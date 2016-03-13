#include "monte.h"

Monte::Monte(int N, IssuerData& issuerData)
{
	for (size_t i = 0; i < N; i++)
	{
		scenarios.push_back(Scenario(issuerData));
	}
}
vector<Scenario> scenarios;