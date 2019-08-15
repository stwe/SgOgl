#pragma once

#include "Core.h"

namespace sg::ogl::camera
{
    class LookAtCamera;
}

namespace sg::ogl::scene
{
    class Node;
    class Renderer;

    class SG_OGL_API Scene
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Scene() = delete;

        Scene(Renderer& t_renderer, camera::LookAtCamera& t_camera);

        Scene(const Scene& t_other) = delete;
        Scene(Scene&& t_other) noexcept = delete;
        Scene& operator=(const Scene& t_other) = delete;
        Scene& operator=(Scene&& t_other) noexcept = delete;

        ~Scene() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        Renderer& GetRenderer() noexcept;
        const Renderer& GetRenderer() const noexcept;

        camera::LookAtCamera& GetCamera() noexcept;
        const camera::LookAtCamera& GetCamera() const noexcept;

        //-------------------------------------------------
        // Scene objects
        //-------------------------------------------------

        void AddObject(Node* t_node) const;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update();
        void Render();

    protected:

    private:
        Renderer& m_renderer;
        camera::LookAtCamera& m_camera;

        Node* m_rootNode{ nullptr };
    };
}
