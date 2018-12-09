LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := modeset-plane-test.c

LOCAL_MODULE := modeset-plane-test

LOCAL_SHARED_LIBRARIES := libdrm_platform

include $(BUILD_EXECUTABLE)
