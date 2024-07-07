include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=faifa
PKG_VERSION:=0.2~svn82.orig
#PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

PATCH_DIR:=patches

FILE:=file

PKG_CAT:=zcat

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_DEPENDS:=libopenssl libpcap

export CONFIG_ZYXEL_HOMEPLUG_DEVICE_DISCOVER

include $(INCLUDE_DIR)/package.mk

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	tar -zxvf $(PKG_SOURCE) -C  $(BUILD_DIR)
	cp $(FILE)/*.* $(PKG_BUILD_DIR)
	cp $(FILE)/configure $(PKG_BUILD_DIR)
endef

define Package/faifa/Default
  SECTION:=LAN
  CATEGORY:=home plug
  TITLE:=home plug faifa
  URL:=https://launchpad.net/ubuntu/+source/faifa/0.2~svn82-1build2
  DESCRIPTION:=
  DEPENDS:=+libopenssl +libpcap
endef

CONFIGURE_ARGS += \
	--libdir=/lib

#########################################3
define Package/faifa
  $(call Package/faifa/Default)
  TITLE+= Daemon
endef


define Build/Compile
	cp $(FILE)/Makefile $(PKG_BUILD_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR)
endef


define Package/faifa/install
	mkdir -p $(PKG_BUILD_DIR)/gpl
	$(MAKE) DESTDIR=$(PKG_BUILD_DIR)/gpl -C $(PKG_BUILD_DIR) install
	$(INSTALL_DIR) $(1)/usr
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/gpl/usr/lib/libfaifa.so.0 $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/gpl/usr/bin/faifa $(1)/usr/bin/
endef


$(eval $(call BuildPackage,faifa))
########################################

