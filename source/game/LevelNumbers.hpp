#pragma once
#include <unordered_map>

namespace game
{
    class LevelNumbers
    {
    public:
        LevelNumbers();
        std::vector<std::string> GetLevelNumbers(unsigned int level);

    private:
        std::unordered_map<unsigned int, std::vector<std::string>> m_levelNumbers;
    };
}