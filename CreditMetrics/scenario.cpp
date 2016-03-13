#include "scenario.h"

ScenarioEntry::ScenarioEntry(IssuerEntry& issuerEntry, IndustryEntry& industryEntry) :
	name(issuerEntry.name),
	rating(calculateRating(convertRating(issuerEntry.rating), issuerEntry.correl, industryEntry.correl))
{}
const int ScenarioEntry::calculateRating(int rating, double issuerCorrel, double industryCorrel)
{
	return 0;
}
const int ScenarioEntry::convertRating(string rating)
{
	int convertedRating;
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
Scenario::Scenario(IssuerData& issuerData, IndustryData& industryData)
{
	for (size_t i = 0, n = issuerData.size(); i < n; i++)
	{
		IssuerEntry& issuerEntry = issuerData.at(i);
		IndustryEntry* industryEntry = industryData.getByName(issuerEntry.industry);
		if (!industryEntry)
			throw runtime_error("No known industry entry for \"" + issuerEntry.industry + "\"");
		push_back(ScenarioEntry(issuerEntry, *industryEntry));
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