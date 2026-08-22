
#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <string_view>

namespace lnpu
{

struct string_hash
{
    using is_transparent = void; // 이게 핵심

    size_t operator()(std::string_view sv) const noexcept
    {
        return std::hash<std::string_view>{}(sv);
    }
    size_t operator()(const std::string& s) const noexcept
    {
        return std::hash<std::string_view>{}(s);
    }
    size_t operator()(const char* s) const noexcept { return std::hash<std::string_view>{}(s); }
};

struct string_equal
{
    using is_transparent = void;

    bool operator()(std::string_view a, std::string_view b) const noexcept { return a == b; }
};

} // namespace lnpu
