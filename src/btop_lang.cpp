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

std::vector<std::string> Lang::desc(const std::string& key) {
    if (!zh()) return {};
    static const std::unordered_map<std::string, std::vector<std::string>> d = {
        {"color_theme", {"选择界面配色主题。"}},
        {"theme_background", {"使用主题自带的背景色。"}},
        {"truecolor", {"强制 24 位真彩色输出。"}},
        {"force_tty", {"TTY 模式：仅用 3 种字符绘制曲线。"}},
        {"vim_keys", {"启用 Vim 风格按键（hjkl 等）。"}},
        {"disable_mouse", {"关闭鼠标操作。"}},
        {"disable_presets", {"禁用默认预设 / 自定义预设。"}},
        {"presets", {"定义 0-9 号布局预设。"}},
        {"shown_boxes", {"选择显示的盒子与排列方式。"}},
        {"update_ms", {"界面刷新间隔（毫秒），建议 2000 以上。"}},
        {"rounded_corners", {"盒子边框使用圆角。"}},
        {"terminal_sync", {"减少终端闪烁，需终端支持同步输出。"}},
        {"graph_symbol", {"全局曲线符号：braille / block / tty。"}},
        {"clock_format", {"顶部时钟的 strftime 格式，留空则关闭。"}},
        {"base_10_sizes", {"使用十进制单位（KB=1000）。"}},
        {"background_update", {"菜单打开时仍刷新主界面。"}},
        {"show_battery", {"显示电池 / UPS 状态。"}},
        {"selected_battery", {"多电池时选择显示哪一个。"}},
        {"show_battery_watts", {"显示充电 / 放电功率。"}},
        {"log_level", {"日志级别：ERROR / WARNING / INFO / DEBUG。"}},
        {"save_config_on_exit", {"退出时自动保存配置。"}},
        {"cpu_bottom", {"CPU 盒子放在屏幕底部。"}},
        {"graph_symbol_cpu", {"CPU 曲线使用的符号。"}},
        {"cpu_graph_upper", {"CPU 曲线上半部分显示的指标。"}},
        {"cpu_graph_lower", {"CPU 曲线下半部分显示的指标。"}},
        {"cpu_invert_lower", {"反转下半部分曲线。"}},
        {"cpu_single_graph", {"关闭下半部分曲线，只保留一张。"}},
        {"show_gpu_info", {"GPU 盒子显示方式：On / Auto / Off。"}},
        {"check_temp", {"读取并显示温度。"}},
        {"cpu_sensor", {"选择 CPU 温度来源。"}},
        {"show_coretemp", {"显示每个核心的温度。"}},
        {"cpu_core_map", {"自定义核心顺序映射。"}},
        {"language", {"界面语言：en（英文）或 zh（中文）。"}},
        {"temp_scale", {"温度单位：摄氏 / 华氏 / 开尔文 / 兰氏。"}},
        {"show_cpu_freq", {"显示 CPU 频率。"}},
        {"freq_mode", {"频率统计方式：首核 / 范围 / 最低 / 最高 / 平均。"}},
        {"custom_cpu_name", {"CPU 盒子显示的型号名，留空则自动。"}},
        {"show_uptime", {"在 CPU 盒显示运行时间。"}},
        {"show_cpu_watts", {"显示 CPU 封装功耗（瓦）。"}},
        {"nvml_measure_pcie_speeds", {"测量 NVIDIA 显卡 PCIe 带宽。"}},
        {"rsmi_measure_pcie_speeds", {"测量 AMD 显卡 PCIe 带宽。"}},
        {"graph_symbol_gpu", {"GPU 曲线使用的符号。"}},
        {"gpu_mirror_graph", {"水平镜像 GPU 曲线。"}},
        {"shown_gpus", {"手动选择要显示的 GPU。"}},
        {"mem_below_net", {"内存盒子放在网络盒子下方。"}},
        {"graph_symbol_mem", {"内存曲线使用的符号。"}},
        {"mem_graphs", {"用曲线代替进度条显示内存。"}},
        {"show_disks", {"显示磁盘使用情况。"}},
        {"show_io_stat", {"显示磁盘 IO 统计。"}},
        {"io_mode", {"磁盘盒子显示大 IO 曲线。"}},
        {"io_graph_combined", {"读写合并成一张图。"}},
        {"io_graph_speeds", {"设置各挂载点曲线满量程。"}},
        {"show_swap", {"显示 swap 使用情况。"}},
        {"swap_disk", {"把 swap 当作一行磁盘显示。"}},
        {"only_physical", {"隐藏网络盘、内存盘等非物理磁盘。"}},
        {"use_fstab", {"从 /etc/fstab 读取磁盘列表。"}},
        {"zfs_hide_datasets", {"隐藏 ZFS 数据集，只显示池。"}},
        {"disk_free_priv", {"显示 root 可用空间。"}},
        {"disks_filter", {"按挂载点过滤磁盘，可用 exclude=。"}},
        {"zfs_arc_cached", {"把 ZFS ARC 计入缓存内存。"}},
        {"graph_symbol_net", {"网络曲线使用的符号。"}},
        {"swap_upload_download", {"交换上传 / 下载曲线位置。"}},
        {"net_download", {"下载曲线满量程（Mib）。"}},
        {"net_upload", {"上传曲线满量程（Mib）。"}},
        {"net_auto", {"自动调整网络曲线量程。"}},
        {"net_sync", {"上传下载共用同一量程。"}},
        {"net_iface", {"选择显示的网络接口。"}},
        {"base_10_bitrate", {"速率使用十进制（1000）。"}},
        {"proc_left", {"进程盒子放在屏幕左侧。"}},
        {"graph_symbol_proc", {"进程 CPU 曲线使用的符号。"}},
        {"proc_sorting", {"进程排序字段：cpu / mem / pid 等。"}},
        {"proc_reversed", {"反向排序进程列表。"}},
        {"proc_tree", {"以树形显示进程。"}},
        {"proc_aggregate", {"合并同名进程。"}},
        {"proc_colors", {"按 CPU 使用率给进程着色。"}},
        {"proc_gradient", {"着色使用渐变。"}},
        {"proc_per_core", {"按核心分别统计进程。"}},
        {"proc_mem_bytes", {"内存以字节显示。"}},
        {"keep_dead_proc_usage", {"保留已退出进程的最后占用。"}},
        {"proc_cpu_graphs", {"每个进程显示 CPU 曲线。"}},
        {"proc_filter_kernel", {"隐藏内核线程。"}},
        {"proc_follow_detailed", {"详细视图跟随选中进程。"}},
        {"custom_gpu_name0", {"覆盖第 0 块 GPU 的显示名称。"}},
        {"custom_gpu_name1", {"覆盖第 1 块 GPU 的显示名称。"}},
        {"custom_gpu_name2", {"覆盖第 2 块 GPU 的显示名称。"}},
        {"custom_gpu_name3", {"覆盖第 3 块 GPU 的显示名称。"}},
        {"custom_gpu_name4", {"覆盖第 4 块 GPU 的显示名称。"}},
        {"custom_gpu_name5", {"覆盖第 5 块 GPU 的显示名称。"}},
    };
    auto it = d.find(key);
    return it == d.end() ? std::vector<std::string>{} : it->second;
}

std::string Lang::pad(const std::string& s, int width) {
    const int w = display_width(s);
    return s + std::string(w < width ? width - w : 0, ' ');
}
