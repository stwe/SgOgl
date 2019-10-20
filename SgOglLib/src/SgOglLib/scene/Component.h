#pragma once

#include "Core.h"

namespace sg::ogl::scene
{
    class Entity;

    class SG_OGL_API Component
    {
    public:
        enum class Type
        {
            RENDERER,
            TERRAIN,
            WATER
        };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Component() = default;

        Component(const Component& t_other) = delete;
        Component(Component&& t_other) noexcept = delete;
        Component& operator=(const Component& t_other) = delete;
        Component& operator=(Component&& t_other) noexcept = delete;

        virtual ~Component() noexcept = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        Entity* GetParentEntity() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetParentEntity(Entity* t_entity);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        virtual void Init() {}
        virtual void Input() {}
        virtual void Update() {}
        virtual void Render() {}

    protected:
        Entity* m_parentEntity{ nullptr };

    private:

    };
}
