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

    fidNumber = env->GetFieldID(limitsClazz, "framebufferColorSampleCounts", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.framebufferColorSampleCounts);

    fidNumber = env->GetFieldID(limitsClazz, "framebufferDepthSampleCounts", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.framebufferDepthSampleCounts);

    fidNumber = env->GetFieldID(limitsClazz, "framebufferStencilSampleCounts", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.framebufferStencilSampleCounts);

    fidNumber = env->GetFieldID(limitsClazz, "framebufferNoAttachmentsSampleCounts", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.framebufferNoAttachmentsSampleCounts);

    fidNumber = env->GetFieldID(limitsClazz, "maxColorAttachments", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.maxColorAttachments);

    fidNumber = env->GetFieldID(limitsClazz, "sampledImageColorSampleCounts", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.sampledImageColorSampleCounts);

    fidNumber = env->GetFieldID(limitsClazz, "sampledImageIntegerSampleCounts", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.sampledImageIntegerSampleCounts);

    fidNumber = env->GetFieldID(limitsClazz, "sampledImageDepthSampleCounts", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.sampledImageDepthSampleCounts);

    fidNumber = env->GetFieldID(limitsClazz, "sampledImageStencilSampleCounts", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.sampledImageStencilSampleCounts);

    fidNumber = env->GetFieldID(limitsClazz, "storageImageSampleCounts", "J");
    env->SetLongField(obj, fidNumber, (jlong)limits.storageImageSampleCounts);

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

    jfieldID fieldId = env->GetFieldID(propertiesClazz, "residencyStandard2DBlockShape", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)properties.residencyStandard2DBlockShape);

    fieldId = env->GetFieldID(propertiesClazz, "residencyStandard2DMultisampleBlockShape", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)properties.residencyStandard2DMultisampleBlockShape);

    fieldId = env->GetFieldID(propertiesClazz, "residencyStandard3DBlockShape", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)properties.residencyStandard3DBlockShape);

    fieldId = env->GetFieldID(propertiesClazz, "residencyAlignedMipSize", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)properties.residencyAlignedMipSize);

    fieldId = env->GetFieldID(propertiesClazz, "residencyNonResidentStrict", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)properties.residencyNonResidentStrict);
}

/**
 * Populates the fields of the <code>PhysicalDeviceFeatures</code> Java object.
 * @param env The JNI environment.
 * @param properties The <code>VkPhysicalDeviceFeatures</code> that contains the properties.
 * @param obj (OUT param) The <code>PhysicalDeviceFeatures</code> Java object to populate.
 */
void populatePhysicalDeviceFeaturesObject(JNIEnv* env, const VkPhysicalDeviceFeatures features, jobject obj)
{
    jclass featuresClazz = env->FindClass("com/example/vulkaninfoapp/PhysicalDeviceFeatures");

    jfieldID fieldId = env->GetFieldID(featuresClazz, "robustBufferAccess", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.robustBufferAccess);

    fieldId = env->GetFieldID(featuresClazz, "fullDrawIndexUint32", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.fullDrawIndexUint32);

    fieldId = env->GetFieldID(featuresClazz, "imageCubeArray", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.imageCubeArray);

    fieldId = env->GetFieldID(featuresClazz, "independentBlend", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.independentBlend);

    fieldId = env->GetFieldID(featuresClazz, "geometryShader", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.geometryShader);

    fieldId = env->GetFieldID(featuresClazz, "tessellationShader", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.tessellationShader);

    fieldId = env->GetFieldID(featuresClazz, "sampleRateShading", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.sampleRateShading);

    fieldId = env->GetFieldID(featuresClazz, "dualSrcBlend", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.dualSrcBlend);

    fieldId = env->GetFieldID(featuresClazz, "logicOp", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.logicOp);

    fieldId = env->GetFieldID(featuresClazz, "multiDrawIndirect", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.multiDrawIndirect);

    fieldId = env->GetFieldID(featuresClazz, "drawIndirectFirstInstance", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.drawIndirectFirstInstance);

    fieldId = env->GetFieldID(featuresClazz, "depthClamp", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.depthClamp);

    fieldId = env->GetFieldID(featuresClazz, "depthBiasClamp", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.depthBiasClamp);

    fieldId = env->GetFieldID(featuresClazz, "fillModeNonSolid", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.fillModeNonSolid);

    fieldId = env->GetFieldID(featuresClazz, "depthBounds", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.depthBounds);

    fieldId = env->GetFieldID(featuresClazz, "wideLines", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.wideLines);

    fieldId = env->GetFieldID(featuresClazz, "largePoints", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.largePoints);

    fieldId = env->GetFieldID(featuresClazz, "alphaToOne", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.alphaToOne);

    fieldId = env->GetFieldID(featuresClazz, "multiViewport", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.multiViewport);

    fieldId = env->GetFieldID(featuresClazz, "samplerAnisotropy", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.samplerAnisotropy);

    fieldId = env->GetFieldID(featuresClazz, "textureCompressionETC2", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.textureCompressionETC2);

    fieldId = env->GetFieldID(featuresClazz, "textureCompressionASTC_LDR", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.textureCompressionASTC_LDR);

    fieldId = env->GetFieldID(featuresClazz, "textureCompressionBC", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.textureCompressionBC);

    fieldId = env->GetFieldID(featuresClazz, "occlusionQueryPrecise", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.occlusionQueryPrecise);

    fieldId = env->GetFieldID(featuresClazz, "pipelineStatisticsQuery", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.pipelineStatisticsQuery);

    fieldId = env->GetFieldID(featuresClazz, "vertexPipelineStoresAndAtomics", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.robustBufferAccess);

    fieldId = env->GetFieldID(featuresClazz, "fragmentStoresAndAtomics", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.fragmentStoresAndAtomics);

    fieldId = env->GetFieldID(featuresClazz, "shaderTessellationAndGeometryPointSize", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderTessellationAndGeometryPointSize);

    fieldId = env->GetFieldID(featuresClazz, "shaderImageGatherExtended", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderImageGatherExtended);

    fieldId = env->GetFieldID(featuresClazz, "shaderStorageImageExtendedFormats", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderStorageImageExtendedFormats);

    fieldId = env->GetFieldID(featuresClazz, "shaderStorageImageMultisample", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderStorageImageMultisample);

    fieldId = env->GetFieldID(featuresClazz, "shaderStorageImageReadWithoutFormat", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderStorageImageReadWithoutFormat);

    fieldId = env->GetFieldID(featuresClazz, "shaderStorageImageWriteWithoutFormat", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderStorageImageWriteWithoutFormat);

    fieldId = env->GetFieldID(featuresClazz, "shaderUniformBufferArrayDynamicIndexing", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderUniformBufferArrayDynamicIndexing);

    fieldId = env->GetFieldID(featuresClazz, "shaderSampledImageArrayDynamicIndexing", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderSampledImageArrayDynamicIndexing);

    fieldId = env->GetFieldID(featuresClazz, "shaderStorageBufferArrayDynamicIndexing", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderStorageBufferArrayDynamicIndexing);

    fieldId = env->GetFieldID(featuresClazz, "shaderStorageImageArrayDynamicIndexing", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderStorageImageArrayDynamicIndexing);

    fieldId = env->GetFieldID(featuresClazz, "shaderClipDistance", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderClipDistance);

    fieldId = env->GetFieldID(featuresClazz, "shaderCullDistance", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderCullDistance);

    fieldId = env->GetFieldID(featuresClazz, "shaderFloat64", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderFloat64);

    fieldId = env->GetFieldID(featuresClazz, "shaderInt64", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderInt64);

    fieldId = env->GetFieldID(featuresClazz, "shaderInt16", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderInt16);

    fieldId = env->GetFieldID(featuresClazz, "shaderResourceResidency", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderResourceResidency);

    fieldId = env->GetFieldID(featuresClazz, "shaderResourceMinLod", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.shaderResourceMinLod);

    fieldId = env->GetFieldID(featuresClazz, "sparseBinding", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.sparseBinding);

    fieldId = env->GetFieldID(featuresClazz, "sparseResidencyBuffer", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.sparseResidencyBuffer);

    fieldId = env->GetFieldID(featuresClazz, "sparseResidencyImage2D", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.sparseResidencyImage2D);

    fieldId = env->GetFieldID(featuresClazz, "sparseResidencyImage3D", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.sparseResidencyImage3D);

    fieldId = env->GetFieldID(featuresClazz, "sparseResidency2Samples", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.sparseResidency2Samples);

    fieldId = env->GetFieldID(featuresClazz, "sparseResidency4Samples", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.sparseResidency4Samples);

    fieldId = env->GetFieldID(featuresClazz, "sparseResidency8Samples", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.sparseResidency8Samples);

    fieldId = env->GetFieldID(featuresClazz, "sparseResidency16Samples", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.sparseResidency16Samples);

    fieldId = env->GetFieldID(featuresClazz, "sparseResidencyAliased", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.sparseResidencyAliased);

    fieldId = env->GetFieldID(featuresClazz, "variableMultisampleRate", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.variableMultisampleRate);

    fieldId = env->GetFieldID(featuresClazz, "inheritedQueries", "Z");
    env->SetBooleanField(obj, fieldId, (jboolean)features.inheritedQueries);
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
    const std::string PhysicalDeviceFeaturesClassName = "com/example/vulkaninfoapp/PhysicalDeviceFeatures";

    const std::string InstanceInfoClassSignature = "Lcom/example/vulkaninfoapp/InstanceInfo;";
    const std::string PhysicalDevicePropertiesClassSignature = "Lcom/example/vulkaninfoapp/PhysicalDeviceProperties;";
    const std::string PhysicalDeviceLimitsClassSignature = "Lcom/example/vulkaninfoapp/PhysicalDeviceLimits;";
    const std::string PhysicalDeviceSparsePropertiesClassSignature = "Lcom/example/vulkaninfoapp/PhysicalDeviceSparseProperties;";
    const std::string PhysicalDeviceFeaturesClassSignature = "Lcom/example/vulkaninfoapp/PhysicalDeviceFeatures;";

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

    // Populate the physicalDeviceFeatures field of the Java VkInfo object.
    vkInfo.physicalDeviceFeatures = PhysicalDevice::getDeviceFeatures(vkInfo.selectedPhysicalDevice);
    jobject physicalDeviceFeaturesObject = getObject(env, PhysicalDeviceFeaturesClassName.c_str());
    populatePhysicalDeviceFeaturesObject(env, vkInfo.physicalDeviceFeatures, physicalDeviceFeaturesObject);
    fieldId = env->GetFieldID(env->FindClass(VkInfoClassName.c_str()), "physicalDeviceFeatures", PhysicalDeviceFeaturesClassSignature.c_str());
    env->SetObjectField(vkInfoObject, fieldId, physicalDeviceFeaturesObject);

    delete vkInfo.instance;
    return vkInfoObject;
}