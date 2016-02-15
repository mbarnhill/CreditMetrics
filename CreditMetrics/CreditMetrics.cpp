// CreditMetrics.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

template <class R> class CSV
{
	public:
		CSV(const string& filename, bool header)
		{
			ifstream file(filename);
			if (!file.good())
			{
				throw runtime_error("File does not exist");
			}
			bool skip = true;
			while (!file.eof())
			{
				string strline;
				getline(file, strline);
				if (skip)
				{
					skip = false;
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
class Issuer
{
	public: 
		Issuer(vector<string>& cells):
			name(cells.at(0)),
			rating(cells.at(1)),
			industry(cells.at(2))
		{}
		const string name;
		const string rating;
		const string industry;
};
class Issuers : public CSV<Issuer>
{
	public:
		Issuers() : CSV("issuers.csv", true) { }
		Issuer* getByName(string name)
		{
			for (int i = 0, n = size(); i < n; i++)
			{
				Issuer& issuer = get(i);
				if(issuer.name == name)
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
		for (int i = 0, n = issuers.size(); i < n; i++)
		{
			Issuer issuer = issuers.get(i);
			cout << issuer.name << "," << issuer.rating << "," << issuer.industry << "\n";
		}
	}
	catch (const exception &e)
	{
		cerr << "error: " << e.what() << "\n";
	}
	getchar();
}