# copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=owamp
PKG_VERSION:=4.4.4
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/perfsonar/owamp/archive/v$(PKG_VERSION)/
DL_DIR:=$(TOPDIR)/dl

PKG_LICENSE:=BSD
PKG_LICENSE_FILES:=COPYING

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

PKG_INSTALL:=1

define Download/i2util
  FILE:=i2util-$(PKG_VERSION).tar.gz
  URL:=https://github.com/perfsonar/i2util/archive/v$(PKG_VERSION)/
endef

define Package/OWAMP
  SECTION:=net
  CATEGORY:=Network
  TITLE:=A tool for performing one-way or two-way active measurements
  URL:=https://github.com/perfsonar/owamp
endef

define Package/OWAMP/description
  This release is contains implementations of the OWAMP (one-way active measurement protocol)
  defined by https://tools.ietf.org/html/rfc4656 and the TWAMP (two-way active measurement protocol)
  defined by https://tools.ietf.org/html/rfc5357.
endef

define Build/Prepare
	$(PKG_UNPACK)
	tar xzf $(DL_DIR)/i2util-$(PKG_VERSION).tar.gz --strip-components=1 -C $(PKG_BUILD_DIR)/I2util
	$(Build/Patch)
	  ( cd $(PKG_BUILD_DIR) ; \
		[ -f ./configure ] || { \
			./bootstrap ; \
		} \
	)
endef

define Package/OWAMP/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib
	$(TARGET_CROSS)strip $(PKG_INSTALL_DIR)/usr/bin/*
	install -m 755 $(PKG_INSTALL_DIR)/usr/bin/*  $(1)/usr/bin/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*  $(1)/usr/lib/
endef


$(eval $(call Download,i2util))
$(eval $(call BuildPackage,OWAMP))

