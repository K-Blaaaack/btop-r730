#!/bin/sh
# btop-r730: 采集 Dell iDRAC 指标，写 /run/btop-server-metrics（btop 左侧面板读取）
# 依赖：ipmitool（root）；服务 btop-server-metrics.service
while :; do
  tmp=/run/btop-server-metrics.tmp
  ipmitool sensor 2>/dev/null | awk -F'|' '
    function trim(s){gsub(/^[ \t]+|[ \t]+$/,"",s); return s}
    /^Fan[1-6]/        { fan[substr(trim($1),4,1)] = int($2) }
    /^Inlet Temp/      { inlet = int($2) }
    /^Exhaust Temp/    { exhaust = int($2) }
    /^Pwr Consumption/ { pwr = int($2) }
    END {
      printf "FAN  %4d %4d %4d   %4d %4d %4d\n", fan[1],fan[2],fan[3],fan[4],fan[5],fan[6]
      printf "TEMP inlet %d°C   exhaust %d°C\n", inlet, exhaust
      printf "PWR  %d W\n", pwr
    }' > "$tmp" 2>/dev/null
  if [ -s "$tmp" ]; then mv -f "$tmp" /run/btop-server-metrics; fi
  sleep 2
done
