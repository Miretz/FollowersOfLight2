#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <iostream>
#include <random>

class RandomGenerator
{
private:
	std::mt19937 gen{ std::random_device{}() };

public:
	RandomGenerator() = default;
	virtual ~RandomGenerator(void) = default;

	bool randomChance(float chance);
	float randomFloat(float min, float max);
};


#endif // RANDOMGENERATOR_H
