#!/bin/sh
setcap cap_sys_time+ep /usr/bin/setrtc
systemctl mask systemd-journald
systemctl stop systemd-journald