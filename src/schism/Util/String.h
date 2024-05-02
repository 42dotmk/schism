#pragma once

#include <algorithm>
#include <cctype>
#include <string>

namespace Schism::Util
{
    static constexpr inline void StrToLower(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(),
                [](auto c)
                {
                    return std::tolower(c);
                });
    }
}
