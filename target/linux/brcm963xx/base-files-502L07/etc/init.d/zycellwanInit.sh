#!/bin/sh

echo -e "\033[1;34mInitial CellWAN driver...\033[0m"
KERNELVER=`uname -r`

# CELLWAN_SUPPORT for CELLWAN_QUECTEL_QMIWWAN
 test -e /lib/modules/$KERNELVER/usbserial.ko && insmod /lib/modules/$KERNELVER/usbserial.ko
 test -e /lib/modules/$KERNELVER/usbnet.ko && insmod /lib/modules/$KERNELVER/usbnet.ko
 test -e /lib/modules/$KERNELVER/usb_wwan.ko && insmod /lib/modules/$KERNELVER/usb_wwan.ko
 test -e /lib/modules/$KERNELVER/option.ko && insmod /lib/modules/$KERNELVER/option.ko
 test -e /lib/modules/$KERNELVER/GobiNet.ko && modprobe GobiNet.ko
 #test -e /lib/modules/$KERNELVER/qmi_wwan_q.ko && modprobe qmi_wwan_q.ko
 #test -e /lib/modules/$KERNELVER/qmi_wwan.ko && modprobe qmi_wwan.ko

# Trigger internal cellwan after the driver is installed
if [ -e /proc/cell/reset ]
then
        echo "[RILCMD] wail for usb init (6s)"
        sleep 6
        echo "[RILCMD] reset the module and rilcmd will be launched in 12 seconds"
        echo 1 > /proc/cell/reset
        sleep 12
        rilcmd &
fi
