#include <jni.h>
#include <android/log.h>

#define DEBUG 1

#ifdef DEBUG

#define LOGD(...) __android_log_print(ANDROID_LOG_VERBOSE, __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, __VA_ARGS__);

#else

#define LOGD(...)
#define LOGE(...)

#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif