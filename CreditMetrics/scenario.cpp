#include "scenario.h"
#include <iostream>
#include <math.h>
#include <boost/math/distributions/normal.hpp>

double IndustryScenario::calcIndustryDraw(NormalRandomNumberGenerator& randGen, IndustryEntry& industryEntry)
{
	double random1 = randGen.rand();
	double random2 = randGen.rand();
	return (industryEntry.correl * random1) + (sqrt(1 - (industryEntry.correl*industryEntry.correl)) * random2);
}
IndustryScenario::IndustryScenario(NormalRandomNumberGenerator& randGen, IndustryEntry& industryEntry) :
	industry(industryEntry.industry),
	industryDraw(calcIndustryDraw(randGen, industryEntry))
{}
IndustryScenarioData::IndustryScenarioData(NormalRandomNumberGenerator& randGen, IndustryData& industryData)
{
	for (size_t i = 0, n = industryData.size(); i < n; i++)
	{
		IndustryEntry& entry = industryData[i];
		push_back(IndustryScenario(randGen, entry));
	}
}
IndustryScenario* IndustryScenarioData::getByName(string industry)
{
	for (size_t i = 0, n = size(); i < n; i++)
	{
		IndustryScenario& entry = at(i);
		if (entry.industry == industry)
			return &entry;
	}
	return nullptr;
}
ScenarioEntry::ScenarioEntry(NormalRandomNumberGenerator& randGen, IssuerEntry& issuerEntry, IndustryScenario& industryScenario, TransitionMatrix& transitionMatrix) :
	name(issuerEntry.name),
	percentile(calculatePercentile(randGen, issuerEntry, industryScenario)),
	rating(calculateRating(randGen, issuerEntry, industryScenario, transitionMatrix)) { }
const size_t ScenarioEntry::calculateRating(NormalRandomNumberGenerator& randGen, IssuerEntry& issuerEntry, IndustryScenario& industryScenario, TransitionMatrix& transitionMatrix)
{
	size_t rating = convertRating(issuerEntry.rating);
	//Assumes rating >= 0 && <= 7
	MatrixRow& row = transitionMatrix.cumSumMatrix[rating];
	size_t i = row.size() - 1;
	for(size_t j = 0, n = row.size(); j<n; j++)
	{
		if(percentile <= row[i])
			return i;
		i--;
	}
	cout << "Failed to find the rating. Using original rating: " << to_string(rating) << endl;
	throw new runtime_error("Bad rating");
}
const double ScenarioEntry::calculatePercentile(NormalRandomNumberGenerator& randGen, IssuerEntry& issuerEntry, IndustryScenario& industryScenario)
{
	double random3 = randGen.rand();
	double assetReturn =  (issuerEntry.correl * industryScenario.industryDraw) + (sqrt(1 - (issuerEntry.correl*issuerEntry.correl)) * random3);
	boost::math::normal norm;
	double percentile = boost::math::cdf(norm, assetReturn);
	return percentile;
}
const size_t ScenarioEntry::convertRating(string rating)
{
	size_t convertedRating;
	if (rating == "AAA")
		convertedRating = 0;
	else if (rating == "AA")
		convertedRating = 1;
	else if (rating == "A")
		convertedRating = 2;
	else if (rating == "BBB")
		convertedRating = 3;
	else if (rating == "BB")
		convertedRating = 4;
	else if (rating == "B")
		convertedRating = 5;
	else if (rating == "CCC")
		convertedRating = 6;
	else if (rating == "C")
		convertedRating = 6;
	else if (rating == "D")
		convertedRating = 7;
	return convertedRating;
}
Scenario::Scenario(NormalRandomNumberGenerator& randGen, IssuerData& issuerData, IndustryData& industryData, TransitionMatrix& transitionMatrix)
{
	IndustryScenarioData industryScenarioData(randGen, industryData);
	for (size_t i = 0, n = issuerData.size(); i < n; i++)
	{
		IssuerEntry& issuerEntry = issuerData.at(i);
		IndustryScenario* industryScenario = industryScenarioData.getByName(issuerEntry.industry);
		if (!industryScenario)
			throw runtime_error("No known industry entry for \"" + issuerEntry.industry + "\"");
		push_back(ScenarioEntry(randGen, issuerEntry, *industryScenario, transitionMatrix));
	}
}
ScenarioEntry* Scenario::getByName(string name)
{
	for (size_t i = 0, n = size(); i < n; i++)
	{
		ScenarioEntry& entry = at(i);
		if (entry.name == name)
			return &entry;
	}
	return nullptr;
}
