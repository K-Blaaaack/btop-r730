#include "btop_lang.hpp"
#include "btop_config.hpp"
#include <unordered_map>
#include <utility>

namespace {
const std::unordered_map<std::string, std::pair<const char*, const char*>> dict = {
    {"load_avg",   {"Load avg:",  "负载："}},
    {"uptime",     {"up",         "运行"}},
    {"sys_pwr",    {"SYS PWR",    "整机功耗"}},
    {"temp",       {"TEMP",       "温度"}},
    {"cpu12",      {"CPU1/2",     "CPU1/2"}},
    {"fan13",      {"FAN1-3",     "风扇1-3"}},
    {"fan46",      {"FAN4-6",     "风扇4-6"}},
    {"psu",        {"PSU",        "电源"}},
    {"sel_intr",   {"SEL/INTR",   "日志/入侵"}},
    {"fw",         {"FW",         "固件"}},
    {"model",      {"MODEL",      "型号"}},
};
}

bool Lang::zh() {
    return Config::getS("language") == "zh";
}

const std::string& Lang::get(const std::string& key) {
    static std::string value;
    auto it = dict.find(key);
    if (it == dict.end()) { value.clear(); return value; }
    value = zh() ? it->second.second : it->second.first;
    return value;
}

int Lang::display_width(const std::string& s) {
    int w = 0;
    for (unsigned char c : s) {
        if ((c & 0xC0) == 0x80) continue;      // UTF-8 continuation
        w += (c >= 0xE0) ? 2 : 1;              // 3-byte CJK ~ 2 columns
    }
    return w;
}

std::string Lang::box(const std::string& name) {
    if (!zh()) return name;
    static const std::unordered_map<std::string, std::string> boxes = {
        {"cpu", "CPU"}, {"mem", "内存"}, {"disks", "磁盘"},
        {"net", "网络"}, {"proc", "进程"}, {"gpu", "显卡"}
    };
    auto it = boxes.find(name);
    return it == boxes.end() ? name : it->second;
}

std::string Lang::word(const std::string& w) {
    if (!zh()) return w;
    static const std::unordered_map<std::string, std::string> words = {
        {"Used", "已用"}, {"Free", "空闲"}, {"Available", "可用"},
        {"Cached", "缓存"}, {"Total", "总计"}, {"Swap", "交换"},
        {"Filter", "过滤"}, {"Download", "下载"}, {"Upload", "上传"}
    };
    auto it = words.find(w);
    return it == words.end() ? w : it->second;
}

std::string Lang::pad(const std::string& s, int width) {
    const int w = display_width(s);
    return s + std::string(w < width ? width - w : 0, ' ');
}
