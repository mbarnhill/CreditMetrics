#ifndef __scenario_h__
#define __scenario_h__

#include "csv.h"
#include "issuers.h"
#include "industries.h"
#include "rand.h"

/*! ScenarioEntry
Create a scenario for a company at the end of the year.
Converts rating to integer values.
For now we let all scenarios be the same rating the companies started with.
*/
class ScenarioEntry
{
public:
	ScenarioEntry(NormalRandomNumberGenerator&, IssuerEntry&, IndustryEntry&);
	const string name;
	const int rating;
private:
	const int calculateRating(NormalRandomNumberGenerator&, int, double, double);
	const int convertRating(string);
};
/*! Scenario
Create a set scenarios, one for each company, at the end of the year.
*/
class Scenario : public vector<ScenarioEntry>
{
public:
	Scenario(NormalRandomNumberGenerator&, IssuerData&, IndustryData&);
	vector<ScenarioEntry> entries;
	ScenarioEntry* getByName(string);
};

#endif