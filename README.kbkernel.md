# btop-kbkernel

btop 1.4.7 的 btop-r730 定制分支，面向 Dell PowerEdge R730 构建节点。

## 相对上游的改动

1. **多路 CPU 功耗合计**：`get_cpuConsumptionUJoules()` 汇总 `intel-rapl:*` 所有 package（按 `max_energy_range_uj` 处理回绕），双路机器显示整机 CPU 功耗而不是单颗。
2. **可选关闭 CPU 占用曲线**：新增 `show_cpu_graph`（默认 true）。
3. **Dell iDRAC 指标面板**：新增 `show_server_panel`（默认 true），在 CPU box 左侧空白区显示 `Fan1~6`、进/排风温度、整机功耗。数据来自 `/run/btop-server-metrics`，由 `scripts/btop-server-metrics.sh`（IPMI）每 2 秒刷新。
4. 风扇转速显示：`/run/btop-server-metrics` 的第二行起由脚本生成，btop 原样绘制。

## 构建

```bash
make -j$(nproc) GPU_SUPPORT=false
sudo install -m755 bin/btop /usr/local/bin/btop
```

依赖：g++ (C++23)、libncursesw、make。

## 采集服务

```bash
sudo install -m755 scripts/btop-server-metrics.sh /usr/local/sbin/
sudo install -m644 scripts/btop-server-metrics.service /etc/systemd/system/
sudo systemctl enable --now btop-server-metrics
```

## 配置

```ini
show_cpu_graph = False
show_server_panel = True
```
