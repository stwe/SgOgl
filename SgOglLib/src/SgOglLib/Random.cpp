// This file is part of the SgOgl package.
// 
// Filename: Random.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include <random>
#include "Random.h"

static std::random_device rd;
static std::mt19937 generator(rd());

float sg::ogl::Random::Float(const float t_min, const float t_max)
{
    const std::uniform_real_distribution<float> distr(t_min, t_max);

    return distr(generator);
}

int sg::ogl::Random::Int(const int t_min, const int t_max)
{
    const std::uniform_int_distribution<int> distr(t_min, t_max);

    return distr(generator);
}
