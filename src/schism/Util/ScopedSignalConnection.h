#pragma once

#include <schism/Util/SignalConnection.h>

namespace Schism::Util {

class ScopedSignalConnection : public SignalConnection {
    public:
        ScopedSignalConnection(const SignalConnection& conn)
            : SignalConnection(conn) {}

        ScopedSignalConnection(const SignalConnection&& conn)
            : SignalConnection(conn) {}

        ~ScopedSignalConnection() { disconnect(); }
};
}  // namespace Schism::Util
