#include "utility.h"

using namespace std;

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#define JNIREG_UTILITY_CLASS "com/aimazing/aimazinglib/Utility"

// ============= JNI Interface ================
jboolean activate(JNIEnv *env, jobject obj, jstring userId);
jboolean isActivated(JNIEnv *env, jobject obj);
jstring getSdkToken(JNIEnv *env, jobject obj);
jstring getUserId(JNIEnv *env, jobject obj);
// ========== End of JNI Interface ============


// Binding Methods, hiding functions' name in java side
static JNINativeMethod u_method_table[] = {
        { "native_activate", "(Ljava/lang/String;Z)Z", (void*)activate },
        { "native_isActivated", "()Z", (void*)isActivated },
};

// Register Native Method to Java
static jint registerNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gMethods, int numMethods) {

    jclass clazz;
    clazz = env->FindClass(className);

    if (clazz == NULL) {
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}


void register_ndk_load(JNIEnv *env) {
    registerNativeMethods(env, JNIREG_UTILITY_CLASS, u_method_table, NELEM(u_method_table));
}


JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

    JNIEnv* env = NULL;
    jint result = -1;

    if ((vm)->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    register_ndk_load(env);

    // Return JNI version
    return JNI_VERSION_1_4;
}

jboolean native_activate(JNIEnv *env, jobject obj, jstring UserId) {
    return Utility::activate(env, obj, UserId);
}


jboolean native_isActivated(JNIEnv *env, jobject obj) {
    return (jboolean) Utility::isActivated(env, obj);
}

jstring getSdkToken(JNIEnv *env, jobject obj){
    return Utility::getSdkToken(env, obj);
}

jstring getUserId(JNIEnv *env, jobject obj){
    return Utility::getUserId(env, obj);
}