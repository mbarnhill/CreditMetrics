#ifndef __yields_h__
#define __yields_h__

#include <string>
#include <vector>
#include "csv.h"
/*! YieldEntry
Labels the cells of the yield curve data
*/
class YieldEntry : public vector<double>
{
public:
	YieldEntry(const vector<string>&);
	const double term;
	const string toString();
};
/*! YieldData
Holds the rows of yield values read from the yield curve .csv file
*/
class YieldData : public CSV<YieldEntry>
{
public:
	YieldData();
	YieldEntry* getByTerm(double);
};
#endif
