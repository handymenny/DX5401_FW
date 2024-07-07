#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ntpclient
#PKG_VERSION:=2003_194
PKG_VERSION:=2007_365
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@ZyXEL_SITE/opensource/network/services
DL_DIR:=$(TOPDIR)/dl

PKG_LICENSE:=GPLv2

#PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-2007

export CONFIG_ZYXEL_TTNET_NTP_MULTIWAN

include $(INCLUDE_DIR)/package.mk

define Package/ntpclient
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=An NTP (RFC-1305) client for unix-alike computers
  DEPENDS:=+libzcfg_msg +librt +libzcfg_fe_rdm_access
endef

define Package/ntpclient/install
	$(INSTALL_DIR) $(1)/bin
	$(CP) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/bin/
	$(INSTALL_DIR) $(1)/etc
	ln -s /tmp/TZ $(1)/etc/TZ 
endef

$(eval $(call BuildPackage,ntpclient))
