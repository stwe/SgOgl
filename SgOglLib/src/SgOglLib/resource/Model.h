#pragma once

#include <vector>
#include <memory>

namespace sg::ogl::resource
{
    class Mesh;

    class Model
    {
    public:
        using MeshUniquePtr = std::unique_ptr<Mesh>;
        using Meshes = std::vector<MeshUniquePtr>;

    protected:

    private:
        Meshes m_meshes;
    };
}
