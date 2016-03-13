#include "issuers.h"

IssuerEntry::IssuerEntry(const vector<string>& cells) :
	name(sanitizeString(cells.at(0))),
	rating(sanitizeString(cells.at(1))),
	industry(sanitizeString(cells.at(2)))
{}
const string IssuerEntry::toString()
{
	return string()
		+ name + "," + rating + "," + industry;
}

IssuerData::IssuerData(): CSV("issuers_old.csv", 1) { }

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