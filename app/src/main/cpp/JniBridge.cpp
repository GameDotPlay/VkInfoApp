#include "Instance.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "InstanceInfo.cpp"
#include "VkInfo.h"
#include <jni.h>
#include <string>



extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_vulkaninfoapp_MainActivity_initInstance(JNIEnv *env, jclass clazz,
                                                         jstring app_name, jstring engine_name) {
    std::string appName(env->GetStringUTFChars(app_name, nullptr));
    std::string engineName(env->GetStringUTFChars(engine_name, nullptr));

    std::vector<const char*> extensions = {};
    std::vector<const char*> layers = {};
    Instance instance(appName, engineName, extensions, layers);
    return env->NewStringUTF(instance.getAppName().c_str());
}