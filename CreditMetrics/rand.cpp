#include "rand.h"

UniformRandomNumberGenerator::UniformRandomNumberGenerator(double min, double max) :
	gen(random_device()()),
	dis(min, max) { }
double UniformRandomNumberGenerator::rand()
{
	return dis(gen);
}