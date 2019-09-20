#pragma once

#include <memory>
#include "Core.h"
#include "Component.h"

namespace sg::ogl::terrain
{
    class Terrain;
}

namespace sg::ogl::scene
{
    class SG_OGL_API TerrainComponent : public Component
    {
    public:
        using TerrainSharedPtr = std::shared_ptr<terrain::Terrain>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TerrainComponent() = delete;

        TerrainComponent(const TerrainComponent& t_other) = delete;
        TerrainComponent(TerrainComponent&& t_other) noexcept = delete;
        TerrainComponent& operator=(const TerrainComponent& t_other) = delete;
        TerrainComponent& operator=(TerrainComponent&& t_other) noexcept = delete;

        explicit TerrainComponent(const TerrainSharedPtr& t_terrain)
            : m_terrain{ t_terrain }
        {}

        ~TerrainComponent() noexcept = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        terrain::Terrain& GetTerrain() const { return *m_terrain; }

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Init() override;

    protected:

    private:
        TerrainSharedPtr m_terrain;
    };
}
