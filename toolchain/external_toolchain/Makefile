#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=external_toolchain

HOST_BUILD_PARALLEL:=1

ifeq ($(call qstrip,$(ZCFG_PLATFORM)),BROADCOM)
  EXTERNAL_TOOLCHAIN_URL:=http://172.21.83.131/file/private/Broadcom/toolchain
  ifeq ($(call qstrip,$(CONFIG_ARCH)),arm)
    EXTERNAL_TOOLCHAIN_SOURCE:=crosstools-arm-gcc-5.5-linux-4.1-glibc-2.26-binutils-2.28.1.Rel1.10.tar.bz2
  endif
  ifeq ($(call qstrip,$(CONFIG_ARCH)),aarch64)
    EXTERNAL_TOOLCHAIN_SOURCE:=crosstools-aarch64-gcc-5.5-linux-4.1-glibc-2.26-binutils-2.28.1.Rel1.10.tar.bz2
  endif
  ifeq ($(call qstrip,$(CONFIG_ARCH)),mips)
    EXTERNAL_TOOLCHAIN_SOURCE:=crosstools-mips-gcc-5.3-linux-4.1-uclibc-1.0.12-binutils-2.25-NPTL.Rel1.9.tar.bz2
  endif
endif

ifeq ($(call qstrip,$(ZCFG_PLATFORM)),ECONET)
  EXTERNAL_TOOLCHAIN_URL:=http://172.21.83.131/file/private/Trendchip/toolchain
  ifeq ($(call qstrip,$(CONFIG_ARCH)),mipsel)
    ifeq ($(call qstrip,$(CONFIG_TOOLCHAIN_LIBC)),uclibc)
      EXTERNAL_TOOLCHAIN_SOURCE:=mipsel-linux-uclibc-4.6.3-kernel3.18.tgz
    endif
    ifeq ($(call qstrip,$(CONFIG_TOOLCHAIN_LIBC)),glibc)
      EXTERNAL_TOOLCHAIN_SOURCE:=mipsel-linux-glibc2.22-4.6.3.tgz
    endif
  endif
  ifeq ($(call qstrip,$(CONFIG_ARCH)),arm)
#2022/03/03
#pureSDK toolchain=>gawk cause many openWRT packages fail, remove it, rename to *_zyxel_update.tar.gz
#EXTERNAL_TOOLCHAIN_SOURCE:=buildroot-gcc493_glibc222_arm32_32bServer.tar.gz
	EXTERNAL_TOOLCHAIN_SOURCE:=buildroot-gcc493_glibc222_arm32_32bServer_zyxel_update.tar.gz
  endif
endif

PKG_SOURCE:=$(EXTERNAL_TOOLCHAIN_SOURCE)
include $(INCLUDE_DIR)/toolchain-build.mk

define Host/Prepare
	if [ ! -d $(TOPDIR)/staging_dir/$(PKG_NAME) ]; then \
		mkdir -p $(TOPDIR)/staging_dir/$(PKG_NAME); \
	fi
	if [ ! -f $(DL_DIR)/$(EXTERNAL_TOOLCHAIN_SOURCE) ]; then \
		$(SCRIPT_DIR)/download.pl "$(DL_DIR)" "$(EXTERNAL_TOOLCHAIN_SOURCE)" "" "$(EXTERNAL_TOOLCHAIN_URL)"; \
	fi
	$(UNPACK_CMD) -C $(TOPDIR)/staging_dir/$(PKG_NAME)
    ifeq ($(call qstrip,$(ZCFG_PLATFORM)),ECONET)
     ifeq ($(call qstrip,$(CONFIG_ARCH)),mipsel)
	    rm -f $(TOPDIR)/staging_dir/$(PKG_NAME)/opt/trendchip/mipsel-linux-glibc2.22-4.6.3/usr/bin/igawk
	    rm -f $(TOPDIR)/staging_dir/$(PKG_NAME)/opt/trendchip/mipsel-linux-glibc2.22-4.6.3/usr/bin/gawk
     endif
#     ifeq ($(call qstrip,$(CONFIG_ARCH)),arm)
#	    rm -f $(TOOLCHAIN_ROOT_DIR)/bin/igawk
#	    rm -f $(TOOLCHAIN_ROOT_DIR)/bin/gawk
#    endif
    endif
endef

define Host/Compile
endef

define Host/Install
endef

define Host/Clean
	$(RM) -rf $(TOPDIR)/staging_dir/$(PKG_NAME)
endef

$(eval $(call HostBuild))
