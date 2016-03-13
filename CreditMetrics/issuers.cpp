#include "issuers.h"

IssuerEntry::IssuerEntry(const vector<string>& cells) :
	name(sanitizeString(cells.at(0))),
	rating(sanitizeString(cells.at(1))),
	industry(sanitizeString(cells.at(2))),
	index(convertDouble(cells.at(3)))
{}
const string IssuerEntry::toString()
{
	return string()
		+ name + "," + rating + "," + industry + "," + to_string(index);
}

IssuerData::IssuerData(): CSV("issuers.csv", 1) { }

IssuerEntry* IssuerData::getByName(string name)
{
	for (size_t i = 0, n = size(); i < n; i++)
	{
		IssuerEntry& issuer = at(i);
		if (issuer.name == name)
			return &issuer;
	}
	return nullptr;
}