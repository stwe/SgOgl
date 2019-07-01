#pragma once

#include <variant>
#include <queue>
#include "EventCategories.h"

namespace sg::ogl::event
{
    enum class QueueResult
    {
        SUCCESS,
        BUFFER_OVERFLOW,
        BUFFER_UNDERFLOW
    };

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

    class CircularEventQueue
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        CircularEventQueue() = delete;

        explicit CircularEventQueue(GLFWwindow* t_glfWwindow, size_t t_maxSize = 1024);

        CircularEventQueue(const CircularEventQueue& t_other) = delete;
        CircularEventQueue(CircularEventQueue&& t_other) noexcept = delete;
        CircularEventQueue& operator=(const CircularEventQueue& t_other) = delete;
        CircularEventQueue& operator=(CircularEventQueue&& t_other) noexcept = delete;

        ~CircularEventQueue() noexcept;

        //-------------------------------------------------
        // Getter && Setter
        //-------------------------------------------------

        /**
         * @brief Replace all callbacks.
         */
        void SetCallbacks() const;

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

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

    protected:

    private:
        GLFWwindow* m_glfWwindow{ nullptr };
        size_t m_maxSize{ 0 };
        std::queue<Event> m_events;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

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
