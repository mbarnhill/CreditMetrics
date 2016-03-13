#include "yields.h"

/*! YieldEntry
Labels the cells of the yield curve data
*/
YieldEntry::YieldEntry(const vector<string>& cells) :
	term(convertDouble(cells.at(0))),
	aaa(convertPercent(cells.at(1))),
	aa(convertPercent(cells.at(2))),
	a(convertPercent(cells.at(3))),
	bbb(convertPercent(cells.at(4))),
	bb(convertPercent(cells.at(5))),
	b(convertPercent(cells.at(6))),
	ccc(convertPercent(cells.at(7))),
	govt(convertPercent(cells.at(8)))
{}
const string YieldEntry::toString()
{
	return string()
		+ to_string(term) + "," + to_string(aaa) + ","
		+ to_string(aa) + "," + to_string(a) + ","
		+ to_string(bbb) + "," + to_string(bb) + ","
		+ to_string(b) + "," + to_string(ccc) + ","
		+ to_string(govt);
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