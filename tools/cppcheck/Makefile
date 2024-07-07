# 
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=cppcheck
PKG_VERSION:=1.82

PKG_SOURCE_URL:=@ZyXEL_SITE/opensource/tools
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_CAT:=zcat

HOST_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/host-build.mk

HOST_MAKE_FLAGS += HOST_BUILD_DIR=$(HOST_BUILD_DIR) CFGDIR=$(HOST_BUILD_DIR)/cfg DESTDIR=$(STAGING_DIR) PREFIX=/host

define Host/Install
	$(INSTALL_BIN) $(HOST_BUILD_DIR)/cppcheck $(STAGING_DIR_HOST)/bin/
endef

define Host/Clean
	rm -f $(STAGING_DIR_HOST)/bin/cppcheck
endef

$(eval $(call HostBuild))
