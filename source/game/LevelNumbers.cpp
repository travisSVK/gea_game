#include "LevelNumbers.hpp"

LevelNumbers::LevelNumbers()
{
    std::vector<std::string> levels;
    levels.reserve(2);

    levels.push_back(std::string("data/sprite/building/numbers/left3.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right0.png"));
    m_levelNumbers[30] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left2.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right9.png"));
    m_levelNumbers[29] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left2.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right8.png"));
    m_levelNumbers[28] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left2.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right7.png"));
    m_levelNumbers[27] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left2.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right6.png"));
    m_levelNumbers[26] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left2.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right5.png"));
    m_levelNumbers[25] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left2.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right4.png"));
    m_levelNumbers[24] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left2.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right3.png"));
    m_levelNumbers[23] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left2.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right2.png"));
    m_levelNumbers[22] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left2.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right1.png"));
    m_levelNumbers[21] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left2.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right0.png"));
    m_levelNumbers[20] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left1.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right9.png"));
    m_levelNumbers[19] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left1.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right8.png"));
    m_levelNumbers[18] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left1.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right7.png"));
    m_levelNumbers[17] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left1.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right6.png"));
    m_levelNumbers[16] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left1.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right5.png"));
    m_levelNumbers[15] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left1.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right4.png"));
    m_levelNumbers[14] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left1.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right3.png"));
    m_levelNumbers[13] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left1.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right2.png"));
    m_levelNumbers[12] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left1.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right1.png"));
    m_levelNumbers[11] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/left1.png"));
    levels.push_back(std::string("data/sprite/building/numbers/right0.png"));
    m_levelNumbers[10] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/right9.png"));
    m_levelNumbers[9] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/right8.png"));
    m_levelNumbers[8] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/right7.png"));
    m_levelNumbers[7] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/right6.png"));
    m_levelNumbers[6] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/right5.png"));
    m_levelNumbers[5] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/right4.png"));
    m_levelNumbers[4] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/right3.png"));
    m_levelNumbers[3] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/right2.png"));
    m_levelNumbers[2] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/right1.png"));
    m_levelNumbers[1] = levels;
    levels.clear();

    levels.push_back(std::string("data/sprite/building/numbers/right0.png"));
    m_levelNumbers[0] = levels;
}

std::vector<std::string> LevelNumbers::GetLevelNumbers(unsigned int level)
{
    return m_levelNumbers[level];
}