/*! CreditMetrics.cpp : Defines the entry point for the console application.
	Note: The given .csv files are not formatted consistently. The .csv parsers below are 
	designed to handle the inconsistencies present in the files in a somewhat 
	general manner by removing extra characters and white space.
	Note: Some of the values in the csv file are set to "-", which is read as zero by c++, 
	so we do not account for this particular inconsistency, as we intend that any of these to be zero regardless.
*/
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional> 
#include <numeric>
#include <cctype>
#include <locale>
#include <random>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
using namespace std;

static inline string& trim(string& s) {
	//left
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	//right
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
	return s;
}
static inline int convertInt(string strInt)
{
	trim(strInt);
	return ::atoi(strInt.c_str());
}
static inline double convertDouble(string strDouble)
{
	trim(strDouble);
	return ::atof(strDouble.c_str());
}
static inline double convertPercent(string strPercent)
{
	replace(strPercent.begin(), strPercent.end(), '%', ' ');
	return convertDouble(strPercent) / 100;
}
static inline string sanitizeString(string str)
{
	boost::to_upper(str);
	trim(str);
	return str;
}

/*! Response for Part B, Step 1) 
\Read in all the .csv files and create transition and correlation matrices.
\Generically stores rows from a .csv file. Conents of each row are passed as a vector of strings to the row class.
*/
template <class R> class CSV : public vector<R>
{
public:
	CSV(const string& filename, size_t skipLines)
	{
		ifstream file(filename);
		if (!file.good())
		{
			throw runtime_error("File does not exist");
		}
		while (!file.eof())
		{
			string strline;
			getline(file, strline);
			if (skipLines > 0)
			{
				skipLines--;
				continue;
			}
			if (strline.empty())
			{
				continue;
			}
			stringstream streamline(strline);
			vector<string> cells;
			while (!streamline.eof())
			{
				string cell;
				getline(streamline, cell, ',');
				if (cell != "")
				{
					cells.push_back(cell);
				}
			}
			if (cells.empty())
			{
				continue;
			}
			push_back(R(cells));
		}
	}
	const string toString()
	{
		string ret;
		for (size_t i = 0, n = size(); i < n; i++)
			ret = ret + at(i).toString() + "\n";
		return ret;
	}
};
/*! IssuerEntry
\Labels the cells of the issuers data
*/
class IssuerEntry
{
public: 
	IssuerEntry(const vector<string>& cells):
		name(sanitizeString(cells.at(0))),
		rating(sanitizeString(cells.at(1))),
		industry(sanitizeString(cells.at(2)))
	{}
	const string name;
	const string rating;
	const string industry;
	const string toString() const
	{
		return string()
			+ name + "," + rating + "," + industry;
	}
};
/*! IssuerData
\Gets and returns an issuer by the company name
*/
class IssuerData : public CSV<IssuerEntry>
{
public:
	IssuerData() : CSV("issuers.csv", 1) { }
	IssuerEntry* getByName(string name)
	{
		for (size_t i = 0, n = size(); i < n; i++)
		{
			IssuerEntry& issuer = at(i);
			if(issuer.name == name)
				return &issuer;
		}
		return nullptr;
	}
};
/*! PortfolioEntry
\Labels the cells of the portfolio data
*/
class PortfolioEntry
{
public:
	PortfolioEntry(const vector<string>& cells) :
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
	const string name, instrumentType, cusip, maturity;
	const int notional, couponsPerYear;
	const double coupon, price, yield, cleanPrice,exprr;
	const string toString() const
	{
		return string()
			+ name + "," + instrumentType + ","
			+ cusip + "," + to_string(notional) + ","
			+ maturity + "," + to_string(coupon) + ","
			+ to_string(couponsPerYear) + "," + to_string(price) + ","
			+ to_string(yield) + "," + to_string(cleanPrice) + ","
			+ to_string(exprr);
	}
private:
	int convertNotional(string strNotional)
	{
		return convertInt(strNotional);
	}
	double convertPrice(string strPrice)
	{
		replace(strPrice.begin(), strPrice.end(), '$', ' ');
		return convertDouble(strPrice);
	}
};
/*! PortfolioData, Includes response for Part B, Step 2)
\Gets and returns a portfolio entry by the company name
*/
class PortfolioData : public CSV<PortfolioEntry>
{
public:
	PortfolioData() : CSV("portfolio_for_project.csv", 1) { }
	PortfolioEntry* getByName(string name)
	{
		for (size_t i = 0, n = size(); i < n; i++)
		{
			PortfolioEntry& issuer = at(i);
			if (issuer.name == name)
				return &issuer;
		}
		return nullptr;
	}
	/*! Response for Part B, Step 2) Reported Value 
		\return The reported value of the portfolio in millions of dollars
	*/
	double getReportedValue()
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
	/*! Response for Part B, Step 2) Theoretical Value
	\return The theoretical value of the portfolio in millions of dollars
	*/
	double getTheorValue()
	{
		double theorValue = 0;
		for (size_t i = 0, n = size(); i < n; i++)
		{
			PortfolioEntry& row = at(i);
			theorValue = theorValue + (row.notional*row.cleanPrice / 100);
		}
		return theorValue;
	}
};
/*! YieldEntry
\Labels the cells of the yield curve data
*/
class YieldEntry
{
public:
	YieldEntry(const vector<string>& cells):
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
	const double term, aaa, aa, a, bbb, bb, b, ccc, govt;
	const string toString() const
	{
		return string() 
			+ to_string(term) + "," + to_string(aaa) + ","
			+ to_string(aa) + "," + to_string(a) + ","
			+ to_string(bbb) + "," + to_string(bb) + ","
			+ to_string(b) + "," + to_string(ccc) + ","
			+ to_string(govt);
	}
};
/*! YieldData
\Gets and returns a row of yield values by the term
*/
class YieldData : public CSV<YieldEntry>
{
public:
	YieldData() : CSV("yield_curve_for_project.csv", 1) { }
	YieldEntry* getByTerm(double term)
	{
		for (size_t i = 0, n = size(); i < n; i++)
		{
			YieldEntry& yield = at(i);
			if (yield.term == term)
				return &yield;
		}
		return nullptr;
	}
};
/*! MatrixRow
\Stores an array of doubles
*/
class MatrixRow : public vector<double>
{
public:
	MatrixRow(const vector<string>& cells)
	{
		for (size_t i = 1, n = cells.size(); i < n; i++)
		{
			push_back(convertDouble(cells.at(i)));
		}
	}
	using vector::vector;
	string toString()
	{
		string ret;
		for (size_t i = 0, n = size(); i < n; i++)
		{
			if (i > 0)
				ret = ret + ",";
			ret = ret + to_string(at(i));
		}
		return ret;
	}
};
/*! Matrix
\Stores an array of matrix rows (vector of vectors of doubles)
*/
class Matrix : public CSV<MatrixRow>
{
public:
	Matrix(const string& filename, size_t skipLines) : CSV(filename, skipLines) {}
};

/*! ScenarioEntry and Scenario: Response for Part B, Step 4)
\Return scenarios for possible ratings for the companies at the end of the year.
\For now we let all scenarios be the same rating the companies started with.
*/
class ScenarioEntry
{
public:
	ScenarioEntry(IssuerEntry& entry) :
		name(entry.name),
		rating(convertRating(entry.rating))
	{}
	const string name;
	const int rating;
	const int convertRating(string rating) const
	{
		int convertedRating;
		if (rating == "AAA")
			convertedRating = 0;
		else if (rating == "AA")
			convertedRating = 1;
		else if (rating == "A")
			convertedRating = 2;
		else if (rating == "BBB")
			convertedRating = 3;
		else if (rating == "BB")
			convertedRating = 4;
		else if (rating == "B")
			convertedRating = 5;
		else if (rating == "CCC")
			convertedRating = 6;
		else if (rating == "C")
			convertedRating = 6;
		else if (rating == "D")
			convertedRating = 7;
		return convertedRating;
	}
};

class Scenario : public vector<ScenarioEntry>
{
public:
	Scenario(IssuerData& data)
	{
		for (size_t i = 0, n = data.size(); i < n; i++)
			push_back(ScenarioEntry(data.at(i)));
	}
	vector<ScenarioEntry> entries;
	ScenarioEntry* getByName(string name)
	{
		for (size_t i = 0, n = size(); i < n; i++)
		{
			ScenarioEntry& entry = at(i);
			if (entry.name == name)
				return &entry;
		}
		return nullptr;
	}
};
/*! Monta: Response for Part B, Step 4) (Continued)
\Return a N scenarios
*/
class Monte
{
public:
	Monte(int N, IssuerData& issuerData) 
	{
		for (size_t i = 0; i < N; i++)
		{
			scenarios.push_back(Scenario(issuerData));
		}
	}
	vector<Scenario> scenarios;
};

int main(int argc, char* argv[])
{
	try
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<> dis(0, 1);

		/*! Returning all data from step 1.
		\See below to read out all of the data from Step 1.
		*/
		IssuerData issuerData;
		// cout << issuerData.toString();
		PortfolioData portfolioData;
		// cout << portfolioData.toString();
		YieldData yieldData;
		// cout << yieldData.toString();
		Matrix correlationMatrix("correlation_matrix_for_project.csv", 1);
		// cout << correlationMatrix.toString();
		Matrix transitionMatrix("transition_matrix_for_project.csv", 3);
		MatrixRow row{ 0,0,0,0,0,0,0,1 };
		transitionMatrix.push_back(row);
		// cout << transitionMatrix.toString();
		cout << "The initial reported portfolio value is " << portfolioData.getReportedValue() << endl;
		cout << "The initial theoretical portfolio value is " << portfolioData.getTheorValue() << endl;

		/*! Response for Part B, Step 3) 
		\Return a matrix of possible prices for each instrument for each possible credit rating.
		\For the moment, we designate each performing bond price to be the theoretical price from above, 
		\and each performing CDS to be a random number between 0 and 1 (on a notional of $100).
		\Defaulted bonds and CDSs are designated as the expected recovery values.
		\ NOTE: requires the boost library.
		*/

		boost::numeric::ublas::matrix<double> priceMatrix(portfolioData.size(), 8);
		for (size_t i = 0, n1 = priceMatrix.size1(); i < n1; i++)
		{
			PortfolioEntry& row = portfolioData.at(i);
			if (row.instrumentType == "CDS")
			{
				for (size_t j = 0, n2 = priceMatrix.size2() - 1; j < n2; j++)
					priceMatrix(i, j) = dis(gen);
			}
			else
			{
				for (size_t j = 0, n2 = priceMatrix.size2() - 1; j < n2; j++)
					priceMatrix(i, j) = row.cleanPrice;
			}
			priceMatrix(i, priceMatrix.size2() - 1) = row.exprr * 100;
		}
		//cout << priceMatrix << endl;

		/*! Response for Part B, Step 5)
		\Set N and go through the N scenarios and compute the value of the portfolio, 
		\and change in value of the portfolio in each scenario.
		*/
		int N = 2000;
		Monte monteCarlo(N, issuerData);
	
		vector<double> portfolioValues;
		vector<double> changeInValues;
		double changeInValueTotal = 0;
		for (size_t i = 0, n1 = monteCarlo.scenarios.size(); i < n1; i++)
		{
			double portfolioValue = 0;
			Scenario& scenario = monteCarlo.scenarios.at(i);
			for (size_t j = 0, n2 = portfolioData.size(); j < n2; j++)
			{
				PortfolioEntry& portfolio = portfolioData.at(j);
				ScenarioEntry* scenarioEntry = scenario.getByName(portfolio.name);
				if (!scenarioEntry)
					throw runtime_error("No known scenario entry for \"" + portfolio.name + "\"");
				portfolioValue = portfolioValue + ((priceMatrix(j, scenarioEntry->rating)*portfolio.notional) * ((double)1/100));
			}
			portfolioValues.push_back(portfolioValue);
			double changeInValue = portfolioValue - portfolioData.getTheorValue();
			changeInValues.push_back(changeInValue);
			changeInValueTotal = changeInValueTotal + changeInValue;
		}

		/*! Response for Part B, Step 6)
		\Calculate statistics for the change in portfolio value
		*/
		double meanChangeInValue = changeInValueTotal / (double)changeInValues.size();
		double sq_sum = inner_product(changeInValues.begin(), changeInValues.end(), changeInValues.begin(), 0.0);
		//NOTE: This calculation of standard deviation may not work for small values. 
		//TO DO: Fix this so a standard deviation of 0 does not cause an overflow.
		double stdev = sqrt((sq_sum / (double)changeInValues.size()) - (meanChangeInValue * meanChangeInValue));

		//cout << changeInValues[0] << endl;
		cout << "The average change in value is " << meanChangeInValue << endl;
		cout << "The standard deviations of the change in value is " << stdev << endl;

		sort(changeInValues.begin(), changeInValues.end());
		double var95Percentile = changeInValues.at((int) (changeInValues.size()*0.05));
		double var99Percentile = changeInValues.at((int) (changeInValues.size()*0.01));

		double percentile = 98;
		double cvar = 0;
		for (size_t i = 0, n1 = (int)(((100-percentile)*0.01)*changeInValues.size()); i < n1; i++)
		{
			cvar = cvar + changeInValues.at(i);
		}
		double cvarPercentile = cvar / (double)(((100 - percentile)*0.01)*changeInValues.size());
	
		cout << "VaR at the 95th percentile is " << var95Percentile << endl;
		cout << "VaR at the 99th percentile is " << var99Percentile << endl;
		cout << "VaR at the desired percentile is " << cvarPercentile << endl;


	}
	catch (const exception &e)
	{
		cerr << "error: " << e.what() << "\n";
	}
	getchar();
}