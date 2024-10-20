#pragma once

#include <schism/System/Debug.h>
#include <schism/System/Defines.h>

#include <functional>
#include <atomic>
#include <mutex>
#include <vector>

namespace Schism::Util {

class SignalConnection;

template <typename T, bool Sync = true>
class Signal {
        using func = std::function<T>;

        struct FuncHandle {
                u64 id{0};
                func f;
        };

    public:
        ~Signal() {}

        SignalConnection connect(func&& callback);

        void operator()() {
            for (auto const& handle : m_funcs) {
                handle.f();
            }
        }

        template <typename... Args>
        void operator()(Args&... args) {
            doOperation([&] {
                for (auto const& handle : m_funcs) {
                    handle.f(std::forward<Args>(args)...);
                }
            });
        }

        template <typename... Args>
        void operator()(Args&&... args) {
            doOperation([&] {
                for (auto const& handle : m_funcs) {
                    handle.f(std::forward<Args>(args)...);
                }
            });
        }

    private:
        inline void doOperation(std::function<void()>&& operation) {
            if constexpr (Sync) {
                std::lock_guard lck(m_mGuard);
                operation();
            } else {
                operation();
            }
        }

        void CloseConnection(u64 id) {
            // can be extended depending on the use case
            // in small sets this is faster than a map, in bigger ones
            // this could be swapped out with a better container
            // good enough for now
            // TODO: angel: optimize this!
            doOperation([&] {
                std::erase_if(m_funcs, [id](const FuncHandle& handle) {
                    return handle.id == id;
                });
            });
        }

    private:
        std::vector<FuncHandle> m_funcs;
        std::atomic<u64> m_idCounter;
        std::mutex m_mGuard;
};
}  // namespace Schism::Util
