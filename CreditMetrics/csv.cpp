#include "csv.h"
using namespace std;

string& trim(string& s)
{
	//left
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	//right
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
	return s;
}
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
double convertPercent(string strPercent)
{
	replace(strPercent.begin(), strPercent.end(), '%', ' ');
	return convertDouble(strPercent) / 100;
}
string sanitizeString(string str)
{
	boost::to_upper(str);
	trim(str);
	return str;
}
