#include "portfolio.h"

PortfolioEntry::PortfolioEntry(const vector<string>& cells) :
		name(sanitizeString(cells.at(0))),
		instrumentType(sanitizeString(cells.at(1))),
		cusip(cells.at(2)),
		notional(convertNotional(cells.at(3))),
		maturity(cells.at(4)),
		coupon(convertPercent(cells.at(5))),
		couponsPerYear(convertInt(cells.at(6))),
		price(convertPrice(cells.at(7))),
		yield(convertPercent(cells.at(8))),
		cleanPrice(convertPrice(cells.at(9))),
		exprr(convertPercent(cells.at(10)))
	{}
const string PortfolioEntry::toString()
{
	return string()
		+ name + "," + instrumentType + ","
		+ cusip + "," + to_string(notional) + ","
		+ maturity + "," + to_string(coupon) + ","
		+ to_string(couponsPerYear) + "," + to_string(price) + ","
		+ to_string(yield) + "," + to_string(cleanPrice) + ","
		+ to_string(exprr);
}
int PortfolioEntry::convertNotional(string strNotional)
{
	return convertInt(strNotional);
}

double PortfolioEntry::convertPrice(string strPrice)
{
	replace(strPrice.begin(), strPrice.end(), '$', ' ');
	return convertDouble(strPrice);
}

/*! PortfolioData
Holds the rows of portfolio entries read from the portfolio .csv file
*/
PortfolioData::PortfolioData() : CSV("portfolio_for_project.csv", 1) { }
/*!
\param name a company name
\return row containing portfolio information about the company. Nullptr if not found.
*/
PortfolioEntry* PortfolioData::getByName(string name)
{
	for (size_t i = 0, n = size(); i < n; i++)
	{
		PortfolioEntry& issuer = at(i);
		if (issuer.name == name)
			return &issuer;
	}
	return nullptr;
}
/*!
\return The reported value of the portfolio in millions of dollars
*/
double PortfolioData::getReportedValue()
{
	double marketValue = 0;
	for (size_t i = 0, n = size(); i < n; i++)
	{
		PortfolioEntry& row = at(i);
		double targetValue;
		if (row.instrumentType == "CDS")
			targetValue = row.cleanPrice;
		else
			targetValue = row.price;
		marketValue = marketValue + (row.notional*targetValue / 100);
	}
	return marketValue;
}
/*!
\return The theoretical value of the portfolio in millions of dollars
*/
double PortfolioData::getTheorValue()
{
	double theorValue = 0;
	for (size_t i = 0, n = size(); i < n; i++)
	{
		PortfolioEntry& row = at(i);
		theorValue = theorValue + (row.notional*row.cleanPrice / 100);
	}
	return theorValue;
}