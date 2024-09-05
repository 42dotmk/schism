#pragma once

#include <functional>
#include "schism/System/Debug.h"

namespace Schism::Util {

template <typename T>
class SingleSignal {
        using func = std::function<T>;

    public:
        ~SingleSignal() { disconnect_slot(); }

        void disconnect_slot() { m_func = nullptr; }

        void connect(func&& callback) { m_func = std::move(callback); }

        auto operator()() { return m_func(); }

        template <typename... Args>
        auto operator()(Args&... args) {
            SC_ASSERT(m_func, "Invalid function call, not a valid function");
            return m_func(std::forward<Args>(args)...);
        }

        template <typename... Args>
        auto operator()(Args&&... args) {
            SC_ASSERT(m_func, "Invalid function call, not a valid function");
            return m_func(std::forward<Args>(args)...);
        }

    private:
        func m_func;
};

}  // namespace Schism::Util
