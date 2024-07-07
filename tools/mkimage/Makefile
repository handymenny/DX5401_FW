#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=mkimage
ifeq ("$(strip ${ZCFG_PLATFORM})","QUALCOMM")
PKG_VERSION:=2014.10

PKG_SOURCE:=u-boot-$(PKG_VERSION).tar.bz2
PKG_MD5SUM:=3ddcaee2f05b7c464778112ec83664b5
else ifneq ($(strip $(CONFIG_TC_SDK_7_3_267_1)$(CONFIG_TC_SDK_7_3_267_750)),)
#PKG_VERSION:=en7523-mkimage
PKG_VERSION:=2014.04

PKG_SOURCE:=u-boot-$(PKG_VERSION).tar.bz2
#PKG_SOURCE_URL:=ftp://ftp.denx.de/pub/u-boot
PKG_SOURCE_URL:=@ZyXEL_SITE/opensource/loader
PKG_MD5SUM:=6d2116d1385a66e9a59742caa9d62a54
else
PKG_VERSION:=2012.04.01

PKG_SOURCE:=u-boot-$(PKG_VERSION).tar.bz2
#PKG_SOURCE_URL:=ftp://ftp.denx.de/pub/u-boot
PKG_MD5SUM:=192bb231082d9159fb6e16de3039b6b2
endif

PKG_CAT:=bzcat
PKG_SOURCE_URL:=@ZyXEL_SITE/opensource/loader

HOST_BUILD_DIR:=$(BUILD_DIR_HOST)/u-boot-$(PKG_VERSION)
PATCH_DIR:=patches-$(PKG_VERSION)
include $(INCLUDE_DIR)/host-build.mk

ifeq ("$(strip ${ZCFG_PLATFORM})","QUALCOMM")
define Host/Prepare
	$(Host/Prepare/Default)
	rm -f \
		$(HOST_BUILD_DIR)/include/errno.h \
		$(HOST_BUILD_DIR)/include/malloc.h \
		$(HOST_BUILD_DIR)/tools/.depend
	touch $(HOST_BUILD_DIR)/include/config.mk
	touch $(HOST_BUILD_DIR)/include/config.h
endef
else ifneq ($(strip $(CONFIG_TC_SDK_7_3_267_1)$(CONFIG_TC_SDK_7_3_267_750)),)
define Host/Prepare
	$(Host/Prepare/Default)
	rm -f \
		$(HOST_BUILD_DIR)/include/errno.h \
		$(HOST_BUILD_DIR)/include/malloc.h \
		$(HOST_BUILD_DIR)/tools/.depend
	touch $(HOST_BUILD_DIR)/include/config.mk
	touch $(HOST_BUILD_DIR)/include/config.h
endef
else
define Host/Prepare
	$(Host/Prepare/Default)
	rm -f \
		$(HOST_BUILD_DIR)/include/errno.h \
		$(HOST_BUILD_DIR)/include/malloc.h
endef
endif

ifeq ("$(strip ${ZCFG_PLATFORM})","QUALCOMM")
define Host/Compile
	$(MAKE) -C $(HOST_BUILD_DIR) defconfig   HOSTCFLAGS="$(HOST_CPPFLAGS) $(HOST_CFLAGS)" HOSTLDFLAGS="$(HOST_LDFLAGS)"
	$(MAKE) -C $(HOST_BUILD_DIR) tools-only  HOSTCFLAGS="$(HOST_CPPFLAGS) $(HOST_CFLAGS)" HOSTLDFLAGS="$(HOST_LDFLAGS)"
endef
else ifneq ($(strip $(CONFIG_TC_SDK_7_3_267_1)$(CONFIG_TC_SDK_7_3_267_750)),)
define Host/Compile
	$(MAKE) -C $(HOST_BUILD_DIR) \
		HOSTLDFLAGS="$(HOST_STATIC_LINKING)" \
		tools-only
endef
else
define Host/Compile
	rm -f $(HOST_BUILD_DIR)/tools/.depend
	touch $(HOST_BUILD_DIR)/include/config.h
	$(MAKE) -C $(HOST_BUILD_DIR) BUILD_DIR= BIN_FILES-y="mkimage" HOSTLDFLAGS="$(HOST_STATIC_LINKING)" tools
endef
endif

define Host/Install
	$(CP) $(HOST_BUILD_DIR)/tools/mkimage $(STAGING_DIR_HOST)/bin/
endef

define Host/Clean
	rm -f $(STAGING_DIR_HOST)/bin/mkimage
endef

$(eval $(call HostBuild))
