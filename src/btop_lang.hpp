#pragma once
#include <string>

namespace Lang {
    bool zh();
    const std::string& get(const std::string& key);
    int display_width(const std::string& s);
    std::string pad(const std::string& s, int width);
}
