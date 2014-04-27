include $(TOPDIR)/rules.mk

PKG_NAME:=command1
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/command1
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Command1 Controller
endef

define Package/command1/description
	Utility for controlling Command1 Lights.
	
	To use this tool you must have a 433Mhz transmitter
	connected to a GPIO port.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/command1/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/command1 $(1)/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/command1.init $(1)/etc/init.d/command1
endef


$(eval $(call BuildPackage,command1))
