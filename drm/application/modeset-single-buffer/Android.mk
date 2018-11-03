LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := modeset-single-buffer.c

LOCAL_MODULE := modeset-single-buffer

LOCAL_SHARED_LIBRARIES := libdrm_platform

include $(BUILD_EXECUTABLE)
