#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Core.h"

namespace sg::ogl::resource
{
    class TextureManager;
    class Mesh;

    class SG_OGL_API Skybox
    {
    public:
        using MeshUniquePtr = std::unique_ptr<Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Skybox() = delete;

        Skybox(TextureManager& t_textureManager, const std::vector<std::string>& t_textureFileNames, float t_size = 500.0f);

        Skybox(const Skybox& t_other) = delete;
        Skybox(Skybox&& t_other) noexcept = delete;
        Skybox& operator=(const Skybox& t_other) = delete;
        Skybox& operator=(Skybox&& t_other) noexcept = delete;

        ~Skybox() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        uint32_t GetCubemapId() const;

        MeshUniquePtr& GetMesh() noexcept;
        const MeshUniquePtr& GetMesh() const noexcept;

    protected:

    private:
        uint32_t m_cubemapId{ 0 };
        MeshUniquePtr m_mesh;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static std::vector<glm::vec3> CreateSkyboxVertices(float t_size);
    };
}
