#include "com_aimazing_aimazinglib.h"
#include "md5.h"
#include "tableGenerator.h"
#include "AES.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <sys/system_properties.h>
#include <assert.h>
#include <json/json.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <../prebuilt_libs/include/curl/curl.h>


namespace Utility {
    jboolean activate(JNIEnv *env, jobject obj, jstring jUserid);
    jboolean isActivated(JNIEnv *env, jobject obj);
    Json::Value httpRequest(const char *url, std::string method, std::string payload, std::string headerStr = "");
    void txTracking(JNIEnv *env, jobject obj);
    bool isRooted(JNIEnv *env);
    jstring getSdkToken(JNIEnv *env, jobject obj);
    jstring getUserId(JNIEnv *env, jobject obj);
}