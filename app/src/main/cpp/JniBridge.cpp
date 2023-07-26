#include "Instance.h"
#include "PhysicalDevice.h"
#include "VkInfo.h"
#include <jni.h>
#include <string>
#include <sstream>

/**
 * Parse the apiVersion as a string in the format of Variant.Major.Minor.Patch.
 * @param apiVersion The apiVersion from <code>VkPhysicalDeviceProperties</code>.
 * @return the apiVersion as a string in the format of Variant.Major.Minor.Patch.
 */
std::string getApiVersionAsString(const uint32_t apiVersion)
{
    std::ostringstream ss;
    ss << VK_API_VERSION_VARIANT(apiVersion) << ".";
    ss << VK_API_VERSION_MAJOR(apiVersion) << ".";
    ss << VK_API_VERSION_MINOR(apiVersion) << ".";
    ss << VK_API_VERSION_PATCH(apiVersion);

    return ss.str();
}

/**
 * Parses a value into hexadecimal form and formats it as a string prefixed with 0x.
 * @param value The value to be converted.
 * @return the hex value formatted as a string and prefixed with 0x.
 */
std::string asHexString(const uint32_t value)
{
    std::ostringstream ss;
    ss << "0x" << std::hex << value;
    return ss.str();
}

/**
 * Initializes a <code>VkInstance</code> on the device.
 * @param env The JNI environment.
 * @param app_name The application name to give to the <code>VkInstance</code>.
 * @param engine_name The engine name to give to the <code>VkInstance</code>.
 * @return the <code>Instance</code> object.
 */
Instance getInstance(JNIEnv *env, const jstring app_name, const jstring engine_name)
{
    std::string appName(env->GetStringUTFChars(app_name, nullptr));
    std::string engineName(env->GetStringUTFChars(engine_name, nullptr));

    std::vector<const char*> extensions = {};
    std::vector<const char*> layers = {};

    return Instance(appName, engineName, extensions, layers);
}

/**
 * Gets a Java object instance based on the parameters provided.
 * @param env The JNI environment.
 * @param className The fully qualified Java class name.
 * @param constructorSig The signature of the class constructor as defined by the JNI.
 * @return the Java object instance.
 */
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

/**
 * Populates the fields of the <code>InstanceInfo</code> Java object.
 * @param env The JNI environment.
 * @param instance The <code>VkInstance</code> that contains the Vulkan instance data.
 * @param instanceInfoObject (OUT param) The <code>InstanceInfo</code> Java object to populate.
 */
void populateInstanceInfoObject(JNIEnv *env, const Instance* instance, jobject instanceInfoObject)
{
    jclass instanceInfoClass = env->FindClass("com/example/vulkaninfoapp/InstanceInfo");

    jfieldID fidNumber = env->GetFieldID(instanceInfoClass, "appName", "Ljava/lang/String;");
    jstring appName = env->NewStringUTF(instance->getAppName().c_str());
    env->SetObjectField(instanceInfoObject, fidNumber, appName);

    fidNumber = env->GetFieldID(instanceInfoClass, "engineName", "Ljava/lang/String;");
    jstring engineNameFromInstance = env->NewStringUTF(instance->getEngineName().c_str());
    env->SetObjectField(instanceInfoObject, fidNumber, engineNameFromInstance);

    fidNumber = env->GetFieldID(instanceInfoClass, "numDevices", "I");
    env->SetIntField(instanceInfoObject, fidNumber, (int)instance->getNumberPhysicalDevices());
}

/**
 * Populates the fields of the <code>PhysicalDeviceProperties</code> Java object.
 * @param env The JNI environment.
 * @param properties The <code>VkPhysicalDeviceProperties</code> that contains the Vulkan device properties.
 * @param obj (OUT param) The <code>PhysicalDeviceProperties</code> Java object to populate.
 */
void populatePhysicalDevicePropertiesObject(JNIEnv *env, const VkPhysicalDeviceProperties properties, jobject obj)
{
    jclass propertiesClazz = env->FindClass("com/example/vulkaninfoapp/PhysicalDeviceProperties");

    jfieldID fidNumber = env->GetFieldID(propertiesClazz, "apiVersion", "Ljava/lang/String;");
    jstring apiVersion = env->NewStringUTF(getApiVersionAsString(properties.apiVersion).c_str());
    env->SetObjectField(obj, fidNumber, apiVersion);

    fidNumber = env->GetFieldID(propertiesClazz, "driverVersion", "Ljava/lang/String;");
    jstring driverVersion = env->NewStringUTF(asHexString(properties.driverVersion).c_str());
    env->SetObjectField(obj, fidNumber, driverVersion);

    fidNumber = env->GetFieldID(propertiesClazz, "vendorId", "Ljava/lang/String;");
    jstring vendorId = env->NewStringUTF(asHexString(properties.vendorID).c_str());
    env->SetObjectField(obj, fidNumber, vendorId);

    fidNumber = env->GetFieldID(propertiesClazz, "deviceId", "Ljava/lang/String;");
    jstring deviceId = env->NewStringUTF(asHexString(properties.deviceID).c_str());
    env->SetObjectField(obj, fidNumber, deviceId);

    fidNumber = env->GetFieldID(propertiesClazz, "physicalDeviceType", "I");
    env->SetIntField(obj, fidNumber, (jint)properties.deviceType);

    fidNumber = env->GetFieldID(propertiesClazz, "deviceName", "Ljava/lang/String;");
    jstring deviceName = env->NewStringUTF(properties.deviceName);
    env->SetObjectField(obj, fidNumber, deviceName);
}

/**
 * Populates the fields of the <code>PhysicalDeviceLimits</code> Java object.
 * @param env The JNI environment.
 * @param limits The <code>VkPhysicalDeviceLimits</code> that contains the Vulkan device limits.
 * @param obj (OUT param) The <code>PhysicalDeviceLimits</code> Java object to populate.
 */
void populatePhysicalDeviceLimitsObject(JNIEnv* env, const VkPhysicalDeviceLimits limits, jobject obj)
{
    jclass limitsClazz = env->FindClass("com/example/vulkaninfoapp/PhysicalDeviceLimits");

    jfieldID fidNumber = env->GetFieldID(limitsClazz, "maxImageDimension1D", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxImageDimension1D);

    fidNumber = env->GetFieldID(limitsClazz, "maxImageDimension2D", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxImageDimension2D);

    fidNumber = env->GetFieldID(limitsClazz, "maxImageDimension3D", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxImageDimension3D);

    fidNumber = env->GetFieldID(limitsClazz, "maxImageDimensionCube", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxImageDimensionCube);

    fidNumber = env->GetFieldID(limitsClazz, "maxImageArrayLayers", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxImageArrayLayers);

    fidNumber = env->GetFieldID(limitsClazz, "maxTexelBufferElements", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxTexelBufferElements);

    fidNumber = env->GetFieldID(limitsClazz, "maxUniformBufferRange", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxUniformBufferRange);

    fidNumber = env->GetFieldID(limitsClazz, "maxStorageBufferRange", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxStorageBufferRange);

    fidNumber = env->GetFieldID(limitsClazz, "maxPushConstantsSize", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxPushConstantsSize);

    fidNumber = env->GetFieldID(limitsClazz, "maxMemoryAllocationCount", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxMemoryAllocationCount);

    fidNumber = env->GetFieldID(limitsClazz, "maxSamplerAllocationCount", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxSamplerAllocationCount);

    fidNumber = env->GetFieldID(limitsClazz, "bufferImageGranularity", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.bufferImageGranularity);

    fidNumber = env->GetFieldID(limitsClazz, "sparseAddressSpaceSize", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.sparseAddressSpaceSize);

    fidNumber = env->GetFieldID(limitsClazz, "maxBoundDescriptorSets", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxBoundDescriptorSets);

    fidNumber = env->GetFieldID(limitsClazz, "maxPerStageDescriptorSamplers", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxPerStageDescriptorSamplers);

    fidNumber = env->GetFieldID(limitsClazz, "maxPerStageDescriptorUniformBuffers", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxPerStageDescriptorUniformBuffers);

    fidNumber = env->GetFieldID(limitsClazz, "maxPerStageDescriptorStorageBuffers", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxPerStageDescriptorStorageBuffers);

    fidNumber = env->GetFieldID(limitsClazz, "maxPerStageDescriptorSampledImages", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxPerStageDescriptorSampledImages);

    // TODO: Continue here populating limits. Forever...
}

/**
 * Populates the fields of the <code>PhysicalDeviceSparseLimits</code> Java object.
 * @param env The JNI environment.
 * @param properties The <code>VkPhysicalDeviceSparseProperties</code> that contains the properties.
 * @param obj (OUT param) The <code>PhysicalDeviceSparseProperties</code> Java object to populate.
 */
void populatePhysicalDeviceSparsePropertiesObject(JNIEnv* env, const VkPhysicalDeviceSparseProperties properties, jobject obj)
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

    fidNumber = env->GetFieldID(propertiesClazz, "residencyNonResidentStrict", "Z");
    env->SetBooleanField(obj, fidNumber, (jboolean)properties.residencyNonResidentStrict);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_vulkaninfoapp_MainActivity_getVkInfo(JNIEnv *env, jclass clazz,
                                                      jstring app_name, jstring engine_name)
{
    const std::string VkInfoClassName = "com/example/vulkaninfoapp/VkInfo";
    const std::string InstanceInfoClassName = "com/example/vulkaninfoapp/InstanceInfo";
    const std::string PhysicalDevicePropertiesClassName = "com/example/vulkaninfoapp/PhysicalDeviceProperties";
    const std::string PhysicalDeviceLimitsClassName = "com/example/vulkaninfoapp/PhysicalDeviceLimits";
    const std::string PhysicalDeviceSparsePropertiesClassName = "com/example/vulkaninfoapp/PhysicalDeviceSparseProperties";

    const std::string InstanceInfoClassSignature = "Lcom/example/vulkaninfoapp/InstanceInfo;";
    const std::string PhysicalDevicePropertiesClassSignature = "Lcom/example/vulkaninfoapp/PhysicalDeviceProperties;";
    const std::string PhysicalDeviceLimitsClassSignature = "Lcom/example/vulkaninfoapp/PhysicalDeviceLimits;";
    const std::string PhysicalDeviceSparsePropertiesClassSignature = "Lcom/example/vulkaninfoapp/PhysicalDeviceSparseProperties;";

    VkInfo vkInfo;
    vkInfo.instance = new Instance(env->GetStringUTFChars(app_name, nullptr), env->GetStringUTFChars(engine_name, nullptr), {}, {});
    if (vkInfo.instance->getHandle() == VK_NULL_HANDLE || vkInfo.instance->getNumberPhysicalDevices() == 0)
    {
        return nullptr;
    }

    // For now this app is only designed to run on SoC devices which have only 1 GPU. So just choose the first one.
    vkInfo.selectedPhysicalDevice = vkInfo.instance->getPhysicalDevices()[0];
    vkInfo.physicalDeviceProperties = PhysicalDevice::getDeviceProperties(vkInfo.selectedPhysicalDevice);

    // Get an instance of the VkInfo Java object.
    jobject vkInfoObject = getObject(env, VkInfoClassName.c_str());

    // Populate the instanceInfo Java object.
    jfieldID fieldId = env->GetFieldID(env->FindClass(VkInfoClassName.c_str()), "instanceInfo", InstanceInfoClassSignature.c_str());
    jobject instanceInfoObject = getObject(env, InstanceInfoClassName.c_str());
    populateInstanceInfoObject(env, vkInfo.instance, instanceInfoObject);
    env->SetObjectField(vkInfoObject, fieldId, instanceInfoObject);

    // Populate the physicalDeviceProperties Java object.
    jobject physicalDevicePropertiesObject = getObject(env, PhysicalDevicePropertiesClassName.c_str());

    // Populate physicalDeviceProperties.limits Java object.
    fieldId = env->GetFieldID(env->FindClass(PhysicalDevicePropertiesClassName.c_str()), "physicalDeviceLimits", PhysicalDeviceLimitsClassSignature.c_str());
    jobject physicalDeviceLimitsObject = getObject(env, PhysicalDeviceLimitsClassName.c_str());
    populatePhysicalDeviceLimitsObject(env, vkInfo.physicalDeviceProperties.limits, physicalDeviceLimitsObject);
    env->SetObjectField(physicalDevicePropertiesObject, fieldId, physicalDeviceLimitsObject);

    // Populate physicalDeviceProperties.sparseProperties Java object.
    fieldId = env->GetFieldID(env->FindClass(PhysicalDevicePropertiesClassName.c_str()), "physicalDeviceSparseProperties", PhysicalDeviceSparsePropertiesClassSignature.c_str());
    jobject physicalDeviceSparsePropertiesObject = getObject(env, PhysicalDeviceSparsePropertiesClassName.c_str());
    populatePhysicalDeviceSparsePropertiesObject(env, vkInfo.physicalDeviceProperties.sparseProperties, physicalDeviceSparsePropertiesObject);
    env->SetObjectField(physicalDevicePropertiesObject, fieldId, physicalDeviceSparsePropertiesObject);

    // Set the physicalDeviceProperties field of the Java VkInfo object.
    populatePhysicalDevicePropertiesObject(env, vkInfo.physicalDeviceProperties, physicalDevicePropertiesObject);
    fieldId = env->GetFieldID(env->FindClass(VkInfoClassName.c_str()), "physicalDeviceProperties", PhysicalDevicePropertiesClassSignature.c_str());
    env->SetObjectField(vkInfoObject, fieldId, physicalDevicePropertiesObject);

    delete vkInfo.instance;
    return vkInfoObject;
}