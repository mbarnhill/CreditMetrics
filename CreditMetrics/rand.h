#ifndef __rand_h__
#define __rand_h__

#include <random>
#include <boost/math/distributions.hpp>

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
class BetaRandomNumberGenerator
{
public:
	BetaRandomNumberGenerator(double alpha, double beta);
	double rand();
private:
	UniformRandomNumberGenerator uniformRand;
	boost::math::beta_distribution<> dis;
};
#endif