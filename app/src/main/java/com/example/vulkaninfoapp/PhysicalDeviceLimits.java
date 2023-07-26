package com.example.vulkaninfoapp;

import java.math.BigInteger;

public class PhysicalDeviceLimits {
    public long maxImageDimension1D;
    public long maxImageDimension2D;
    public long maxImageDimension3D;
    public long maxImageDimensionCube;
    public long maxImageArrayLayers;
    public long maxTexelBufferElements;
    public long maxUniformBufferRange;
    public long maxStorageBufferRange;
    public long maxPushConstantsSize;
    public long maxMemoryAllocationCount;
    public long maxSamplerAllocationCount;
    public long bufferImageGranularity;
    public long sparseAddressSpaceSize;
    public long maxBoundDescriptorSets;
    public long maxPerStageDescriptorSamplers;
    public long maxPerStageDescriptorStorageBuffers;
    public long maxPerStageDescriptorSampledImages;
    public long maxPerStageDescriptorStorageImages;
    public long maxPerStageDescriptorInputAttachments;
    public long maxPerStageResources;
    public long maxPerStageDescriptorUniformBuffers;
    public long maxDescriptorSetSamplers;
    public long maxDescriptorSetUniformBuffers;
    public long maxDescriptorSetUniformBuffersDynamic;
    public long maxDescriptorSetStorageBuffers;
    public long maxDescriptorSetStorageBuffersDynamic;
    public long maxDescriptorSetSampledImages;
    public long maxDescriptorSetStorageImages;
    public long maxDescriptorSetInputAttachments;
    public long maxVertexInputAttributes;
    public long maxVertexInputBindings;
    public long maxVertexInputAttributeOffset;
    public long maxVertexInputBindingStride;
    public long maxVertexOutputComponents;
    public long maxTessellationGenerationLevel;
    public long maxTessellationPatchSize;
    public long maxTessellationControlPerVertexInputComponents;
    public long maxTessellationControlPerVertexOutputComponents;
    public long maxTessellationControlPerPatchOutputComponents;
    public long maxTessellationControlTotalOutputComponents;
    public long maxTessellationEvaluationInputComponents;
    public long maxTessellationEvaluationOutputComponents;
    public long maxGeometryShaderInvocations;
    public long maxGeometryInputComponents;
    public long maxGeometryOutputComponents;
    public long maxGeometryOutputVertices;
    public long maxGeometryTotalOutputComponents;
    public long maxFragmentInputComponents;
    public long maxFragmentOutputAttachments;
    public long maxFragmentDualSrcAttachments;
    public long maxFragmentCombinedOutputResources;
    public long maxComputeSharedMemorySize;
    public long[] maxComputeWorkGroupCount = new long[3];
    public long maxComputeWorkGroupInvocations;
    public long[] maxComputeWorkGroupSize = new long[3];
    public long subPixelPrecisionBits;
    public long subTexelPrecisionBits;
    public long mipmapPrecisionBits;
    public long maxDrawIndexedIndexValue;
    public long maxDrawIndirectCount;
    public float maxSamplerLodBias;
    public float maxSamplerAnisotropy;
    public long maxViewports;
    public long[] maxViewportDimensions = new long[2];
    public float[] viewportBoundsRange = new float[2];
    public long viewportSubPixelBits;
    public long minMemoryMapAlignment;
    public long minTexelBufferOffsetAlignment;
    public long minUniformBufferOffsetAlignment;
    public long minStorageBufferOffsetAlignment;
    public long minTexelOffset;
    public long maxTexelOffset;
    public long minTexelGatherOffset;
    public long maxTexelGatherOffset;
    public float minInterpolationOffset;
    public float maxInterpolationOffset;
    public long subPixelInterpolationOffsetBits;
    public long maxFramebufferWidth;
    public long maxFramebufferHeight;
    public long maxFramebufferLayers;
    public String framebufferColorSampleCounts;
    public String framebufferDepthSampleCounts;
    public String framebufferStencilSampleCounts;
    public String framebufferNoAttachmentsSampleCounts;
    public long maxColorAttachments;
    public String sampledImageColorSampleCounts;
    public String sampledImageIntegerSampleCounts;
    public String sampledImageDepthSampleCounts;
    public String sampledImageStencilSampleCounts;
    public String storageImageSampleCounts;
    public long maxSampleMaskWords;
    public boolean timestampComputeAndGraphics;
    public float timestampPeriod;
    public long maxClipDistances;
    public long maxCullDistances;
    public long maxCombinedClipAndCullDistances;
    public long discreteQueuePriorities;
    public float[] pointSizeRange = new float[2];
    public float[] lineWidthRange = new float[2];
    public float pointSizeGranularity;
    public float lineWidthGranularity;
    public boolean strictLines;
    public boolean standardSampleLocations;
    public long optimalBufferCopyOffsetAlignment;
    public long optimalBufferCopyRowPitchAlignment;
    public long nonCoherentAtomSize;
}