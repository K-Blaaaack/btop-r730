#!/bin/sh
# btop-r730: 采集 Dell iDRAC 指标 -> /run/btop-server-metrics（btop 左侧面板）
MODEL=$(dmidecode -s system-product-name 2>/dev/null)
SERVICE=$(dmidecode -s system-serial-number 2>/dev/null)
BIOS=$(dmidecode -s bios-version 2>/dev/null)
IDRAC=$(ipmitool mc info 2>/dev/null | awk -F: '/Firmware Revision/{gsub(/[^0-9.]/,"",$2); print $2; exit}')

while :; do
  tmp=/run/btop-server-metrics.tmp
  {
    ipmitool sensor 2>/dev/null | awk -F'|' '
      function trim(s){gsub(/^[ \t]+|[ \t]+$/,"",s); return s}
      /^Fan[1-6]/        { fan[substr(trim($1),4,1)] = int($2) }
      /^Inlet Temp/      { inlet = int($2) }
      /^Exhaust Temp/    { exhaust = int($2) }
      /^Temp /           { if (cpu1 == "") cpu1 = int($2); else if (cpu2 == "") cpu2 = int($2) }
      /^Intrusion/       { intr = trim($2) }
      END {
        printf "inlet=%d\nexhaust=%d\ncpu1=%d\ncpu2=%d\n", inlet, exhaust, cpu1, cpu2
        for (i=1; i<=6; i++) printf "fan%d=%d\n", i, fan[i]
        printf "intrusion=%s\n", (intr == "0x0" ? "clear" : intr)
      }'
    ipmitool dcmi power reading 2>/dev/null | awk -F: '
      /Instantaneous/ { gsub(/[^0-9]/,"",$2); printf "pwr=%s\n", $2 }
      /Minimum/       { gsub(/[^0-9]/,"",$2); printf "pwr_min=%s\n", $2 }
      /Maximum/       { gsub(/[^0-9]/,"",$2); printf "pwr_max=%s\n", $2 }
      /Average/       { gsub(/[^0-9]/,"",$2); printf "pwr_avg=%s\n", $2 }'
    printf 'psu=%s\n' "$(ipmitool sdr type "Power Supply" 2>/dev/null | grep -c 'Presence detected')"
    printf 'sel=%s\n' "$(ipmitool sel info 2>/dev/null | awk '/Entries/{print $3; exit}')"
    printf 'bios=%s\nidrac=%s\nmodel=%s\nservice=%s\n' "$BIOS" "$IDRAC" "$MODEL" "$SERVICE"
  } > "$tmp" 2>/dev/null
  if [ -s "$tmp" ]; then mv -f "$tmp" /run/btop-server-metrics; fi
  sleep 3
done
