#include "rand.h"

UniformRandomNumberGenerator::UniformRandomNumberGenerator(double min, double max) :
	gen(random_device()()),
	dis(min, max) { }
double UniformRandomNumberGenerator::rand()
{
	return dis(gen);
}
NormalRandomNumberGenerator::NormalRandomNumberGenerator(double mean, double stdev) :
	gen(random_device()()),
	dis(mean, stdev) { }
double NormalRandomNumberGenerator::rand()
{
	return dis(gen);
}
BetaRandomNumberGenerator::BetaRandomNumberGenerator(double alpha, double beta) :
	uniformRand(0, 1),
	dis(alpha, beta) { }
double BetaRandomNumberGenerator::rand()
{
	return boost::math::quantile(this->dis, this->uniformRand.rand());
}
