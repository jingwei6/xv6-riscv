#!/bin/bash

pid=$(ps aux | grep '[q]emu-system-riscv64' | awk '{print $2}')
if [ -n "$pid" ]; then
  kill -9 $pid
  echo "Killed qemu-system-riscv64 (pid: $pid)"
else
  echo "No qemu-system-riscv64 process found."
fi