#include <schism/Util/Signal.h>
#include <schism/Util/SignalConnection.h>

namespace Schism::Util {

template <typename T, bool Sync>
SignalConnection Signal<T, Sync>::connect(Signal<T, Sync>::func&& callback) {
    auto id = m_idCounter.fetch_add(1);

    doSyncOperation([&] { m_funcs.emplace_back(id, callback); });

    auto close_func = [this](u64 id) {
        CloseConnection(id);
    };

    return {id, close_func};
}

}  // namespace Schism::Util
