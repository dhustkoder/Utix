UTIX_JNI_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := Utix

UTIX_SRC_DIR :=${UTIX_JNI_PATH}/../../Utix/src/Utix
UTIX_INCLUDE_DIR :=${UTIX_JNI_PATH}/../../Utix/include

LOCAL_C_INCLUDES := ${UTIX_INCLUDE_DIR} ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.9/include
LOCAL_SRC_FILES := $(wildcard $(UTIX_SRC_DIR)/*.cpp)

LOCAL_CPPFLAGS := -std=c++11 -Wall -Wextra -O3 -fPIC -static
LOCAL_STATIC_LIBRARIES := Utix log


include $(BUILD_STATIC_LIBRARY)
