# 
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=mbedtls
PKG_VERSION:=2.5.1

PKG_SOURCE_URL:=@ZyXEL_SITE/opensource/tools
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-apache.tgz
PKG_CAT:=zcat

PATCH_DIR:=patches-$(PKG_VERSION)

HOST_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/host-build.mk

HOST_MAKE_FLAGS += HOST_BUILD_DIR=$(HOST_BUILD_DIR) CFGDIR=$(HOST_BUILD_DIR)/cfg DESTDIR=$(STAGING_DIR) PREFIX=/host

define Host/Install
	$(INSTALL_BIN) $(HOST_BUILD_DIR)/programs/pkey/rsa_sign $(STAGING_DIR_HOST)/bin/
endef

define Host/Clean
	rm -f $(STAGING_DIR_HOST)/bin/rsa_sign
endef

$(eval $(call HostBuild))
