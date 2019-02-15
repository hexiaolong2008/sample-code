LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := modeset-atomic-crtc.c

LOCAL_MODULE := modeset-atomic-crtc

LOCAL_SHARED_LIBRARIES := libdrm_platform

include $(BUILD_EXECUTABLE)
