#ifndef __csv_h__
#define __csv_h__

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

string& trim(string&);
int convertInt(string);
double convertDouble(string);
double convertPercent(string);
string sanitizeString(string);

template <class R> class CSV : public vector<R>
{
public:
	CSV(const string&, size_t);
};

#include "csv.hxx"

#endif