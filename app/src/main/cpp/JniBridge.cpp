#include "Instance.h"
#include "PhysicalDevice.h"
#include "PhysicalDeviceType.hpp"
#include "LogicalDevice.h"
#include "InstanceInfo.hpp"
#include "VkInfo.h"
#include <jni.h>
#include <string>

Instance initInstance(JNIEnv *env, jstring app_name, jstring engine_name)
{
    std::string appName(env->GetStringUTFChars(app_name, nullptr));
    std::string engineName(env->GetStringUTFChars(engine_name, nullptr));

    std::vector<const char*> extensions = {};
    std::vector<const char*> layers = {};
    Instance instance = Instance(appName, engineName, extensions, layers);

    return instance;
}

jobject getObject(JNIEnv *env, const char* className, const char* constructorSig = "()V")
{
    jclass clazz = env->FindClass(className);
    if (clazz == nullptr)
    {
        return nullptr;
    }

    jmethodID constructId = env->GetMethodID(clazz, "<init>", constructorSig);
    if (constructId == nullptr)
    {
        return nullptr;
    }

    jobject objectInst = env->NewObject(clazz, constructId);
    if (objectInst == nullptr)
    {
        return nullptr;
    }

    return objectInst;
}

InstanceInfo getInstanceInfo(const Instance& instance)
{
    InstanceInfo instanceInfo = {};

    instanceInfo.instance = instance.getInstance();
    instanceInfo.appName = instance.getAppName();
    instanceInfo.engineName = instance.getEngineName();
    instanceInfo.deviceCount = instance.getNumberPhysicalDevices();
    instanceInfo.devices.resize(instanceInfo.deviceCount);
    instanceInfo.devices = instance.getPhysicalDevices();

    return instanceInfo;
}

void populateInstanceInfoObject(JNIEnv *env, const InstanceInfo& instanceInfo, jobject instanceInfoObject)
{
    jclass instanceInfoClass = env->FindClass("com/example/vulkaninfoapp/InstanceInfo");

    jfieldID fidNumber = env->GetFieldID(instanceInfoClass, "appName", "Ljava/lang/String;");
    jstring appName = env->NewStringUTF(instanceInfo.appName.c_str());
    env->SetObjectField(instanceInfoObject, fidNumber, appName);

    fidNumber = env->GetFieldID(instanceInfoClass, "engineName", "Ljava/lang/String;");
    jstring engineNameFromInstance = env->NewStringUTF(instanceInfo.engineName.c_str());
    env->SetObjectField(instanceInfoObject, fidNumber, engineNameFromInstance);

    fidNumber = env->GetFieldID(instanceInfoClass, "numDevices", "I");
    env->SetIntField(instanceInfoObject, fidNumber, (int)instanceInfo.deviceCount);
}

VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice physicalDevice)
{
    return PhysicalDevice::getDeviceProperties(physicalDevice);
}

void populatePhysicalDevicePropertiesObject(JNIEnv *env, VkPhysicalDeviceProperties properties, jobject obj)
{
    jclass propertiesClazz = env->FindClass("com/example/vulkaninfoapp/PhysicalDeviceProperties");
    jclass deviceTypeClazz = env->FindClass("com/example/vulkaninfoapp/PhysicalDeviceType");
    jfieldID fidNumber = env->GetFieldID(propertiesClazz, "physicalDeviceType", "Lcom/example/vulkaninfoapp/PhysicalDeviceType;");

    fidNumber = env->GetFieldID(propertiesClazz, "apiVersion", "J");
    env->SetLongField(obj, fidNumber, (jlong)properties.apiVersion);

    fidNumber = env->GetFieldID(propertiesClazz, "driverVersion", "J");
    env->SetLongField(obj, fidNumber, (jlong)properties.driverVersion);

    fidNumber = env->GetFieldID(propertiesClazz, "vendorId", "J");
    env->SetLongField(obj, fidNumber, (jlong)properties.vendorID);

    fidNumber = env->GetFieldID(propertiesClazz, "deviceId", "J");
    env->SetLongField(obj, fidNumber, (jlong)properties.deviceID);

    const char* deviceType = PhysicalDeviceType::PhysicalDeviceTypeNames[properties.deviceType].c_str();
    fidNumber = env->GetStaticFieldID(deviceTypeClazz , deviceType, "Lcom/example/vulkaninfoapp/PhysicalDeviceType;");
    jobject deviceTypeObj = env->GetStaticObjectField(deviceTypeClazz, fidNumber);
    env->SetStaticObjectField(deviceTypeClazz, fidNumber, deviceTypeObj);

    fidNumber = env->GetFieldID(propertiesClazz, "deviceName", "Ljava/lang/String;");
    jstring deviceName = env->NewStringUTF(properties.deviceName);
    env->SetObjectField(obj, fidNumber, deviceName);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_vulkaninfoapp_MainActivity_getVkInfo(JNIEnv *env, jclass clazz,
                                                      jstring app_name, jstring engine_name)
{
    const std::string VK_INFO_CLASS_NAME = "com/example/vulkaninfoapp/VkInfo";
    const std::string INSTANCE_INFO_CLASS_SIGNATURE = "Lcom/example/vulkaninfoapp/InstanceInfo;";
    const std::string PHYSICAL_DEVICE_PROPERTIES_CLASS_SIGNATURE = "Lcom/example/vulkaninfoapp/PhysicalDeviceProperties;";
    VkInfo vkInfo = {};
    Instance instance = initInstance(env, app_name, engine_name);
    if (instance.getInstance() == VK_NULL_HANDLE)
    {
        return nullptr;
    }

    vkInfo.instanceInfo = getInstanceInfo(instance);
    vkInfo.selectedPhysicalDevice = vkInfo.instanceInfo.devices[0];
    vkInfo.physicalDeviceProperties = getPhysicalDeviceProperties(vkInfo.selectedPhysicalDevice);

    jobject vkInfoObject = getObject(env, VK_INFO_CLASS_NAME.c_str());

    jfieldID fidNumber = env->GetFieldID(env->FindClass(VK_INFO_CLASS_NAME.c_str()), "instanceInfo", INSTANCE_INFO_CLASS_SIGNATURE.c_str());
    jobject instanceInfoObject = getObject(env, "com/example/vulkaninfoapp/InstanceInfo");
    populateInstanceInfoObject(env, vkInfo.instanceInfo, instanceInfoObject);
    env->SetObjectField(vkInfoObject, fidNumber, instanceInfoObject);

    fidNumber = env->GetFieldID(env->FindClass(VK_INFO_CLASS_NAME.c_str()), "physicalDeviceProperties", PHYSICAL_DEVICE_PROPERTIES_CLASS_SIGNATURE.c_str());
    jobject physicalDevicePropertiesObject = getObject(env, "com/example/vulkaninfoapp/PhysicalDeviceProperties");
    populatePhysicalDevicePropertiesObject(env, vkInfo.physicalDeviceProperties, physicalDevicePropertiesObject);
    env->SetObjectField(vkInfoObject, fidNumber, physicalDevicePropertiesObject);

    return vkInfoObject;
}