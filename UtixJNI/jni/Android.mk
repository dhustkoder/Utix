LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

UTIX_SRC_DIR :=${LOCAL_PATH}/../../Utix/src/Utix/
UTIX_INCLUDE_DIR :=${LOCAL_PATH}/../../Utix/include/


LOCAL_MODULE := Utix
LOCAL_C_INCLUDES := ${UTIX_INCLUDE_DIR} ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.9/include
LOCAL_SRC_FILES := $(wildcard $(UTIX_SRC_DIR)*.cpp)

LOCAL_CPPFLAGS := -std=c++11 -Wall -Wextra -O3 -fPIC -static
LOCAL_STATIC_LIBRARIES := Utix log


include $(BUILD_STATIC_LIBRARY)