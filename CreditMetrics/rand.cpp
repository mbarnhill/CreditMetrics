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