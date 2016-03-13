#ifndef __scenario_h__
#define __scenario_h__

#include <vector>
#include "csv.h"
#include "issuers.h"
#include "industries.h"
#include "rand.h"

class IndustryScenario
{
public:
	IndustryScenario(NormalRandomNumberGenerator&, IndustryEntry&);
	string industry;
	double industryDraw;
private:
	double calcIndustryDraw(NormalRandomNumberGenerator&, IndustryEntry&);
};
class IndustryScenarioData : vector<IndustryScenario>
{
public:
	IndustryScenarioData(NormalRandomNumberGenerator&, IndustryData&);
	IndustryScenario* getByName(string name);
};
/*! ScenarioEntry
Create a scenario for a company at the end of the year.
Converts rating to integer values.
For now we let all scenarios be the same rating the companies started with.
*/
class ScenarioEntry
{
public:
	ScenarioEntry(NormalRandomNumberGenerator& randGen, IssuerEntry& issuerEntry, IndustryScenario& industryScenario);
	const string name;
	const int rating;
	const double assetReturn;
private:
	const double calculateAssetReturn(NormalRandomNumberGenerator&, IssuerEntry&, IndustryScenario&);
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