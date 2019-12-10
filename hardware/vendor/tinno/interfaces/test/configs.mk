LOCAL_PATH := vendor/tinno/interfaces/test

$(warning LOCAL_PATH=$(LOCAL_PATH))

VENDOR_SEPOLICY_FILE += $(LOCAL_PATH)/sepolicy

DEVICE_MANIFEST_FILE += \
	$(LOCAL_PATH)/manifest.xml

DEVICE_FRAMEWORK_COMPATIBILITY_MATRIX_FILE += \
	$(LOCAL_PATH)/framework-manifest.xml

PRODUCT_PACKAGES += \
   vendor.tinno.hardware.test@1.0 \
	vendor.tinno.hardware.test@1.0-impl \
	vendor.tinno.hardware.test@1.0-service \
   vendor.tinno.hardware.test@1.0-service.rc \

#PRODUCT_PACKAGES += test.default
PRODUCT_PACKAGES += test_test

PRODUCT_PROPERTY_OVERRIDES += ro.hardware.test=$(TARGET_PRODUCT)
