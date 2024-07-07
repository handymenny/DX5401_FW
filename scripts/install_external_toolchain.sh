#!/bin/sh
#Download and install the needed external toolchain.

#For Broadcom toolchain: sh scripts/install_external_toolchain.sh dl brcm
#For Econet toolchain: sh scripts/install_external_toolchain.sh dl ecnt

brcm_toolchain_url=http://172.21.83.131/file/private/Broadcom/toolchain
BROADCOM_TOOLCHAIN_LINUX_4_1_0_502L07_ARM=crosstools-arm-gcc-5.5-linux-4.1-glibc-2.26-binutils-2.28.1.Rel1.10.tar.bz2
BROADCOM_TOOLCHAIN_LINUX_4_1_0_502L07_AARCH64=crosstools-aarch64-gcc-5.5-linux-4.1-glibc-2.26-binutils-2.28.1.Rel1.10.tar.bz2
BROADCOM_TOOLCHAIN_LINUX_4_1_0_502L07_MIPS=crosstools-mips-gcc-5.3-linux-4.1-uclibc-1.0.12-binutils-2.25-NPTL.Rel1.9.tar.bz2
BROADCOM_FILELIST=($BROADCOM_TOOLCHAIN_LINUX_4_1_0_502L07_ARM $BROADCOM_TOOLCHAIN_LINUX_4_1_0_502L07_AARCH64 $BROADCOM_TOOLCHAIN_LINUX_4_1_0_502L07_MIPS)
BROADCOM_DIRLIST=(${BROADCOM_TOOLCHAIN_LINUX_4_1_0_502L07_ARM%%.Rel1*} ${BROADCOM_TOOLCHAIN_LINUX_4_1_0_502L07_AARCH64%%.Rel1*} ${BROADCOM_TOOLCHAIN_LINUX_4_1_0_502L07_MIPS%%.Rel1*})
BROADCOM_CHECKLIST=("arm-buildroot-linux-gnueabi-gcc" "aarch64-buildroot-linux-gnu-gcc" "mips-buildroot-linux-uclibc-gcc")

ecnt_toolchain_url=http://172.21.83.131/file/private/Trendchip/toolchain
ECONET_TOOLCHAIN_MIPSEL_LINUX_UCLIBC_4_6_3=mipsel-linux-uclibc-4.6.3-kernel3.18.tgz
ECONET_TOOLCHAIN_MIPSEL_LINUX_GLIBC_2_2_2=mipsel-linux-glibc2.22-4.6.3.tgz
ECONET_TOOLCHAIN_BUILDROOT_GLIBC_2_2_2_ARM_32=buildroot-gcc493_glibc222_arm32_32bServer.tar.gz
ECONET_FILELIST=($ECONET_TOOLCHAIN_MIPSEL_LINUX_UCLIBC_4_6_3 $ECONET_TOOLCHAIN_MIPSEL_LINUX_GLIBC_2_2_2 $ECONET_TOOLCHAIN_BUILDROOT_GLIBC_2_2_2_ARM_32)
ECONET_DIRLIST=(${ECONET_TOOLCHAIN_MIPSEL_LINUX_UCLIBC_4_6_3%%.tgz*} ${ECONET_TOOLCHAIN_MIPSEL_LINUX_GLIBC_2_2_2%%.tgz*} ${ECONET_TOOLCHAIN_BUILDROOT_GLIBC_2_2_2_ARM_32%%.tar.gz})
ECONET_CHECKLIST=("mipsel-buildroot-linux-uclibc-gcc" "mipsel-buildroot-linux-gnu-gcc" "arm-buildroot-linux-gnueabi-gcc")

DLDIR=$1

case $2 in
"brcm")
for ((i=0;i<${#BROADCOM_FILELIST[@]};i++))
do
	if [ ! $(find /opt/toolchains/${BROADCOM_DIRLIST[i]} -name ${BROADCOM_CHECKLIST[i]} 2>/dev/null) ] ; then
		if [ ! -e "$DLDIR/${BROADCOM_FILELIST[i]}" ] ; then
			wget -r -np -nd $brcm_toolchain_url/${BROADCOM_FILELIST[i]} -P $DLDIR
		fi
		echo "Enter the root password to install the toolchain to /opt/toolchains!"
		sudo tar -xPjv -f $DLDIR/${BROADCOM_FILELIST[i]}
	else
		echo "${BROADCOM_FILELIST[i]} has already been installed!"
	fi
done
;;
"ecnt")
for ((i=0;i<${#ECONET_FILELIST[@]};i++))
do
	if [ ! $(find /opt/trendchip/${ECONET_DIRLIST[i]} -name ${ECONET_CHECKLIST[i]} 2>/dev/null) ] ; then
		if [ ! -e "$DLDIR/${ECONET_FILELIST[i]}" ] ; then
			wget -r -np -nd $ecnt_toolchain_url/${ECONET_FILELIST[i]} -P $DLDIR
		fi
		echo "Enter the root password to install the toolchain to /opt/trendchip!"
		sudo tar -zxv -f $DLDIR/${ECONET_FILELIST[i]}
		if [ ! -e "/opt/trendchip" ] ; then
			sudo mkdir /opt/trendchip
		fi
		sudo mv opt/trendchip/${ECONET_DIRLIST[i]} /opt/trendchip
		sudo rm -rf opt
	else
		echo "${ECONET_FILELIST[i]} has already been installed!"
	fi
done

;;
esac
