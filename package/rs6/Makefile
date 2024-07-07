#
# 
#
# Send IPv6 RS Software.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rs6
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_CONFIG_DEPENDS:=
PACKAGE_DIR:=$(BIN_DIR)/packages/private
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define Package/rs6
  SECTION:=net
  CATEGORY:=Zyxel Packages
  TITLE:=rs6/Send IPv6 RS
  #SUBMENU:=Applications
endef

define Package/rs6/description
  rs6/Send IPv6 RS
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

TARGET_CFLAGS += \
	-I$(LINUX_DIR)/arch/$(LINUX_KARCH)/include \
	-I$(LINUX_DIR)/drivers/net/usb

MAKE_FLAGS += \
	CFLAGS="$(TARGET_CFLAGS)"


define Build/Configure
endef

define Package/rs6/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rs6 $(1)/usr/bin
endef


$(eval $(call BuildPackage,rs6))

