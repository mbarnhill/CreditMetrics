#include "scenario.h"

ScenarioEntry::ScenarioEntry(IssuerEntry& entry) :
	name(entry.name),
	rating(convertRating(entry.rating))
{}
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
Scenario::Scenario(IssuerData& data)
{
	for (size_t i = 0, n = data.size(); i < n; i++)
		push_back(ScenarioEntry(data.at(i)));
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