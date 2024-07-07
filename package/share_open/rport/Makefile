include $(TOPDIR)/rules.mk

PKG_NAME:=rport
PKG_VERSION:=0.8.2
PKG_RELEASE:=0

PKG_BUILD_DIR:=$(BUILD_DIR)/rport-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:=https://github.com/cloudradar-monitoring/rport/archive/refs/tags/
PKG_SOURCE_URL:=@ZyXEL_SITE/opensource/network/services
PKG_HASH:=e6de76a454a95e40b856ae67036651e7
CPU:=$(subst powerpc,ppc,$(subst aarch64,arm64,$(subst x86_64,x64,$(subst i386,ia32,$(ARCH)))))
ifeq ("$(CPU)", "mips")
MIPS_FLOAT:=softfloat 
MIPS_ARCH:='-march=mips32r2'
endif

include $(INCLUDE_DIR)/package.mk

define Package/rport
	SECTION:=admin
	CATEGORY:=Administration
	TITLE:=Remote Management (RMM) Software
	URL:=https://github.com/cloudradar-monitoring/rport
	DEPENDS:=+libpthread +uuidgen
endef

define Package/rport/description
	Rport helps you to manage your remote servers without the hassle of VPNs,
	chained SSH connections, jump-hosts, or the use of commercial tools like
	TeamViewer and its clones.
endef

define Build/Compile
	cd $(PKG_BUILD_DIR) && \
	CGO_ENABLED=1 \
	GOOS=linux \
	GOARCH=$(CPU) \
	GOGCCFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
	GOMIPS=$(MIPS_FLOAT) \
	CGO_CFLAGS=$(MIPS_ARCH) \
	CC=$(TARGET_CC_NOCACHE) \
	go build -ldflags "-s -w -X 'github.com/cloudradar-monitoring/rport/share.BuildVersion=$(PKG_VERSION)'" \
	-o $(PKG_NAME) \
	-v ./cmd/$(PKG_NAME)/... && \
	upx -9 --ultra-brute $(PKG_NAME)
endef

define Package/rport/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DATA) ./files/rport.conf $(1)/etc/rport.conf
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/rp_serv.sh $(1)/etc/init.d/rp_serv.sh
	$(INSTALL_DIR) $(1)/etc/rc3.d
	ln -s ../init.d/rp_serv.sh $(1)/etc/rc3.d/S51rp_serv
endef

$(eval $(call BuildPackage,rport))
