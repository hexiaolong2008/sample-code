LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := modeset-page-flip.c

LOCAL_MODULE := modeset-page-flip

LOCAL_SHARED_LIBRARIES := libdrm_platform

include $(BUILD_EXECUTABLE)
