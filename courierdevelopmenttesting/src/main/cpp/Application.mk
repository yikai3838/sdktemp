LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

APP_STL := gnustl_shared

#NDK_TOOLCHAIN_VERSION := obfuscator3.6
#NDK_TOOLCHAIN_VERSION := obfuscator3.6

include $(BUILD_SHARED_LIBRARY)