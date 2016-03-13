#ifndef __rand_h__
#define __rand_h__

#include <random>
using namespace std;
class UniformRandomNumberGenerator
{
public:
	UniformRandomNumberGenerator(double min, double max);
	double rand();
private:
	mt19937 gen;
	uniform_real_distribution<> dis;
};
#endif