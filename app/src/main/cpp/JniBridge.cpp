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

    jfieldID fidNumber = env->GetFieldID(propertiesClazz, "apiVersion", "J");
    env->SetLongField(obj, fidNumber, (jlong)properties.apiVersion);

    fidNumber = env->GetFieldID(propertiesClazz, "driverVersion", "J");
    env->SetLongField(obj, fidNumber, (jlong)properties.driverVersion);

    fidNumber = env->GetFieldID(propertiesClazz, "vendorId", "J");
    env->SetLongField(obj, fidNumber, (jlong)properties.vendorID);

    fidNumber = env->GetFieldID(propertiesClazz, "deviceId", "J");
    env->SetLongField(obj, fidNumber, (jlong)properties.deviceID);

    fidNumber = env->GetFieldID(propertiesClazz, "physicalDeviceType", "I");
    env->SetIntField(obj, fidNumber, (jint)properties.deviceType);

    fidNumber = env->GetFieldID(propertiesClazz, "deviceName", "Ljava/lang/String;");
    jstring deviceName = env->NewStringUTF(properties.deviceName);
    env->SetObjectField(obj, fidNumber, deviceName);
}

void populatePhysicalDeviceSparsePropertiesObject(JNIEnv* env, VkPhysicalDeviceSparseProperties properties, jobject obj)
{
    jclass propertiesClazz = env->FindClass("com/example/vulkaninfoapp/PhysicalDeviceSparseProperties");

    jfieldID fidNumber = env->GetFieldID(propertiesClazz, "residencyStandard2DBlockShape", "Z");
    env->SetBooleanField(obj, fidNumber, (jboolean)properties.residencyStandard2DBlockShape);

    fidNumber = env->GetFieldID(propertiesClazz, "residencyStandard2DMultisampleBlockShape", "Z");
    env->SetBooleanField(obj, fidNumber, (jboolean)properties.residencyStandard2DMultisampleBlockShape);

    fidNumber = env->GetFieldID(propertiesClazz, "residencyStandard3DBlockShape", "Z");
    env->SetBooleanField(obj, fidNumber, (jboolean)properties.residencyStandard3DBlockShape);

    fidNumber = env->GetFieldID(propertiesClazz, "residencyAlignedMipSize", "Z");
    env->SetBooleanField(obj, fidNumber, (jboolean)properties.residencyAlignedMipSize);

    fidNumber = env->GetFieldID(propertiesClazz, "residencyNonResidentScrict", "Z");
    env->SetBooleanField(obj, fidNumber, (jboolean)properties.residencyNonResidentStrict);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_vulkaninfoapp_MainActivity_getVkInfo(JNIEnv *env, jclass clazz,
                                                      jstring app_name, jstring engine_name)
{
    const std::string VK_INFO_CLASS_NAME = "com/example/vulkaninfoapp/VkInfo";
    const std::string INSTANCE_INFO_CLASS_NAME = "com/example/vulkaninfoapp/InstanceInfo";
    const std::string PHYSICAL_DEVICE_PROPERTIES_CLASS_NAME = "com/example/vulkaninfoapp/PhysicalDeviceProperties";
    const std::string PHYSICAL_DEVICE_SPARSE_PROPERTIES_CLASS_NAME = "com/example/vulkaninfoapp/PhysicalDeviceSparseProperties";

    const std::string INSTANCE_INFO_CLASS_SIGNATURE = "Lcom/example/vulkaninfoapp/InstanceInfo;";
    const std::string PHYSICAL_DEVICE_PROPERTIES_CLASS_SIGNATURE = "Lcom/example/vulkaninfoapp/PhysicalDeviceProperties;";
    const std::string PHYSICAL_DEVICE_SPARSE_PROPERTIES_CLASS_SIGNATURE = "Lcom/example/vulkaninfoapp/PhysicalDeviceSparseProperties;";
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
    jobject instanceInfoObject = getObject(env, INSTANCE_INFO_CLASS_NAME.c_str());
    populateInstanceInfoObject(env, vkInfo.instanceInfo, instanceInfoObject);
    env->SetObjectField(vkInfoObject, fidNumber, instanceInfoObject);

    jobject physicalDevicePropertiesObject = getObject(env, PHYSICAL_DEVICE_PROPERTIES_CLASS_NAME.c_str());

    fidNumber = env->GetFieldID(env->FindClass(PHYSICAL_DEVICE_PROPERTIES_CLASS_NAME.c_str()), "physicalDeviceSparseProperties", PHYSICAL_DEVICE_SPARSE_PROPERTIES_CLASS_SIGNATURE.c_str());
    jobject physicalDeviceSparsePropertiesObject = getObject(env, PHYSICAL_DEVICE_SPARSE_PROPERTIES_CLASS_NAME.c_str());
    populatePhysicalDeviceSparsePropertiesObject(env, vkInfo.physicalDeviceProperties.sparseProperties, physicalDeviceSparsePropertiesObject);
    env->SetObjectField(physicalDevicePropertiesObject, fidNumber, physicalDeviceSparsePropertiesObject);

    populatePhysicalDevicePropertiesObject(env, vkInfo.physicalDeviceProperties, physicalDevicePropertiesObject);
    fidNumber = env->GetFieldID(env->FindClass(VK_INFO_CLASS_NAME.c_str()), "physicalDeviceProperties", PHYSICAL_DEVICE_PROPERTIES_CLASS_SIGNATURE.c_str());
    env->SetObjectField(vkInfoObject, fidNumber, physicalDevicePropertiesObject);

    return vkInfoObject;
}