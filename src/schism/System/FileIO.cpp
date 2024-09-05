#include "FileIO.h"

#include <fstream>
#include <sstream>

#include "System.h"

namespace Schism::System {
std::string ReadFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        SC_CORE_TRACE("Cannot open file: {0}", filepath);
    }

    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}
}  // namespace Schism::System