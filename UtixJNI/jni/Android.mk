UTIX_JNI_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := Utix
UTIX_SRC_DIR :=${UTIX_JNI_PATH}/../../Utix/src/Utix
UTIX_INCLUDE_DIR :=${UTIX_JNI_PATH}/../../Utix/include


LOCAL_CPPFLAGS := -std=c++11 -Wall -Wextra -fPIC -static -fno-exceptions
LOCAL_C_INCLUDES := ${UTIX_INCLUDE_DIR} ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.9/include
LOCAL_SRC_FILES := $(wildcard $(UTIX_SRC_DIR)/*.cpp)

ifdef DEBUG
LOCAL_CPPFLAGS += -D_DEBUG -O0 -g 
else
LOCAL_CPPFLAGS += -DNDEBUG -O3 -fomit-frame-pointer 
endif
LOCAL_STATIC_LIBRARIES := Utix log


include $(BUILD_STATIC_LIBRARY)
