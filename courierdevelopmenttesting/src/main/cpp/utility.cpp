#include "utility.h"

using namespace std;

namespace Utility {

    // Cert Location
    static string caCertPtr;

    void setDir(JNIEnv *env, jobject obj);
    bool registerDev(JNIEnv *env, jobject obj);
    static bool writeSharedPref(JNIEnv *env, jobject obj, string name, string key, string value);
    char *jstringToChar(JNIEnv *env, jstring jstr);
    string getStoragePath(JNIEnv *env, jobject obj);
    string saveCertPemFile(JNIEnv *env, jobject obj);
    string getDeviceId(JNIEnv *env);
    string getAppId(JNIEnv *env, jobject obj);
    string longtoStr(unsigned long val);
    static string readSharedPref(JNIEnv *env, jobject obj, string name, string key);
    size_t writeCallback(const char *in, size_t size, size_t num, string *out);

    // String obfuscator
    string generateString(int mode){
        string a = "a";
        string b = "b";
        string c = "c";
        string d = "d";
        string e = "e";
        string f = "f";
        string g = "g";
        string h = "h";
        string i = "i";
        string j = "j";
        string k = "k";
        string l = "l";
        string m = "m";
        string n = "n";
        string o = "o";
        string p = "p";
        string q = "q";
        string r = "r";
        string s = "s";
        string t = "t";
        string u = "u";
        string v = "v";
        string w = "w";
        string x = "x";
        string y = "y";
        string z = "z";

        string A = "A";
        string B = "B";
        string C = "C";
        string D = "D";
        string E = "E";
        string F = "F";
        string G = "G";
        string H = "H";
        string I = "I";
        string J = "J";
        string K = "K";
        string L = "L";
        string M = "M";
        string N = "N";
        string O = "O";
        string P = "P";
        string Q = "Q";
        string R = "R";
        string S = "S";
        string T = "T";
        string U = "U";
        string V = "V";
        string W = "W";
        string X = "X";
        string Y = "Y";
        string Z = "Z";

        string _1 = "1";
        string _2 = "2";
        string _3 = "3";
        string _4 = "4";
        string _5 = "5";
        string _6 = "6";
        string _7 = "7";
        string _8 = "8";
        string _9 = "9";
        string _0 = "0";

        string _a = "-";
        string _b = ":";
        string _c = "/";
        string _d = " ";
        string _e = "\"";
        string _f = "{";
        string _g = "}";
        string _h = ".";
        string _i = ",";
        string _j = "_";

        switch (mode){
            case 1: // ACK
                return A+C+K;
            case 2: // POST
                return P+O+S+T;
            case 3: // Content-Type: application/json
                return C+o+n+t+e+n+t+_a+T+y+p+e+_b+_d+a+p+p+l+i+c+a+t+i+o+n+_c+j+s+o+n;
            case 4: // charsets: utf-8
                return c+h+a+r+s+e+t+s+_b+_d+u+t+f+_a+_8;
            case 5: // GET
                return G+E+T;
            case 6: // code
                return c+o+d+e;
            case 7: // body
                return b+o+d+y;
            case 8: // cacert.pem
                return c+a+c+e+r+t+_h+p+e+m;
            case 9: // 35
                return _3+_5;
            case 10: // {"deviceID":"
                return _f+_e+d+e+v+i+c+e+I+D+_e+_b+_e;
            case 11: // ","clientId":"
                return _e+_i+_e+c+l+i+e+n+t+I+d+_e+_b+_e;
            case 12: // ","deviceModel":"
                return _e+_i+_e+d+e+v+i+c+e+M+o+d+e+l+_e+_b+_e;
            case 13: // ","userID":"
                return _e+_i+_e+u+s+e+r+I+D+_e+_b+_e;
            case 14: // "}
                return _e+_g;
            case 15: // https://apn.aimazing.com.sg/api/activation/courier
                return h+t+t+p+s+_b+_c+_c+a+p+n+_h+a+i+m+a+z+i+n+g+_h+c+o+m+_h+s+g+_c+a+p+i+_c+a+c+t+i+v+a+t+i+o+n+_c+c+o+u+r+i+e+r;
            case 16: // 200
                return _2+_0+_0;
            case 17: // token
                return t+o+k+e+n;
            case 18: // tokenHash
                return t+o+k+e+n+H+a+s+h;
            case 19: // tokenExpiry
                return t+o+k+e+n+E+x+p+i+r+y;
            case 20: // cacertPath
                return c+a+c+e+r+t+P+a+t+h;
            case 21: // currentTime
                return c+u+r+r+e+n+t+T+i+m+e;
            case 22: // /proc/self/status
                return _c+p+r+o+c+_c+s+e+l+f+_c+s+t+a+t+u+s;
            case 23: // TracerPid:
                return T+r+a+c+e+r+P+i+d+_b;
            case 24: // release-keys
                return r+e+l+e+a+s+e+_a+k+e+y+s;
            case 25: // MWtoken
                return M+W+t+o+k+e+n;
            case 26: // x
                return x;
            case 27: // time_offset
                return t+i+m+e+_j+o+f+f+s+e+t;
            case 28: // {"username":"
                return _f+_e+u+s+e+r+n+a+m+e+_e+_b+_e;
            case 29: // ","password":"
                return  _e+_i+_e+p+a+s+s+w+o+r+d+_e+_b+_e;
            case 31: // server_time
                return s+e+r+v+e+r+_j+t+i+m+e;
            case 32: // expiry_date
                return e+x+p+i+r+y+_j+d+a+t+e;
            case 33: // {"tott":"
                return _f+_e+t+o+t+t+_e+_b+_e;
            case 34: // ","amount":"
                return _e+_i+_e+a+m+o+u+n+t+_e+_b+_e;
            case 35: // ","random":"
                return _e+_i+_e+r+a+n+d+o+m+_e+_b+_e;
            case 37: // amz-token:
                return a+m+z+_a+t+o+k+e+n+_b+_d;
            case 41: // sdkToken;
                return s+d+k+T+o+k+e+n;
            case 42: // userId;
                return u+s+e+r+I+d;
            case 43: // ","osVersion":"
                return _e+_i+_e+o+s+V+e+r+s+i+o+n+_e+_b+_e;
            case 44: // ","calibrationResult":"
                return _e+_i+_e+c+a+l+i+b+r+a+t+i+o+n+R+e+s+u+l+t+_e+_b+_e;
            case 45: // ","sdkName":"
                return _e+_i+_e+s+d+k+N+a+m+e+_e+_b+_e;
            case 46: // ","sdkVersion":"
                return _e+_i+_e+s+d+k+V+e+r+s+i+o+n+_e+_b+_e;
            case 47: // ","appID":"
                return _e+_i+_e+a+p+p+I+D+_e+_b+_e;
            case 50: // courierJNE
                return c+o+u+r+i+e+r+J+N+E;
            case 51: // v1.0.0
                return v+_1+_h+_0+_h+_0;
            case 100: // 706f94ca-01b1-48f4-90e0-24082a112f65
                return _7+_0+_6+f+_9+_4+c+a+_a+_0+_1+b+_1+_a+_4+_8+f+_4+_a+_9+_0+e+_0+_a+_2+_4+_0+_8+_2+a+_1+_1+_2+f+_6+_5;

            default:
                return "";
        }
    }



    // HTTP Connection Callback
    size_t writeCallback(const char *in, size_t size, size_t num, string *out) {

        const size_t totalBytes(size * num);
        out->append(in, totalBytes);

        return totalBytes;
    }

    Json::Value httpRequest(const char* url, string method, string payload, string headerString) {

        /*
         * 0. Method Determination
         * 1. Payload Determination
         * 2. Headers Determination
         *
         */

        CURL* curl = curl_easy_init();
        struct curl_slist *requestHeaders = NULL;
        Json::Reader jsonReader;
        Json::Value responseJsonBody;
        string *responseStringBody = new string();

        // Init Curl Easy
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6L);
        curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, caCertPtr.c_str());
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, responseStringBody);

        // Init Request Header
        if (headerString.compare("") != 0) {
            LOGE("requestHeader", "%s", headerString.c_str());
            requestHeaders = curl_slist_append(requestHeaders, headerString.c_str());
        } else {
            LOGE("requestHeader", "");
        }

        // Init Request Body
        if (method.compare(generateString(2)) == 0) { // "POST"
            requestHeaders = curl_slist_append(requestHeaders, generateString(3).c_str()); // "Content-Type: application/json"
            requestHeaders = curl_slist_append(requestHeaders, generateString(4).c_str()); // "charsets: utf-8"

            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payload.length());
        } else if (method.compare(generateString(5)) == 0) { // "GET"

        } else {
            return Json::nullValue;
        }

        // Send HTTP Request
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, requestHeaders);
        CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(requestHeaders);

        //
        // HTTP Response
        //

        // If Curl Failed
        if (res != CURLE_OK) {
            LOGE("curl", "CURL failed with error code %d", res);
        }

        // Get Response Code
        long httpCodeLong(0);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCodeLong);
        string responseCode = longtoStr(httpCodeLong);;
        LOGE("curl", "http response code: %s", responseCode.c_str());

        // Build Return Json Object
        if (!jsonReader.parse(*responseStringBody, responseJsonBody)) {
            LOGE("curl", "Could not parse HTTP data as JSON");
            LOGE("curl", "HTTP data was:%c", *responseStringBody->c_str());
        }

        Json::FastWriter fastWriter;

        Json::Value returnValue;
        returnValue[generateString(6)] = responseCode; // "code"
        returnValue[generateString(7)] = fastWriter.write(responseJsonBody); // "body"

        curl_easy_cleanup(curl);

        delete responseStringBody;
        return returnValue;
    }


    void setDir(JNIEnv *env, jobject obj) {

        jstring caCertDir = env->NewStringUTF(saveCertPemFile(env, obj).c_str());

        if (!caCertDir) return;
        const char *caCertDir_c = env->GetStringUTFChars(caCertDir, NULL);
        if (!caCertDir_c) return;
        const jsize len = env->GetStringUTFLength(caCertDir);

        string caCert(caCertDir_c, len);
        caCertPtr = caCert;

        LOGD("CaCertDirptr", " in std string: %s", caCertPtr.c_str());

        env->ReleaseStringUTFChars(caCertDir, caCertDir_c);
    }


    string getStoragePath(JNIEnv *env, jobject obj) {

        jclass cls_emitter = env->GetObjectClass(obj);
        jfieldID getActivity = env->GetFieldID(cls_emitter, "activity", "Landroid/app/Activity;");
        jobject activity = env->GetObjectField(obj, getActivity);
        jclass cls_activity = env->FindClass("android/app/Activity");

        jmethodID getContext = env->GetMethodID(cls_activity, "getApplicationContext",
                                                "()Landroid/content/Context;");
        jobject context = env->CallObjectMethod(activity, getContext);
        jmethodID getInfo = env->GetMethodID(cls_activity, "getApplicationInfo",
                                             "()Landroid/content/pm/ApplicationInfo;");
        jobject info = env->CallObjectMethod(activity, getInfo);

        jclass cls_info = env->FindClass("android/content/pm/ApplicationInfo");
        jfieldID getdataDir = env->GetFieldID(cls_info, "dataDir", "Ljava/lang/String;");
        jstring jdestPath = (jstring) env->GetObjectField(info, getdataDir);

        string destPath = jstringToChar(env, jdestPath);

        env->DeleteLocalRef(cls_info);
        env->DeleteLocalRef(info);
        env->DeleteLocalRef(context);

        return destPath;

    }


    string saveCertPemFile(JNIEnv *env, jobject obj) {

        string assetFileName = generateString(8);
        string destPath = getStoragePath(env, obj);
        string certFilePath = destPath + "/" + assetFileName;

        jclass cls_emitter = env->GetObjectClass(obj);
        jfieldID getActivity = env->GetFieldID(cls_emitter, "activity", "Landroid/app/Activity;");
        jobject activity = env->GetObjectField(obj, getActivity);

        jclass cls_activity = env->FindClass("android/app/Activity");
        jmethodID getContext = env->GetMethodID(cls_activity, "getApplicationContext",
                                                "()Landroid/content/Context;");
        jobject context = env->CallObjectMethod(activity, getContext);

        jclass cls_file = env->FindClass("java/io/File");
        jmethodID jfile_init = env->GetMethodID(cls_file, "<init>", "(Ljava/lang/String;)V");
        jobject jfile = env->NewObject(cls_file, jfile_init,
                                       env->NewStringUTF(certFilePath.c_str()));

        jmethodID jfile_exists = env->GetMethodID(cls_file, "exists", "()Z");
        jboolean isExists = env->CallBooleanMethod(jfile, jfile_exists);

        if (isExists) {
            jmethodID jfile_delete = env->GetMethodID(cls_file, "delete", "()Z");
            jboolean isDelete = env->CallBooleanMethod(jfile, jfile_delete);
        }

        jclass cls_context = env->FindClass("android/content/Context");
        jmethodID getAssets = env->GetMethodID(cls_context, "getAssets",
                                               "()Landroid/content/res/AssetManager;");
        jobject assetManager = env->CallObjectMethod(context, getAssets);

        AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
        AAsset *asset = AAssetManager_open(mgr, assetFileName.c_str(), AASSET_MODE_STREAMING);

        char buf[BUFSIZ];
        int nb_read = 0;
        FILE *out = fopen(certFilePath.c_str(), "w");
        while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0)
            fwrite(buf, nb_read, 1, out);
        fclose(out);
        AAsset_close(asset);

        env->DeleteLocalRef(assetManager);
        env->DeleteLocalRef(cls_context);
        env->DeleteLocalRef(jfile);
        env->DeleteLocalRef(cls_file);
        env->DeleteLocalRef(context);

        return certFilePath;
    }


    string longtoStr(unsigned long val) {

        string mystring;
        stringstream mystream;
        mystream << val;
        mystring = mystream.str();

        return mystring;
    }


    string getModel(JNIEnv *env) {
        jclass cls_build = env->FindClass("android/os/Build");
        jclass cls_uuid = env->FindClass("java/util/UUID");

        // Get MANUFACTURER
        jfieldID getManufacturer = env->GetStaticFieldID(cls_build, "MANUFACTURER",
                                                         "Ljava/lang/String;");
        jstring jmanufacturer = (jstring) env->GetStaticObjectField(cls_build, getManufacturer);

        // Get MODEL
        jfieldID getModel = env->GetStaticFieldID(cls_build, "MODEL", "Ljava/lang/String;");
        jstring jmodel = (jstring) env->GetStaticObjectField(cls_build, getModel);

        string manufacturer = jstringToChar(env, jmanufacturer);
        string model = jstringToChar(env, jmodel);
        string concatStr = manufacturer + " " + model;

        env->DeleteLocalRef(jmanufacturer);
        env->DeleteLocalRef(jmodel);

        return concatStr;
    }


    string getDeviceId(JNIEnv *env) {

        jclass cls_build = env->FindClass("android/os/Build");
        jclass cls_uuid = env->FindClass("java/util/UUID");

        // Get Serial
        jfieldID getSerial = env->GetStaticFieldID(cls_build, "SERIAL", "Ljava/lang/String;");
        jstring jserial = (jstring) env->GetStaticObjectField(cls_build, getSerial);

        // Get Board
        jfieldID getBoard = env->GetStaticFieldID(cls_build, "BOARD", "Ljava/lang/String;");
        jstring jboard = (jstring) env->GetStaticObjectField(cls_build, getBoard);

        // Get Brand
        jfieldID getBrand = env->GetStaticFieldID(cls_build, "BRAND", "Ljava/lang/String;");
        jstring jbrand = (jstring) env->GetStaticObjectField(cls_build, getBrand);

        // Get CPU_ABI
        jfieldID getABI = env->GetStaticFieldID(cls_build, "CPU_ABI", "Ljava/lang/String;");
        jstring jabi = (jstring) env->GetStaticObjectField(cls_build, getABI);

        // Get DEVICE
        jfieldID getDevice = env->GetStaticFieldID(cls_build, "DEVICE", "Ljava/lang/String;");
        jstring jdevice = (jstring) env->GetStaticObjectField(cls_build, getDevice);

        // Get MANUFACTURER
        jfieldID getManufacturer = env->GetStaticFieldID(cls_build, "MANUFACTURER",
                                                         "Ljava/lang/String;");
        jstring jmanufacturer = (jstring) env->GetStaticObjectField(cls_build, getManufacturer);

        // Get MODEL
        jfieldID getModel = env->GetStaticFieldID(cls_build, "MODEL", "Ljava/lang/String;");
        jstring jmodel = (jstring) env->GetStaticObjectField(cls_build, getModel);

        // Get PRODUCT
        jfieldID getProduct = env->GetStaticFieldID(cls_build, "PRODUCT", "Ljava/lang/String;");
        jstring jproduct = (jstring) env->GetStaticObjectField(cls_build, getProduct);

        // Get hashcode
        jclass cls_string = env->FindClass("java/lang/String");
        jmethodID getHashCode = env->GetMethodID(cls_string, "hashCode", "()I");
        jint jhashCode = env->CallIntMethod(jserial, getHashCode);

        string board = jstringToChar(env, jboard);
        string brand = jstringToChar(env, jbrand);
        string abi = jstringToChar(env, jabi);
        string device = jstringToChar(env, jdevice);
        string manufacturer = jstringToChar(env, jmanufacturer);
        string model = jstringToChar(env, jmodel);
        string product = jstringToChar(env, jproduct);

        unsigned long a = board.length() % 10;
        unsigned long b = brand.length() % 10;
        unsigned long c = abi.length() % 10;
        unsigned long d = device.length() % 10;
        unsigned long e = manufacturer.length() % 10;
        unsigned long f = model.length() % 10;
        unsigned long g = product.length() % 10;

        string m_szDevIDShort = "35" + longtoStr(a)
                                + longtoStr(b)
                                + longtoStr(c)
                                + longtoStr(d)
                                + longtoStr(e)
                                + longtoStr(f)
                                + longtoStr(g);

        jmethodID juuid = env->GetMethodID(cls_uuid, "<init>", "(JJ)V");
        jobject uuid = env->NewObject(cls_uuid, juuid, (jlong) atol(m_szDevIDShort.c_str()),
                                      static_cast<jlong>(jhashCode));

        jmethodID uuidToString = env->GetMethodID(cls_uuid, "toString", "()Ljava/lang/String;");
        jstring juuidStr = (jstring) env->CallObjectMethod(uuid, uuidToString);

        env->DeleteLocalRef(uuid);
        env->DeleteLocalRef(jboard);
        env->DeleteLocalRef(jbrand);
        env->DeleteLocalRef(jabi);
        env->DeleteLocalRef(jdevice);
        env->DeleteLocalRef(jmanufacturer);
        env->DeleteLocalRef(jmodel);
        env->DeleteLocalRef(jproduct);
        env->DeleteLocalRef(cls_build);
        env->DeleteLocalRef(cls_uuid);

        return jstringToChar(env, juuidStr);
    }


    string getAppId(JNIEnv *env, jobject obj) {

        jclass cls_emitter = env->GetObjectClass(obj);
        jfieldID getActivity = env->GetFieldID(cls_emitter, "activity", "Landroid/app/Activity;");
        jobject activity = env->GetObjectField(obj, getActivity);

        jclass cls_activity = env->FindClass("android/app/Activity");
        jmethodID getContext = env->GetMethodID(cls_activity, "getApplicationContext",
                                                "()Landroid/content/Context;");
        jobject context = env->CallObjectMethod(activity, getContext);
        jclass cls_context = env->FindClass("android/content/Context");

        jmethodID getPackagename = env->GetMethodID(cls_context, "getPackageName",
                                                    "()Ljava/lang/String;");
        jstring jpackageName = (jstring) env->CallObjectMethod(context, getPackagename);


        env->DeleteLocalRef(cls_context);
        env->DeleteLocalRef(context);

        return jstringToChar(env, jpackageName);
    }


    static bool writeSharedPref(JNIEnv *env, jobject obj, string name, string key, string value) {

        jclass cls_emitter = env->GetObjectClass(obj);
        jfieldID getActivity = env->GetFieldID(cls_emitter, "activity", "Landroid/app/Activity;");
        jobject activity = env->GetObjectField(obj, getActivity);

        jclass cls_activity = env->FindClass("android/app/Activity");
        jmethodID getSharedPreference = env->GetMethodID(cls_activity, "getSharedPreferences",
                                                         "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        jobject sharedPreference = env->CallObjectMethod(activity, getSharedPreference,
                                                         env->NewStringUTF(name.data()), 0);

        jclass cls_sp = env->FindClass("android/content/SharedPreferences");
        jmethodID getEditor = env->GetMethodID(cls_sp, "edit",
                                               "()Landroid/content/SharedPreferences$Editor;");
        jobject editor = env->CallObjectMethod(sharedPreference, getEditor);

        jclass cls_editor = env->FindClass("android/content/SharedPreferences$Editor");

        string path = getStoragePath(env, obj) + "/";

        // Encrypt
        AES aes(path);
        LOGD("CPP============", "before enc: %s", value.c_str());
        string data = aes.encrypt(value);
        LOGD("CPP============", "after enc: %s", data.c_str());
        // =====

        jmethodID putString = env->GetMethodID(cls_editor, "putString",
                                               "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");
        env->CallObjectMethod(editor, putString,
                              env->NewStringUTF(md5(key + getDeviceId(env)).data()),
                              env->NewStringUTF(data.data()));

        jmethodID commit = env->GetMethodID(cls_editor, "commit", "()Z");
        jboolean status = env->CallBooleanMethod(editor, commit);

        env->DeleteLocalRef(cls_editor);
        env->DeleteLocalRef(sharedPreference);

        return status;
    }


    static string readSharedPref(JNIEnv *env, jobject obj, string name, string key) {

        jclass cls_emitter = env->GetObjectClass(obj);
        jfieldID getActivity = env->GetFieldID(cls_emitter, "activity", "Landroid/app/Activity;");
        jobject activity = env->GetObjectField(obj, getActivity);

        jclass cls_activity = env->FindClass("android/app/Activity");
        jmethodID getSharedPreference = env->GetMethodID(cls_activity, "getSharedPreferences",
                                                         "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        jobject sharedPreference = env->CallObjectMethod(activity, getSharedPreference,
                                                         env->NewStringUTF(name.data()), 0);

        jclass cls_sp = env->FindClass("android/content/SharedPreferences");
        jmethodID getString = env->GetMethodID(cls_sp, "getString",
                                               "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        jstring value = (jstring) env->CallObjectMethod(sharedPreference, getString,
                                                        env->NewStringUTF(
                                                                md5(key + getDeviceId(env)).data()),
                                                        env->NewStringUTF("NULL"));

        env->DeleteLocalRef(cls_sp);
        env->DeleteLocalRef(sharedPreference);

        // Decrypt
        string path = getStoragePath(env, obj) + "/";

        AES aes(path);
        string data = jstringToChar(env, value);

        if (strcmp(data.c_str(), "NULL") != 0) {
            LOGD("CPP============", "before dec: %s", data.c_str());
            data = aes.decrypt(data);
            LOGD("CPP============", "after dec: %s", data.c_str());
        }

        return data;
    }

    bool registerDev(JNIEnv *env, jobject obj, jstring jUserId) {
        // Get required info
        string packageName = "";
        if (obj != NULL) {
            packageName = getAppId(env, obj);
            LOGD("app", "name: %s", packageName.c_str());
        }

        string uuidStr = getDeviceId(env);
        LOGD("id", "uuid: %s", uuidStr.c_str());

        string model = getModel(env);

        char osVersionChar[PROP_VALUE_MAX];
        int osVersionLength = __system_property_get("ro.build.version.release", osVersionChar);

        string osVersion(osVersionChar);
        LOGD("os", "%s", osVersion.c_str());
        string userId = jstringToChar(env, jUserId);
        LOGD("userid", "%s", userId.c_str());

        //string payload = {"deviceID": "xxx", "deviceModel": "xxx", "osVersion": "xxx", "sdkName": "", "sdkVersion": "", "appID": "", "userID": "xxx"};
        string payload = generateString(10) + uuidStr + generateString(12) + model + generateString(43) + osVersion + generateString(45) + generateString(50) + generateString(46) + generateString(51) + generateString(47) + getAppId() + generateString(13) + userId + generateString(14);
        LOGD("payload", "%s", payload.c_str());
        // Send request

        Json::Value response = httpRequest(generateString(15).c_str(), generateString(2).c_str(), payload); // "https://apn.aimazing.com.sg/activation/courier", "POST"

        // Init return value
        jobjectArray returnValue = (jobjectArray)env->NewObjectArray(2, env->FindClass("java/lang/String"),env->NewStringUTF(""));
        env->SetObjectArrayElement(returnValue, 0, env->NewStringUTF(response["code"].asString().c_str()));
        env->SetObjectArrayElement(returnValue, 1, env->NewStringUTF(response["body"].asString().c_str()));

        string statusCode = response[generateString(6)].asString().c_str(); // "code"
        if (statusCode.compare(generateString(16)) == 0) {
            // Register successful
            string path = getStoragePath(env, obj) + "/";

            // Generate table
            tableGenerator tb;
            LOGE("registerDev", "before generatetable()");
            tb.generate_table(path);

            Json::Reader jsonReader;
            Json::Value bodyObject;
            if (!jsonReader.parse(response["body"].asString().c_str(), bodyObject)) {
                LOGE("registerDev", "Could not parse HTTP data as JSON");
                LOGE("registerDev", "HTTP data was: %s", response["body"].asString().c_str());
                return false;
            }

            // Write shared preference
            writeSharedPref(env, obj, getDeviceId(env), generateString(20), caCertPtr); // "cacertPath"
            writeSharedPref(env, obj, getDeviceId(env), generateString(41), bodyObject[generateString(41)].asString()); // "sdkToken"
            writeSharedPref(env, obj, getDeviceId(env), generateString(42), userId); // "userId"


            return true;
        }

        return false;
    }


    jboolean isActivated(JNIEnv *env, jobject obj) {
        string sdkToken = readSharedPref(env, obj, getDeviceId(env), generateString(41));
        string userId = readSharedPref(env, obj, getDeviceId(env), generateString(42));

        LOGD("read sharedpref", "value: %s", sdkToken.c_str());


        // Check if Token exists
        if (sdkToken.compare("NULL") == 0 || userId.compare("NULL") == 0) {
            return JNI_FALSE;
        }

        return JNI_TRUE;
    }


    jboolean activate(JNIEnv *env, jobject obj, jstring UserId) {

        setDir(env, obj);
        bool res = registerDev(env, obj, UserId);

        return (jboolean) res;
    }


    bool isRooted(JNIEnv *env) {
        jclass class_debug = env->FindClass("android/os/Debug");
        jmethodID method_debug = env->GetStaticMethodID(class_debug, "isDebuggerConnected", "()Z");
        jboolean bool_debug = env->CallStaticBooleanMethod(class_debug, method_debug);

        if (bool_debug) {
            return true;
        }

        // check if being traced
        std::ifstream fin(generateString(22).c_str());
        std::string tmp;
        while (fin >> tmp) {
            if (tmp == generateString(23)) {
                fin >> tmp;
                if (tmp != "0") {
                    return true;
                }

                break;
            }
        }

        // execute command, but too slow?
        FILE *file;
        /*char buffer[128];
        const std::string test_exe[] = {"which su", "pm list packages | grep supersu", "pm list packages | grep superuser", "pm list packages | grep android.su",
                     "pm list packages | grep temprootremovejb", "pm list packages | grep os.ZygoteInit", "pm list packages | grep saurik.substrate.MS$2", "pm list packages | grep xposed"};
        int siz = sizeof(test_exe) / sizeof(test_exe[0]);
        for(int i=0; i<siz; i++){
            file = popen(test_exe[i].c_str(), "r");
            if(fgets(buffer, 128, file) != NULL) {
                pclose(file);
                return true;
            }
            pclose(file);
        }*/

        // find if file exits
        const std::string paths[] = {"/system/xbin/daemonsu", "/system/etc/init.d/99SuperSUDaemon",
                                     "/system/app/Superuser.apk", "/sbin/su", "/system/bin/su",
                                     "/system/xbin/su", "/data/local/xbin/su",
                                     "/data/local/bin/su", "/system/sd/xbin/su",
                                     "/system/bin/failsafe/su", "/data/local/su", "/su/bin/su"};
        int siz = sizeof(paths) / sizeof(paths[0]);
        for (int i = 0; i < siz; i++) {
            if (FILE * file = fopen(paths[i].c_str(), "r")) {
                fclose(file);
                return true;
            }
        }

        // build tag method text = android.os.Build.TAGS;
        jclass class_build = env->FindClass("android/os/Build");
        jfieldID field_build = env->GetStaticFieldID(class_build, "TAGS", "Ljava/lang/String;");
        jstring jstring_build = (jstring) env->GetStaticObjectField(class_build, field_build);
        std::string string_build = env->GetStringUTFChars(jstring_build, 0);

        if (string_build != "release-keys") {
            return true;
        }


        return false;
    }

    char *jstringToChar(JNIEnv *env, jstring jstr) {

        char *rtn = NULL;
        jclass clsstring = env->FindClass("java/lang/String");
        jstring strencode = env->NewStringUTF("utf-8");
        jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
        jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
        jsize alen = env->GetArrayLength(barr);
        jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);

        if (alen > 0) {
            rtn = (char *) malloc(alen + 1);
            memcpy(rtn, ba, alen);
            rtn[alen] = 0;
        }

        env->ReleaseByteArrayElements(barr, ba, 0);
        env->DeleteLocalRef(clsstring);
        env->DeleteLocalRef(strencode);

        return rtn;
    }

    jstring getSdkToken(JNIEnv *env, jobject obj){
        string token = readSharedPref(env, obj, getDeviceId(env), generateString(41));
        return env->NewStringUTF(token.c_str());
    }

    jstring getUserId(JNIEnv *env, jobject obj){
        string token = readSharedPref(env, obj, getDeviceId(env), generateString(42));
        return env->NewStringUTF(token.c_str());
    }

}