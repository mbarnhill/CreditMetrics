#ifndef __issuers_h__
#define __issuers_h__

#include <string>
#include <vector>
#include "csv.h"

/*! IssuerEntry
Labels the cells of the issuers data
*/
class IssuerEntry
{
public:
	IssuerEntry(const vector<string>&);
	const string name;
	const string rating;
	const string industry;
	const string toString();
};
/*! IssuerData
Holds the rows of issuer entries read from the issuers .csv file
*/
class IssuerData : public CSV<IssuerEntry>
{
public:
	IssuerData();
	/*!
	\param name a company name
	\return row containing information about the company. Nullptr if not found.
	*/
	IssuerEntry* getByName(string);
};

#endif
