#!/bin/bash

if [ "$(id -u)" != "0" ]; then
  echo "This script must be run as root." 1>&2
  exit 1
fi

./ft232r_prog --old-pid 0x6001 --product "Clearpath Robotics /midg" --max-bus-power 500
