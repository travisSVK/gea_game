#pragma once
#include <random>

class RandomNumberGenerator
{
public:

    RandomNumberGenerator()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
    }

    static uint16_t GenerateRandomNumber(uint16_t min, uint16_t max)
    {
        static RandomNumberGenerator instance;

        std::uniform_int_distribution<> dis(min, max);
        return dis(instance.gen);
    }

private:
    std::mt19937 gen;
};