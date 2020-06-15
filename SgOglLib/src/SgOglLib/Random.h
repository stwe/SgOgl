// This file is part of the SgOgl package.
// 
// Filename: Random.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

namespace sg::ogl
{
    class Random
    {
    public:
        static float Float(float t_min = 0.0f, float t_max = 1.0f);
        static int Int(int t_min = 0, int t_max = std::numeric_limits<int32_t>::max());

    protected:

    private:
    };
}
