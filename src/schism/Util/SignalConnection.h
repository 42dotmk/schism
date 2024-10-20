#pragma once

#include <schism/System/Defines.h>

namespace Schism::Util {

// this can be extended to support multiple type and stuff
class SignalConnection {
        using close_func = std::function<void(u64)>;

        class Signal;
        friend Signal;

    private:
        SignalConnection(u64 id, close_func&& closeFunc)
            : m_id(id), m_closeFunc(closeFunc) {}

    public:
        // can be extended later on
        void disconnect() { m_closeFunc(m_id); }

    private:
        close_func m_closeFunc;
        u64 m_id;
};
}  // namespace Schism::Util
