#include "yields.h"

/*! YieldEntry
Labels the cells of the yield curve data
*/
YieldEntry::YieldEntry(const vector<string>& cells) :
	term(convertDouble(cells[0]))
{
	for(size_t i=1, n=cells.size(); i<n; i++)
		this->push_back(convertPercent(cells[i]));
}
const string YieldEntry::toString()
{
	string ret;
	ret = ret + to_string(this->term) + ",";
	for(int i=0, n=this->size(); i<n; i++) {
		ret = ret + to_string((*this)[i]);
		if(i < (n-1))
			ret = ret + ",";
	}
	return ret;
}
YieldData::YieldData() : CSV("yield_curve_for_project.csv", 1) { }
YieldEntry* YieldData::getByTerm(double term)
{
	for (size_t i = 0, n = size(); i < n; i++)
	{
		YieldEntry& yield = at(i);
		if (yield.term == term)
			return &yield;
	}
	return nullptr;
}