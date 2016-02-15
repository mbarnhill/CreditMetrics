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
using namespace std;

static inline string& trim(string& s) {
	//left
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	//right
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
	return s;
}

template <class R> class CSV
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
				rows.push_back(R(cells));
			}
		}
		size_t size() { return rows.size(); }
		R& get(size_t i) { return rows[i]; }
	private:
		vector<R> rows;
};
class IssuerRating
{
	public: 
		IssuerRating(vector<string>& cells):
			name(cells.at(0)),
			rating(cells.at(1)),
			industry(cells.at(2))
		{}
		const string name;
		const string rating;
		const string industry;
};
class Issuers : public CSV<IssuerRating>
{
	public:
		Issuers() : CSV("issuers.csv", 1) { }
		IssuerRating* getByName(string name)
		{
			for (size_t i = 0, n = size(); i < n; i++)
			{
				IssuerRating& issuer = get(i);
				if(issuer.name == name)
					return &issuer;
			}
			return nullptr;
		}
};
class PortfolioEntry
{
	public:
		PortfolioEntry(vector<string>& cells) :
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
		const string name;
		const string instrumentType;
		const string cusip;
		const int notional;
		const string maturity;
		const double coupon;
		const int couponsPerYear;
		const double price;
		const double yield;
		const double cleanPrice;
		const double exprr;
	private:
		int convertInt(string strInt)
		{
			trim(strInt);
			return ::atoi(strInt.c_str());
		}
		double convertDouble(string strDouble)
		{
			trim(strDouble);
			return ::atof(strDouble.c_str());
		}
		int convertNotional(string strNotional)
		{
			return convertInt(strNotional) * 1000000;
		}
		double convertPercent(string strPercent)
		{
			replace(strPercent.begin(), strPercent.end(), '%', ' ');
			return convertDouble(strPercent) / 100;
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
				PortfolioEntry& issuer = get(i);
				if (issuer.name == name)
					return &issuer;
			}
			return nullptr;
		}
};
int main(int argc, char* argv[])
{
	try
	{
		Issuers issuers;
		for (size_t i = 0, n = issuers.size(); i < n; i++)
		{
			IssuerRating issuer = issuers.get(i);
			cout << issuer.name << "," << issuer.rating << "," << issuer.industry << "\n";
		}
		PortfolioData portfolioData;
		for (size_t i = 0, n = portfolioData.size(); i < n; i++)
		{
			PortfolioEntry entry = portfolioData.get(i);
			cout << entry.name << "," << entry.cleanPrice << "\n";
		}
	}
	catch (const exception &e)
	{
		cerr << "error: " << e.what() << "\n";
	}
	getchar();
}