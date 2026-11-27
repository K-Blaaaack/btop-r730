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

std::string Lang::opt(const std::string& key) {
    if (!zh()) return "";
    static const std::unordered_map<std::string, std::string> opts = {
        {"color_theme","主题"},{"language","语言"},{"update_ms","刷新间隔"},
        {"temp_scale","温度单位"},{"clock_format","时钟格式"},
        {"show_uptime","显示运行时间"},{"show_cpu_watts","显示 CPU 功耗"},
        {"show_cpu_graph","显示 CPU 曲线"},{"show_server_panel","显示 iDRAC 面板"},
        {"cpu_graph_upper","CPU 上图字段"},{"cpu_graph_lower","CPU 下图字段"},
        {"cpu_sensor","CPU 温度传感器"},{"cpu_invert_lower","下图反转"},
        {"cpu_single_graph","只显示单图"},{"cpu_bottom","CPU 盒子置底"},
        {"show_temps","显示温度"},{"show_coretemp","显示核心温度"},
        {"graph_symbol","图形符号"},{"graph_symbol_cpu","CPU 图形符号"},
        {"graph_symbol_mem","内存图形符号"},{"graph_symbol_net","网络图形符号"},
        {"graph_symbol_proc","进程图形符号"},{"proc_sorting","进程排序"},
        {"proc_reversed","进程反向排序"},{"proc_tree","进程树"},
        {"proc_colors","进程着色"},{"proc_gradient","进程渐变"},
        {"proc_per_core","按核心显示进程"},{"proc_mem_bytes","内存按字节"},
        {"proc_cpu_graphs","进程 CPU 曲线"},{"proc_filter_kernel","过滤内核线程"},
        {"show_disks","显示磁盘"},{"only_physical","仅物理磁盘"},
        {"use_fstab","使用 fstab"},{"zfs_hide_datasets","隐藏 ZFS 数据集"},
        {"disk_free_priv","特权可用空间"},{"disks_filter","磁盘过滤"},
        {"show_io_stat","显示 IO 统计"},{"io_mode","IO 模式"},
        {"io_graph_combined","IO 合并曲线"},{"io_graph_speeds","IO 曲线上限"},
        {"mem_graphs","内存曲线"},{"mem_below_net","内存置于网络下"},
        {"show_swap","显示交换"},{"swap_disk","交换作为磁盘"},
        {"show_battery","显示电池"},{"selected_battery","选择电池"},
        {"show_battery_watts","显示电池功率"},{"net_download","下载上限"},
        {"net_upload","上传上限"},{"net_auto","网络自动缩放"},
        {"net_sync","网络同步缩放"},{"net_iface","网络接口"},
        {"swap_upload_download","交换上传下载"},{"base_10_sizes","十进制单位"},
        {"base_10_bitrate","十进制速率"},{"vim_keys","Vim 键位"},
        {"tty_mode","TTY 模式"},{"low_color","低色彩"},{"force_utf","强制 UTF"},
        {"theme_background","主题背景"},{"truecolor","真彩色"},
        {"rounded_corners","圆角"},{"terminal_sync","终端同步"},
        {"shown_boxes","显示盒子"},{"presets","预设"},{"disable_presets","禁用预设"},
        {"log_level","日志级别"},{"save_config_on_exit","退出时保存配置"},
        {"background_update","后台更新"},{"check_temp","检查温度"},
        {"custom_cpu_name","自定义 CPU 名称"},{"freq_mode","频率模式"},
        {"show_gpu_info","显示 GPU 信息"},{"nvml_measure_pcie_speeds","NVIDIA PCIe 测速"},
        {"rsmi_measure_pcie_speeds","AMD PCIe 测速"},{"gpu_mirror_graph","GPU 曲线镜像"},
        {"shown_gpus","显示 GPU"},{"gpu_refresh_rate","GPU 刷新率"}
    };
    auto it = opts.find(key);
    return it == opts.end() ? "" : it->second;
}

std::string Lang::tr(const std::string& text) {
    return text;
}

std::string Lang::pad(const std::string& s, int width) {
    const int w = display_width(s);
    return s + std::string(w < width ? width - w : 0, ' ');
}
