#pragma once

#include <string>

namespace sg::ogl::resource
{
    class SG_OGL_LOCAL ComputeShaderTexture
    {
    public:
        /**
         * @brief The unique name of the texture in the TextureManager.
         */
        std::string uniqueTextureName;

        /**
         * @brief The name of the compute shader.
         */
        std::string computeShaderName;

    protected:

    private:

    };
}
