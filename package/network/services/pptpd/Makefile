#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk


PKG_NAME:=pptpd
PKG_VERSION:=1.4.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@ZyXEL_SITE/opensource/network/services/
PKG_MD5SUM:=36f9f45c6ffa92bc3b6e24ae2d053505
DL_DIR:=$(TOPDIR)/dl

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/pptpd
  SECTION:=net
  CATEGORY:=Network
  TITLE:=PPTP server
  MAINTAINER:=Jo-Philipp Wich <xm@subsignal.org>
  URL:=https://sourceforge.net/projects/poptop/
  DEPENDS:=+ppp +kmod-mppe +kmod-gre +kmod-iptunnel4
endef

CONFIGURE_ARGS += \
	--enable-bcrelay \

CONFIGURE_VARS += \
	ac_cv_header_libintl_h=no \

ifeq ($(CONFIG_KERNEL_ZYXEL_PPTPD_RELAY),y)
TARGET_CPPFLAGS += -DZYXEL_PPTPD_RELAY
endif

MAKE_FLAGS += \
	COPTS="$(TARGET_CFLAGS) -I$(STAGING_DIR)/usr/include" \
	INSTALL="install" \

define Package/pptpd/description
 This package contains a PPTP (Point-to-Point Tunneling Protocol) server.
endef

define Package/pptpd/conffiles
/etc/pptpd.conf
endef
PKG_BUILD_DEPENDS:=ppp

MAKE_FLAGS += OPTIMIZE="$(TARGET_CFLAGS)"

define Package/pptpd/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pptpd $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pptpctrl $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bcrelay $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DATA) ./files/pptpd.conf $(1)/etc/
endef

$(eval $(call BuildPackage,pptpd))
