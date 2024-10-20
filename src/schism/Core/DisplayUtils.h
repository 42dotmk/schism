#pragma once

#include <schism/System/Ptr.h>

#include <schism/Core/Display.h>

#include <functional>
#include <schism/Util/SignalConnection.h>

namespace Schism::Core::DisplayUtils {

using DisplayCallback = std::function<void(const Ref<Display>& display)>;

static std::vector<Ref<Display>> ListDisplays();
static Ref<Display> PrimaryDisplay();

static Util::SignalConnection OnDisplayAttached(DisplayCallback&& callback);
static Util::SignalConnection OnDisplayRemoved(DisplayCallback&& callback);

}  // namespace Schism::Core::DisplayUtils
