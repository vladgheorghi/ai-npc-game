#include "rng.h"

#include <random>

namespace ai_npc {
    static std::mt19937 rng(std::random_device{}());

    float randFloat(float min, float max) {
        return std::uniform_real_distribution<float>(min, max)(rng);
    }

    int randInt(int min, int max) {
        return std::uniform_int_distribution<int>(min, max)(rng);
    }
}
