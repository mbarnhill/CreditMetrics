#include "industries.h"

IndustryEntry::IndustryEntry(const vector<string>& cells) :
	industry(cells.at(0)),
	correl(convertDouble(cells.at(1))) {}

const string IndustryEntry::toString()
{
	return string()
		+ industry + "," + to_string(correl);
}
IndustryData::IndustryData() : CSV("industries.csv", 1) {}
IndustryEntry* IndustryData::getByName(string industry)
{
	for (size_t i = 0, n = size(); i < n; i++)
	{
		IndustryEntry& industryEntry = at(i);
		if (industryEntry.industry == industry)
			return &industryEntry;
	}
	return nullptr;
}