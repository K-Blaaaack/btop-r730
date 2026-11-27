#!/bin/sh
# btop-r730: 采集 Dell iDRAC 指标，写 /run/btop-server-metrics（btop 左侧面板读取）
while :; do
  tmp=/run/btop-server-metrics.tmp
  {
    ipmitool sensor 2>/dev/null | awk -F'|' '
      function trim(s){gsub(/^[ \t]+|[ \t]+$/,"",s); return s}
      /^Fan[1-6]/        { fan[substr(trim($1),4,1)] = int($2) }
      /^Inlet Temp/      { inlet = int($2) }
      /^Exhaust Temp/    { exhaust = int($2) }
      /^Pwr Consumption/ { pwr = int($2) }
      END {
        printf "pwr=%d\n", pwr
        printf "inlet=%d\nexhaust=%d\n", inlet, exhaust
        for (i=1; i<=6; i++) printf "fan%d=%d\n", i, fan[i]
      }'
    printf 'psu=%s\n' "$(ipmitool sdr type "Power Supply" 2>/dev/null | grep -c 'Presence detected')"
    printf 'sel=%s\n' "$(ipmitool sel info 2>/dev/null | awk '/Entries/{print $3; exit}')"
  } > "$tmp" 2>/dev/null
  if [ -s "$tmp" ]; then mv -f "$tmp" /run/btop-server-metrics; fi
  sleep 2
done
