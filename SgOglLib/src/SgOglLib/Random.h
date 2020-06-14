#pragma once

#include <random>

namespace sg::ogl
{
    class Random
    {
    public:
        static void Init()
        {
            m_generator.seed(std::random_device()());
        }

        static float Float()
        {
            return static_cast<float>(m_distribution(m_generator)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
        }

    protected:

    private:
        inline static std::mt19937 m_generator;
        inline static std::uniform_int_distribution<std::mt19937::result_type> m_distribution;
    };
}
