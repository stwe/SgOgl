#pragma once

#include <variant>
#include <queue>

struct GLFWwindow;

namespace sg::ogl::event
{
    //-------------------------------------------------
    // Types
    //-------------------------------------------------

    enum class QueueResult
    {
        SUCCESS,
        BUFFER_OVERFLOW,
        BUFFER_UNDERFLOW
    };

    enum class EventType
    {
        // Default
        NONE,

        // Keyboard input
        KEY_PRESSED,
        KEY_REPEATED,
        KEY_RELEASED,

        // Mouse input
        BUTTON_PRESSED,
        BUTTON_RELEASED,
        CURSOR_MOVED,
        CURSOR_ENTERED,
        CURSOR_LEFT,
        SCROLLED,

        // Window events
        WINDOW_MOVED,
        WINDOW_RESIZED,
        WINDOW_CLOSED,
        WINDOW_REFRESH,
        WINDOW_FOCUSED,
        WINDOW_DEFOCUSED,
        WINDOW_ICONIFIED,
        WINDOW_UNICONIFIED,

        // Framebuffer size
        FRAMEBUFFER_RESIZED,
    };

    //-------------------------------------------------
    // Event Categories
    //-------------------------------------------------

    /**
     * @brief Using in `glfwSetWindowPosCallback`.
     */
    struct PositionCategory
    {
        EventType eventType{ EventType::NONE };
        GLFWwindow* window{ nullptr };
        int xPos{ 0 };
        int yPos{ 0 };
    };

    /**
     * @brief Using in `glfwSetWindowSizeCallback` and `glfwSetFramebufferSizeCallback`.
     */
    struct SizeCategory
    {
        EventType eventType{ EventType::NONE };
        GLFWwindow* window{ nullptr };
        int width{ 0 };
        int height{ 0 };
    };

    /**
     * @brief Using in `glfwSetWindowCloseCallback`, `glfwSetWindowRefreshCallback`,
     *                 `glfwSetWindowFocusCallback`, `glfwSetWindowIconifyCallback` and
     *                 `glfwSetCursorEnterCallback`.
     */
    struct SwitchCategory
    {
        EventType eventType{ EventType::NONE };
        GLFWwindow* window{ nullptr };
        int value{ 0 }; // GLFW_TRUE = 1 or GLFW_FALSE = 0
    };

    /**
     * @brief Using in `glfwSetCursorPosCallback` and `glfwSetScrollCallback`.
     */
    struct UseDeviceCategory
    {
        EventType eventType{ EventType::NONE };
        GLFWwindow* window{ nullptr };
        double xPos{ 0.0 };
        double yPos{ 0.0 };
    };

    /**
     * @brief Using in `glfwSetMouseButtonCallback`.
     */
    struct MouseCategory
    {
        EventType eventType{ EventType::NONE };
        GLFWwindow* window{ nullptr };
        int button{ 0 };
        int mods{ 0 };
    };

    /**
     * @brief Using in `glfwSetKeyCallback`.
     */
    struct KeyboardCategory
    {
        EventType eventType{ EventType::NONE };
        GLFWwindow* window{ nullptr };
        int key{ 0 };
        int scancode{ 0 };
        int mods{ 0 };
    };

    //-------------------------------------------------
    // Union && Visitor
    //-------------------------------------------------

    using Event = std::variant<
        PositionCategory,
        SizeCategory,
        SwitchCategory,
        UseDeviceCategory,
        MouseCategory,
        KeyboardCategory
    >;

    template<typename ...Ts>
    struct EventVisitor : Ts...
    {
        EventVisitor(const Ts& ... t_args)
            : Ts{ t_args }...
        {
        }

        using Ts::operator()...;
    };

    //-------------------------------------------------
    // Circular Event Queue
    //-------------------------------------------------

    class CircularEventQueue
    {
    public:
        CircularEventQueue() = delete;

        explicit CircularEventQueue(GLFWwindow* t_glfWwindow, size_t t_maxSize = 1024);

        CircularEventQueue(const CircularEventQueue& t_other) = delete;
        CircularEventQueue(CircularEventQueue&& t_other) noexcept = delete;
        CircularEventQueue& operator=(const CircularEventQueue& t_other) = delete;
        CircularEventQueue& operator=(CircularEventQueue&& t_other) noexcept = delete;

        ~CircularEventQueue() noexcept;

        /**
         * @brief The front of the queue is removed. If the queue is empty
         *        return a BUFFER_UNDERFLOW result.
         * @return QueueResult
         */
        template <typename ...T>
        QueueResult Update(EventVisitor<T...> t_eventVisitor)
        {
            if (m_events.empty())
            {
                return QueueResult::BUFFER_UNDERFLOW;
            }

            // visit
            std::visit(t_eventVisitor, m_events.front());

            // remove the "oldest" element in the queue
            m_events.pop();

            return QueueResult::SUCCESS;
        }

        /**
         * @brief Replace all callbacks.
         */
        void SetCallbacks() const;

    protected:

    private:
        GLFWwindow* m_glfWwindow{ nullptr };

        size_t m_maxSize{ 0 };
        std::queue<Event> m_events;

        /**
         * @brief Insert a new event at the end of the queue. If the queue is
         *        full return a BUFFER_OVERFLOW result and leave the queue
         *        unchanged.
         * @tparam T The event category type.
         * @param t_event The event category object to insert.
         * @return QueueResult
         */
        template <typename T>
        QueueResult Add(const T& t_event)
        {
            if (m_events.size() >= m_maxSize)
            {
                return QueueResult::BUFFER_OVERFLOW;
            }

            m_events.push(t_event);

            return QueueResult::SUCCESS;
        }
    };
}
