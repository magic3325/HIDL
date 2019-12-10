LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := tvibrator_drv.ko
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_INIT_RC := init.tvibrator_drv.rc
# LOCAL_SRC_FILES := \
# 	tvibrator_drv.c\
# 	Makefile
LOCAL_REQUIRED_MODULES :=


ifeq ($(TARGET_OUT_VENDOR),)
LOCAL_MODULE_PATH := $(ALPS_OUT)/vendor/lib/modules
else
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/lib/modules
endif

include $(MTK_KERNEL_MODULE)
