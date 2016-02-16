// CreditMetrics.cpp : Defines the entry point for the console application.
// Note: The given .csv files are not formatted consistently. The .csv parsers below are 
// designed to handle the inconsistencies present in the files in a somewhat 
// general manner by removing extra characters and white space. 
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>
#include <boost/numeric/ublas/matrix.hpp>
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
class IssuerEntry
{
public: 
	IssuerEntry(const vector<string>& cells):
		name(cells.at(0)),
		rating(cells.at(1)),
		industry(cells.at(2))
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
class PortfolioEntry
{
public:
	PortfolioEntry(const vector<string>& cells) :
		name(cells.at(0)),
		instrumentType(cells.at(1)),
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
		return convertInt(strNotional) * 1000000;
	}
	double convertPrice(string strPrice)
	{
		replace(strPrice.begin(), strPrice.end(), '$', ' ');
		return convertDouble(strPrice);
	}
};
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
};
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
class TransitionEntry
{
public:
	TransitionEntry(const vector<string>& cells) :
		aaa(convertPercent(cells.at(1))),
		aa(convertPercent(cells.at(2))),
		a(convertPercent(cells.at(3))),
		bbb(convertPercent(cells.at(4))),
		bb(convertPercent(cells.at(5))),
		b(convertPercent(cells.at(6))),
		ccc(convertPercent(cells.at(7))),
		d(convertPercent(cells.at(8)))
	{}
	const double aaa, aa, a, bbb, bb, b, ccc, d;
	const string toString() const
	{
		return string() 
			+ to_string(aaa) + "," + to_string(aa) + ","
			+ to_string(a) + "," + to_string(bbb) + "," 
			+ to_string(bb) + "," + to_string(b) + "," + to_string(ccc) + "," + to_string(d);
	}
};
class TransitionData : public CSV<TransitionEntry>
{
public:
	TransitionData() : CSV("transition_matrix_for_project.csv", 3) {}
};
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
	string toString()
	{
		string ret;
		for (size_t i = 0, n = size(); i < n; i++)
		{
			ret = ret + to_string(at(i)) + ",";
		}
		return ret;
	}
};
class Matrix : public CSV<MatrixRow>
{
public:
	Matrix() : CSV("correlation_matrix_for_project.csv", 1) {}
};
int main(int argc, char* argv[])
{
	try
	{
		IssuerData issuerData;
		cout << issuerData.toString();
		PortfolioData portfolioData;
		cout << portfolioData.toString();
		YieldData yieldData;
		cout << yieldData.toString();
		TransitionData transitionData;
		cout << transitionData.toString();
		Matrix matrixIn;
		cout << matrixIn.toString();
	}
	catch (const exception &e)
	{
		cerr << "error: " << e.what() << "\n";
	}
	getchar();
}