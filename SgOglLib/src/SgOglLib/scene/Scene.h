#pragma once

#include <string>

namespace sg::ogl::scene
{
    class Renderer;

    class Scene
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Scene() = delete;

        explicit Scene(std::string t_name, Renderer* t_renderer = nullptr);

        Scene(const Scene& t_other) = delete;
        Scene(Scene&& t_other) noexcept = delete;
        Scene& operator=(const Scene& t_other) = delete;
        Scene& operator=(Scene&& t_other) noexcept = delete;

        ~Scene() = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        Renderer* GetRenderer() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetRenderer(Renderer* t_renderer);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Render();

    protected:

    private:
        std::string m_name;
        Renderer* m_renderer{ nullptr };
    };
}
