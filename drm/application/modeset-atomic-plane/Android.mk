LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := modeset-atomic-plane.c

LOCAL_MODULE := modeset-atomic-plane

LOCAL_SHARED_LIBRARIES := libdrm_platform

include $(BUILD_EXECUTABLE)
