#pragma once
#include <string>

namespace Lang {
    bool zh();
    const std::string& get(const std::string& key);
    int display_width(const std::string& s);
    std::string pad(const std::string& s, int width);
    std::string box(const std::string& name);
    std::string word(const std::string& w);
    std::string opt(const std::string& key);
    std::string tr(const std::string& text);
}
