#ifndef __industries_h__
#define __industries_h__

#include <string>
#include <vector>
#include "csv.h"

/*!
Labels the cells of the industries data
*/
class IndustryEntry
{
public:
	IndustryEntry(const vector<string>&);
	const string industry;
	const double correl;
	const string toString();
};
/*! IssuerData
Holds the rows of industry entries read from the industries.csv file
*/
class IndustryData : public CSV<IndustryEntry>
{
public:
	IndustryData();
	/*!
	\param name an industry name
	\return row containing information about the industry. Nullptr if not found.
	*/
	IndustryEntry* getByName(string);
};

#endif
