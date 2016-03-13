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
class NormalRandomNumberGenerator
{
public:
	NormalRandomNumberGenerator(double mean, double stdev);
	double rand();
private:
	mt19937 gen;
	normal_distribution<> dis;
};
#endif