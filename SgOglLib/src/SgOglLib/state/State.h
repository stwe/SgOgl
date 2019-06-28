#pragma once

#include <memory>
#include "Core.h"

namespace sg::ogl
{
    class Window;
}

namespace sg::ogl::resource
{
    class ShaderManager;
    class TextureManager;
}

namespace sg::ogl::state
{
    /**
     * @brief All States have a unique Id.
     */
    enum StateId
    {
        NONE,
        TITLE,
        MENU,
        GAME,
        LOADING,
        PAUSE
    };

    class StateStack;

    class SG_OGL_API State
    {
    public:
        using StateUniquePtr = std::unique_ptr<State>;

        /**
         * @brief Struct to hold shared objects between all States.
         *        Every State will have access to a GetContext() method.
         */
        struct Context
        {
            Context() = delete;

            Context(Window& t_window, resource::ShaderManager& t_shaderManager, resource::TextureManager& t_textureManager);

            Window* window{ nullptr };
            resource::ShaderManager* shaderManager{ nullptr };
            resource::TextureManager* textureManager{ nullptr };
        };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        State() = delete;

        State(StateStack& t_stateStack, Context& t_context);

        State(const State& t_other) = delete;
        State(State&& t_other) noexcept = delete;
        State& operator=(const State& t_other) = delete;
        State& operator=(State&& t_other) noexcept = delete;

        virtual ~State() noexcept = default;

        //-------------------------------------------------
        // State logic
        //-------------------------------------------------

        virtual bool Input() = 0;
        virtual bool Update(float t_dt) = 0;
        virtual void Render() = 0;

    protected:
        //-------------------------------------------------
        // Operations
        //-------------------------------------------------

        void RequestStackPush(StateId t_stateId) const;
        void RequestStackPop() const;
        void RequestStateClear() const;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        Context GetContext() const;

    private:
        /**
         * @brief Pointer to the parent StateStack.
         */
        StateStack* m_stateStack{ nullptr };

        /**
         * @brief Holder of shared objects.
         */
        Context m_context;
    };
}
