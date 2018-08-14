LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := curl
LOCAL_SRC_FILES := ../prebuilt_libs/${APP_ABI}/libcurl.a

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    :=  wbc

LOCAL_SRC_FILES :=  NTL/vec_GF2.cpp \
                    NTL/mat_GF2.cpp \
                    NTL/tools.cpp \
                    NTL/WordVector.cpp \
                    tableGenerator.cpp

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    :=  md5

LOCAL_SRC_FILES :=  md5.cpp

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    :=  jsoncpp

LOCAL_SRC_FILES :=  jsoncpp.cpp

LOCAL_CXXFLAGS += -fexceptions

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)


LOCAL_MODULE    :=  native-lib

LOCAL_SRC_FILES :=  native-lib.cpp \
                    utility.cpp \
                    AES.cpp


LOCAL_ARM_MODE := arm
LOCAL_STATIC_LIBRARIES := curl wbc md5 jsoncpp
LOCAL_LDLIBS := -lz -llog -landroid -lOpenSLES -latomic
LOCAL_LDFLAGS += -Wl,--gc-sections
#LOCAL_CXXFLAGS += -mllvm -sub -mllvm -fla -mllvm -perFLA=80 -mllvm -bcf -mllvm -boguscf-prob=80 -mllvm -boguscf-loop=1


include $(BUILD_SHARED_LIBRARY)