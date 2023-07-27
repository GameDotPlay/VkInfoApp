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
 * Gets a sample count description based on the <code>VkSampleCountFlagBit</code> provided.
 * @param sampleCountBit The <code>VkSampleCountFlagBit</code> to parse.
 * @return the description of the <code>VkSampleCountFlagBit</code> value.
 */
std::string getSampleCountDescription(const VkSampleCountFlags sampleCountFlags)
{
    std::string description;
    VkSampleCountFlagBits sampleCountBit = (VkSampleCountFlagBits)sampleCountFlags;

    switch (sampleCountBit)
    {
        case 0x00000001:
            description = "1 sample per pixel";
            break;
        case 0x00000002:
            description = "2 samples per pixel";
            break;
        case 0x00000004:
            description = "4 samples per pixel";
            break;
        case 0x00000008:
            description = "8 samples per pixel";
            break;
        case 0x00000010:
            description = "16 samples per pixel";
            break;
        case 0x00000020:
            description = "32 samples per pixel";
            break;
        case 0x00000040:
            description = "64 samples per pixel";
            break;
        default:
            description = "Unknown SampleCountFlagBit value";
            break;
    }

    return description;
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

    fidNumber = env->GetFieldID(limitsClazz, "maxPerStageDescriptorStorageImages", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxPerStageDescriptorStorageImages);

    fidNumber = env->GetFieldID(limitsClazz, "maxPerStageDescriptorInputAttachments", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxPerStageDescriptorInputAttachments);

    fidNumber = env->GetFieldID(limitsClazz, "maxPerStageResources", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxPerStageResources);

    fidNumber = env->GetFieldID(limitsClazz, "maxDescriptorSetSamplers", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxDescriptorSetSamplers);

    fidNumber = env->GetFieldID(limitsClazz, "maxDescriptorSetUniformBuffers", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxDescriptorSetUniformBuffers);

    fidNumber = env->GetFieldID(limitsClazz, "maxDescriptorSetUniformBuffersDynamic", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxDescriptorSetUniformBuffers);

    fidNumber = env->GetFieldID(limitsClazz, "maxDescriptorSetStorageBuffers", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxDescriptorSetStorageBuffers);

    fidNumber = env->GetFieldID(limitsClazz, "maxDescriptorSetStorageBuffersDynamic", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxDescriptorSetStorageBuffersDynamic);

    fidNumber = env->GetFieldID(limitsClazz, "maxDescriptorSetSampledImages", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxDescriptorSetSampledImages);

    fidNumber = env->GetFieldID(limitsClazz, "maxDescriptorSetStorageImages", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxDescriptorSetStorageImages);

    fidNumber = env->GetFieldID(limitsClazz, "maxDescriptorSetInputAttachments", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxDescriptorSetInputAttachments);

    fidNumber = env->GetFieldID(limitsClazz, "maxVertexInputAttributes", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxVertexInputAttributes);

    fidNumber = env->GetFieldID(limitsClazz, "maxVertexInputBindings", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxVertexInputBindings);

    fidNumber = env->GetFieldID(limitsClazz, "maxVertexInputAttributeOffset", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxVertexInputAttributeOffset);

    fidNumber = env->GetFieldID(limitsClazz, "maxVertexInputBindingStride", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxVertexInputBindingStride);

    fidNumber = env->GetFieldID(limitsClazz, "maxVertexOutputComponents", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxVertexOutputComponents);

    fidNumber = env->GetFieldID(limitsClazz, "maxTessellationGenerationLevel", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxTessellationGenerationLevel);

    fidNumber = env->GetFieldID(limitsClazz, "maxTessellationPatchSize", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxTessellationPatchSize);

    fidNumber = env->GetFieldID(limitsClazz, "maxTessellationControlPerVertexInputComponents", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxTessellationControlPerVertexInputComponents);

    fidNumber = env->GetFieldID(limitsClazz, "maxTessellationControlPerVertexOutputComponents", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxTessellationControlPerVertexOutputComponents);

    fidNumber = env->GetFieldID(limitsClazz, "maxTessellationControlPerPatchOutputComponents", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxTessellationControlPerPatchOutputComponents);

    fidNumber = env->GetFieldID(limitsClazz, "maxTessellationControlTotalOutputComponents", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxTessellationControlTotalOutputComponents);

    fidNumber = env->GetFieldID(limitsClazz, "maxTessellationEvaluationInputComponents", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxTessellationEvaluationInputComponents);

    fidNumber = env->GetFieldID(limitsClazz, "maxTessellationEvaluationOutputComponents", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxTessellationEvaluationOutputComponents);

    fidNumber = env->GetFieldID(limitsClazz, "maxGeometryShaderInvocations", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxGeometryShaderInvocations);

    fidNumber = env->GetFieldID(limitsClazz, "maxGeometryInputComponents", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxGeometryInputComponents);

    fidNumber = env->GetFieldID(limitsClazz, "maxGeometryOutputComponents", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxGeometryOutputComponents);

    fidNumber = env->GetFieldID(limitsClazz, "maxGeometryOutputVertices", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxGeometryOutputVertices);

    fidNumber = env->GetFieldID(limitsClazz, "maxGeometryTotalOutputComponents", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxGeometryTotalOutputComponents);

    fidNumber = env->GetFieldID(limitsClazz, "maxFragmentInputComponents", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxFragmentInputComponents);

    fidNumber = env->GetFieldID(limitsClazz, "maxFragmentOutputAttachments", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxFragmentOutputAttachments);

    fidNumber = env->GetFieldID(limitsClazz, "maxFragmentDualSrcAttachments", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxFragmentDualSrcAttachments);

    fidNumber = env->GetFieldID(limitsClazz, "maxFragmentCombinedOutputResources", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxFragmentCombinedOutputResources);

    fidNumber = env->GetFieldID(limitsClazz, "maxComputeSharedMemorySize", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxComputeSharedMemorySize);

    fidNumber = env->GetFieldID(limitsClazz, "maxComputeWorkGroupCount", "[J");
    jlongArray outJLongs = env->NewLongArray(3);
    jlong computeWorkGroupCount[] = {limits.maxComputeWorkGroupCount[0], limits.maxComputeWorkGroupCount[1], limits.maxComputeWorkGroupCount[2]};
    env->SetLongArrayRegion(outJLongs, 0, 3, computeWorkGroupCount);
    env->SetObjectField(obj, fidNumber, outJLongs);

    fidNumber = env->GetFieldID(limitsClazz, "maxComputeWorkGroupInvocations", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxComputeWorkGroupInvocations);

    fidNumber = env->GetFieldID(limitsClazz, "maxComputeWorkGroupSize", "[J");
    outJLongs = env->NewLongArray(3);
    jlong computeWorkGroupSize[] = {limits.maxComputeWorkGroupSize[0], limits.maxComputeWorkGroupSize[1], limits.maxComputeWorkGroupSize[2]};
    env->SetLongArrayRegion(outJLongs, 0, 3, computeWorkGroupSize);
    env->SetObjectField(obj, fidNumber, outJLongs);

    fidNumber = env->GetFieldID(limitsClazz, "subPixelPrecisionBits", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.subPixelPrecisionBits);

    fidNumber = env->GetFieldID(limitsClazz, "subTexelPrecisionBits", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.subTexelPrecisionBits);

    fidNumber = env->GetFieldID(limitsClazz, "mipmapPrecisionBits", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.mipmapPrecisionBits);

    fidNumber = env->GetFieldID(limitsClazz, "maxDrawIndexedIndexValue", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxDrawIndexedIndexValue);

    fidNumber = env->GetFieldID(limitsClazz, "maxDrawIndirectCount", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxDrawIndirectCount);

    fidNumber = env->GetFieldID(limitsClazz, "maxSamplerLodBias", "F");
    env->SetFloatField(obj, fidNumber, (jfloat)limits.maxSamplerLodBias);

    fidNumber = env->GetFieldID(limitsClazz, "maxSamplerAnisotropy", "F");
    env->SetFloatField(obj, fidNumber, (jfloat)limits.maxSamplerAnisotropy);

    fidNumber = env->GetFieldID(limitsClazz, "maxViewports", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxViewports);

    fidNumber = env->GetFieldID(limitsClazz, "maxViewportDimensions", "[J");
    outJLongs = env->NewLongArray(2);
    jlong viewportDimensions[] = {limits.maxViewportDimensions[0], limits.maxViewportDimensions[1]};
    env->SetLongArrayRegion(outJLongs, 0, 2, viewportDimensions);
    env->SetObjectField(obj, fidNumber, outJLongs);

    fidNumber = env->GetFieldID(limitsClazz, "viewportBoundsRange", "[F");
    jfloatArray outJFloats = env->NewFloatArray(2);
    jfloat boundsRange[] = {limits.viewportBoundsRange[0], limits.viewportBoundsRange[1]};
    env->SetFloatArrayRegion(outJFloats, 0, 2,boundsRange);
    env->SetObjectField(obj, fidNumber, outJFloats);

    fidNumber = env->GetFieldID(limitsClazz, "viewportSubPixelBits", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.viewportSubPixelBits);

    fidNumber = env->GetFieldID(limitsClazz, "minMemoryMapAlignment", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.minMemoryMapAlignment);

    fidNumber = env->GetFieldID(limitsClazz, "minTexelBufferOffsetAlignment", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.minTexelBufferOffsetAlignment);

    fidNumber = env->GetFieldID(limitsClazz, "minUniformBufferOffsetAlignment", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.minUniformBufferOffsetAlignment);

    fidNumber = env->GetFieldID(limitsClazz, "minStorageBufferOffsetAlignment", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.minStorageBufferOffsetAlignment);

    fidNumber = env->GetFieldID(limitsClazz, "minTexelOffset", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.minTexelOffset);

    fidNumber = env->GetFieldID(limitsClazz, "maxTexelOffset", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxTexelOffset);

    fidNumber = env->GetFieldID(limitsClazz, "minTexelGatherOffset", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.minTexelGatherOffset);

    fidNumber = env->GetFieldID(limitsClazz, "maxTexelGatherOffset", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxTexelGatherOffset);

    fidNumber = env->GetFieldID(limitsClazz, "minInterpolationOffset", "F");
    env->SetFloatField(obj, fidNumber, (jfloat)limits.minInterpolationOffset);

    fidNumber = env->GetFieldID(limitsClazz, "maxInterpolationOffset", "F");
    env->SetFloatField(obj, fidNumber, (jfloat)limits.maxInterpolationOffset);

    fidNumber = env->GetFieldID(limitsClazz, "subPixelInterpolationOffsetBits", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.subPixelInterpolationOffsetBits);

    fidNumber = env->GetFieldID(limitsClazz, "maxFramebufferWidth", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxFramebufferWidth);

    fidNumber = env->GetFieldID(limitsClazz, "maxFramebufferHeight", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxFramebufferHeight);

    fidNumber = env->GetFieldID(limitsClazz, "maxFramebufferLayers", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxFramebufferLayers);

    fidNumber = env->GetFieldID(limitsClazz, "framebufferColorSampleCounts", "Ljava/lang/String;");
    jstring outJString = env->NewStringUTF(getSampleCountDescription(limits.framebufferColorSampleCounts).c_str());
    env->SetObjectField(obj, fidNumber, outJString);

    fidNumber = env->GetFieldID(limitsClazz, "framebufferDepthSampleCounts", "Ljava/lang/String;");
    outJString = env->NewStringUTF(getSampleCountDescription(limits.framebufferDepthSampleCounts).c_str());
    env->SetObjectField(obj, fidNumber, outJString);

    fidNumber = env->GetFieldID(limitsClazz, "framebufferStencilSampleCounts", "Ljava/lang/String;");
    outJString = env->NewStringUTF(getSampleCountDescription(limits.framebufferStencilSampleCounts).c_str());
    env->SetObjectField(obj, fidNumber, outJString);

    fidNumber = env->GetFieldID(limitsClazz, "framebufferNoAttachmentsSampleCounts", "Ljava/lang/String;");
    outJString = env->NewStringUTF(getSampleCountDescription(limits.framebufferNoAttachmentsSampleCounts).c_str());
    env->SetObjectField(obj, fidNumber, outJString);

    fidNumber = env->GetFieldID(limitsClazz, "maxColorAttachments", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxColorAttachments);

    fidNumber = env->GetFieldID(limitsClazz, "sampledImageColorSampleCounts", "Ljava/lang/String;");
    outJString = env->NewStringUTF(getSampleCountDescription(limits.sampledImageColorSampleCounts).c_str());
    env->SetObjectField(obj, fidNumber, outJString);

    fidNumber = env->GetFieldID(limitsClazz, "sampledImageIntegerSampleCounts", "Ljava/lang/String;");
    outJString = env->NewStringUTF(getSampleCountDescription(limits.sampledImageIntegerSampleCounts).c_str());
    env->SetObjectField(obj, fidNumber, outJString);

    fidNumber = env->GetFieldID(limitsClazz, "sampledImageDepthSampleCounts", "Ljava/lang/String;");
    outJString = env->NewStringUTF(getSampleCountDescription(limits.sampledImageDepthSampleCounts).c_str());
    env->SetObjectField(obj, fidNumber, outJString);

    fidNumber = env->GetFieldID(limitsClazz, "sampledImageStencilSampleCounts", "Ljava/lang/String;");
    outJString = env->NewStringUTF(getSampleCountDescription(limits.sampledImageStencilSampleCounts).c_str());
    env->SetObjectField(obj, fidNumber, outJString);

    fidNumber = env->GetFieldID(limitsClazz, "storageImageSampleCounts", "Ljava/lang/String;");
    outJString = env->NewStringUTF(getSampleCountDescription(limits.storageImageSampleCounts).c_str());
    env->SetObjectField(obj, fidNumber, outJString);

    fidNumber = env->GetFieldID(limitsClazz, "maxSampleMaskWords", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxSampleMaskWords);

    fidNumber = env->GetFieldID(limitsClazz, "timestampComputeAndGraphics", "Z");
    env->SetBooleanField(obj, fidNumber, (jboolean)limits.timestampComputeAndGraphics);

    fidNumber = env->GetFieldID(limitsClazz, "timestampPeriod", "F");
    env->SetFloatField(obj, fidNumber, (jfloat)limits.timestampPeriod);

    fidNumber = env->GetFieldID(limitsClazz, "maxClipDistances", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxClipDistances);

    fidNumber = env->GetFieldID(limitsClazz, "maxCullDistances", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxCullDistances);

    fidNumber = env->GetFieldID(limitsClazz, "maxCombinedClipAndCullDistances", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxCombinedClipAndCullDistances);

    fidNumber = env->GetFieldID(limitsClazz, "discreteQueuePriorities", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.discreteQueuePriorities);

    fidNumber = env->GetFieldID(limitsClazz, "pointSizeRange", "[F");
    outJFloats = env->NewFloatArray(2);
    jfloat pointsSizeRange[] = {limits.pointSizeRange[0], limits.pointSizeRange[1]};
    env->SetFloatArrayRegion(outJFloats, 0, 2,pointsSizeRange);
    env->SetObjectField(obj, fidNumber, outJFloats);

    fidNumber = env->GetFieldID(limitsClazz, "lineWidthRange", "[F");
    outJFloats = env->NewFloatArray(2);
    jfloat lineWidthRange[] = {limits.lineWidthRange[0], limits.lineWidthRange[1]};
    env->SetFloatArrayRegion(outJFloats, 0, 2,lineWidthRange);
    env->SetObjectField(obj, fidNumber, outJFloats);

    fidNumber = env->GetFieldID(limitsClazz, "pointSizeGranularity", "F");
    env->SetFloatField(obj, fidNumber, (jfloat)limits.pointSizeGranularity);

    fidNumber = env->GetFieldID(limitsClazz, "lineWidthGranularity", "F");
    env->SetFloatField(obj, fidNumber, (jfloat)limits.lineWidthGranularity);

    fidNumber = env->GetFieldID(limitsClazz, "strictLines", "Z");
    env->SetBooleanField(obj, fidNumber, (jboolean)limits.strictLines);

    fidNumber = env->GetFieldID(limitsClazz, "standardSampleLocations", "Z");
    env->SetBooleanField(obj, fidNumber, (jboolean)limits.standardSampleLocations);

    fidNumber = env->GetFieldID(limitsClazz, "optimalBufferCopyOffsetAlignment", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.optimalBufferCopyOffsetAlignment);

    fidNumber = env->GetFieldID(limitsClazz, "optimalBufferCopyRowPitchAlignment", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.optimalBufferCopyRowPitchAlignment);

    fidNumber = env->GetFieldID(limitsClazz, "nonCoherentAtomSize", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.nonCoherentAtomSize);
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