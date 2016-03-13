#ifndef __portfolio_h__
#define __portfolio_h__

#include <string>
#include <vector>
#include "csv.h"

/*! PortfolioEntry
Labels the cells of the portfolio data
*/
class PortfolioEntry
{
public:
	PortfolioEntry(const vector<string>&);

	const string name, instrumentType, cusip, maturity;
	const int notional, couponsPerYear;
	const double coupon, price, yield, cleanPrice, exprr;
	const string toString();
private:
	int convertNotional(string);
	double convertPrice(string);
};

/*! PortfolioData
Holds the rows of portfolio entries read from the portfolio .csv file
*/
class PortfolioData : public CSV<PortfolioEntry>
{
public:
	PortfolioData();
	/*!
	\param name a company name
	\return row containing portfolio information about the company. Nullptr if not found.
	*/
	PortfolioEntry* getByName(string);

	/*!
	\return The reported value of the portfolio in millions of dollars
	*/
	double getReportedValue();

	/*!
	\return The theoretical value of the portfolio in millions of dollars
	*/
	double getTheorValue();
};

#endif
