package com.example.vulkaninfoapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Pair;
import android.view.View;
import android.widget.ExpandableListAdapter;
import android.widget.ExpandableListView;
import android.widget.Toast;

import com.example.vulkaninfoapp.databinding.ActivityMainBinding;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("vulkaninfoapp");
    }

    private ActivityMainBinding binding;
    private List<String> groupList;
    private List<Pair<String, String>> childList;
    private Map<String, List<Pair<String, String>>> mobileCollection;
    private ExpandableListView expandableListView;
    private ExpandableListAdapter expandableListAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        createGroupList();
        createCollection();
        expandableListView = findViewById(R.id.vulkanInfo);
        expandableListAdapter = new MyExpandableListAdapter(this, groupList, mobileCollection);
        expandableListView.setAdapter(expandableListAdapter);
        expandableListView.setOnGroupExpandListener(new ExpandableListView.OnGroupExpandListener() {
            int lastExpandedPosition = -1;

            @Override
            public void onGroupExpand(int i) {
                if (lastExpandedPosition != -1 && i != lastExpandedPosition) {
                    expandableListView.collapseGroup(lastExpandedPosition);
                }
            }
        });
        expandableListView.setOnChildClickListener(new ExpandableListView.OnChildClickListener() {
            @Override
            public boolean onChildClick(ExpandableListView expandableListView, View view, int i, int i1, long l) {
                String selected = expandableListAdapter.getChild(i, i1).toString();
                Toast.makeText(getApplicationContext(), "Selected: " + selected, Toast.LENGTH_SHORT).show();
                return true;
            }
        });
    }

    private void createGroupList() {
        groupList = new ArrayList<>();
        groupList.add("Instance Info");
        groupList.add("Physical Device Properties");
        groupList.add("Physical Device Limits");
        groupList.add("Physical Device Sparse Properties");
        groupList.add("Physical Device Features");
        groupList.add("Physical Device Memory Properties");
        groupList.add("Physical Device Memory Heap Properties");
        groupList.add("Physical Device Queue Family Properties");
    }

    private void createCollection() {

        mobileCollection = new HashMap<String, List<Pair<String, String>>>();
        VkInfo vkInfo = getVkInfo("Vulkan Info App", "No engine");

        for (String group : groupList) {
            switch(group) {
                case "Instance Info":
                    populateInstanceInfo(vkInfo.instanceInfo);
                    break;

                case "Physical Device Properties":
                    populatePhysicalDeviceProperties(vkInfo.physicalDeviceProperties);
                    break;

                case "Physical Device Limits":
                    populatePhysicalDeviceLimits(vkInfo.physicalDeviceProperties.physicalDeviceLimits);
                    break;

                case "Physical Device Sparse Properties":
                    populatePhysicalDeviceSparseProperties(vkInfo.physicalDeviceProperties.physicalDeviceSparseProperties);
                    break;

                case "Physical Device Features":
                    populatePhysicalDeviceFeautures(vkInfo.physicalDeviceFeatures);
                    break;

                case "Physical Device Memory Properties":
                    break;

                case "Physical Device Heap Properties":
                    break;

                case "Physical Device Queue Family Properties":
                    break;

                default:
            }

            mobileCollection.put(group, childList);
        }
    }

    private void populateInstanceInfo(InstanceInfo instanceInfo) {
        childList = new ArrayList<Pair<String, String>>();
        if (instanceInfo != null) {
            childList.add(new Pair(InstanceInfo.InstancePropertyNames[InstancePropertyIndices.appName.ordinal()], instanceInfo.appName));
            childList.add(new Pair(InstanceInfo.InstancePropertyNames[InstancePropertyIndices.engineName.ordinal()], instanceInfo.engineName));
            childList.add(new Pair(InstanceInfo.InstancePropertyNames[InstancePropertyIndices.numDevices.ordinal()], String.valueOf(instanceInfo.numDevices)));
        }
    }

    private void populatePhysicalDeviceProperties(PhysicalDeviceProperties physicalDeviceProperties) {
        childList = new ArrayList<Pair<String, String>>();
        if (physicalDeviceProperties != null) {
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDevicePropertyNames[PhysicalDevicePropertyIndices.apiVersion.ordinal()], physicalDeviceProperties.apiVersion));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDevicePropertyNames[PhysicalDevicePropertyIndices.driverVersion.ordinal()], physicalDeviceProperties.driverVersion));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDevicePropertyNames[PhysicalDevicePropertyIndices.vendorId.ordinal()], physicalDeviceProperties.vendorId));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDevicePropertyNames[PhysicalDevicePropertyIndices.deviceId.ordinal()], physicalDeviceProperties.deviceId));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDevicePropertyNames[PhysicalDevicePropertyIndices.deviceType.ordinal()], PhysicalDeviceProperties.PhysicalDeviceTypeNames[physicalDeviceProperties.physicalDeviceType]));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDevicePropertyNames[PhysicalDevicePropertyIndices.deviceName.ordinal()], physicalDeviceProperties.deviceName));
        }
    }

    private void populatePhysicalDeviceLimits(PhysicalDeviceLimits limits) {
        childList = new ArrayList<Pair<String, String>>();
        if (limits != null) {
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxImageDimension1D.ordinal()], String.valueOf(limits.maxImageDimension1D)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxImageDimension2D.ordinal()], String.valueOf(limits.maxImageDimension2D)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxImageDimension3D.ordinal()], String.valueOf(limits.maxImageDimension3D)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxImageDimensionCube.ordinal()], String.valueOf(limits.maxImageDimensionCube)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxImageArrayLayers.ordinal()], String.valueOf(limits.maxImageArrayLayers)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxTexelBufferElements.ordinal()], String.valueOf(limits.maxTexelBufferElements)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxUniformBufferRange.ordinal()], String.valueOf(limits.maxUniformBufferRange)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxStorageBufferRange.ordinal()], String.valueOf(limits.maxStorageBufferRange)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxPushConstantsSize.ordinal()], String.valueOf(limits.maxPushConstantsSize)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxMemoryAllocationCount.ordinal()], String.valueOf(limits.maxMemoryAllocationCount)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxSamplerAllocationCount.ordinal()], String.valueOf(limits.maxSamplerAllocationCount)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.BufferImageGranularity.ordinal()], String.valueOf(limits.bufferImageGranularity)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.SparseAddressSpaceSize.ordinal()], String.valueOf(limits.sparseAddressSpaceSize)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxBoundDescriptorSets.ordinal()], String.valueOf(limits.maxBoundDescriptorSets)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxPerStageDescriptorSamplers.ordinal()], String.valueOf(limits.maxPerStageDescriptorSamplers)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxPerStageDescriptorUniformBuffers.ordinal()], String.valueOf(limits.maxPerStageDescriptorUniformBuffers)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxPerStageDescriptorStorageBuffers.ordinal()], String.valueOf(limits.maxPerStageDescriptorStorageBuffers)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxPerStageDescriptorSampledImages.ordinal()], String.valueOf(limits.maxPerStageDescriptorSampledImages)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxPerStageDescriptorStorageImages.ordinal()], String.valueOf(limits.maxPerStageDescriptorStorageImages)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxPerStageDescriptorInputAttachments.ordinal()], String.valueOf(limits.maxPerStageDescriptorInputAttachments)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxPerStageResources.ordinal()], String.valueOf(limits.maxPerStageResources)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxDescriptorSetSamplers.ordinal()], String.valueOf(limits.maxDescriptorSetSamplers)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxDescriptorSetUniformBuffers.ordinal()], String.valueOf(limits.maxDescriptorSetUniformBuffers)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxDescriptorSetUniformBuffersDynamic.ordinal()], String.valueOf(limits.maxDescriptorSetUniformBuffersDynamic)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxDescriptorSetStorageBuffers.ordinal()], String.valueOf(limits.maxDescriptorSetStorageBuffers)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxDescriptorSetStorageBuffersDynamic.ordinal()], String.valueOf(limits.maxDescriptorSetStorageBuffersDynamic)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxDescriptorSetSampledImages.ordinal()], String.valueOf(limits.maxDescriptorSetSampledImages)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxDescriptorSetStorageImages.ordinal()], String.valueOf(limits.maxDescriptorSetStorageImages)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxDescriptorSetInputAttachments.ordinal()], String.valueOf(limits.maxDescriptorSetInputAttachments)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxVertexInputAttributes.ordinal()], String.valueOf(limits.maxVertexInputAttributes)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxVertexInputBindings.ordinal()], String.valueOf(limits.maxVertexInputBindings)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxVertexInputAttributeOffset.ordinal()], String.valueOf(limits.maxVertexInputAttributeOffset)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxVertexInputBindingStride.ordinal()], String.valueOf(limits.maxVertexInputBindingStride)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxVertexOutputComponents.ordinal()], String.valueOf(limits.maxVertexOutputComponents)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxTessellationGenerationLevel.ordinal()], String.valueOf(limits.maxTessellationGenerationLevel)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxTessellationPatchSize.ordinal()], String.valueOf(limits.maxTessellationPatchSize)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxTessellationControlPerVertexInputComponents.ordinal()], String.valueOf(limits.maxTessellationControlPerVertexInputComponents)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxTessellationControlPerVertexOutputComponents.ordinal()], String.valueOf(limits.maxTessellationControlPerVertexOutputComponents)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxTessellationControlPerPatchOutputComponents.ordinal()], String.valueOf(limits.maxTessellationControlPerPatchOutputComponents)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxTessellationControlTotalOutputComponents.ordinal()], String.valueOf(limits.maxTessellationControlTotalOutputComponents)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxTessellationEvaluationInputComponents.ordinal()], String.valueOf(limits.maxTessellationEvaluationInputComponents)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxTessellationEvaluationOutputComponents.ordinal()], String.valueOf(limits.maxTessellationEvaluationOutputComponents)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxGeometryShaderInvocations.ordinal()], String.valueOf(limits.maxGeometryShaderInvocations)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxGeometryInputComponents.ordinal()], String.valueOf(limits.maxGeometryInputComponents)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxGeometryOutputComponents.ordinal()], String.valueOf(limits.maxGeometryOutputComponents)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxGeometryOutputVertices.ordinal()], String.valueOf(limits.maxGeometryOutputComponents)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxGeometryTotalOutputComponents.ordinal()], String.valueOf(limits.maxGeometryTotalOutputComponents)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxFragmentInputComponents.ordinal()], String.valueOf(limits.maxFragmentInputComponents)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxFragmentOutputAttachments.ordinal()], String.valueOf(limits.maxFragmentOutputAttachments)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxFragmentDualSrcAttachments.ordinal()], String.valueOf(limits.maxFragmentDualSrcAttachments)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxFragmentCombinedOutputResources.ordinal()], String.valueOf(limits.maxFragmentCombinedOutputResources)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxComputeSharedMemorySize.ordinal()], String.valueOf(limits.maxComputeSharedMemorySize)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxComputeWorkGroupCount.ordinal()],
                    "{" + String.valueOf(limits.maxComputeWorkGroupCount[0]) + ", " + String.valueOf(limits.maxComputeWorkGroupCount[1]) + ", " + String.valueOf(limits.maxComputeWorkGroupCount[2]) + "}"));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxComputeWorkGroupInvocations.ordinal()], String.valueOf(limits.maxComputeWorkGroupInvocations)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxComputeWorkGroupSize.ordinal()],
                    "{" + String.valueOf(limits.maxComputeWorkGroupSize[0]) + ", " + String.valueOf(limits.maxComputeWorkGroupSize[1]) + ", " + String.valueOf(limits.maxComputeWorkGroupSize[2]) + "}"));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.SubPixelPrecisionBits.ordinal()], String.valueOf(limits.subPixelPrecisionBits)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.SubTexelPrecisionBits.ordinal()], String.valueOf(limits.subTexelPrecisionBits)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MipmapPrecisionBits.ordinal()], String.valueOf(limits.mipmapPrecisionBits)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxDrawIndexedIndexValue.ordinal()], String.valueOf(limits.maxDrawIndexedIndexValue)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxDrawIndirectCount.ordinal()], String.valueOf(limits.maxDrawIndirectCount)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxSamplerLodBias.ordinal()], String.valueOf(limits.maxSamplerLodBias)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxSamplerAnisotropy.ordinal()], String.valueOf(limits.maxSamplerAnisotropy)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxViewports.ordinal()], String.valueOf(limits.maxViewports)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxViewportDimensions.ordinal()],
                    "{" + String.valueOf(limits.maxViewportDimensions[0]) + ", " + String.valueOf(limits.maxViewportDimensions[1]) + "}"));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.ViewportBoundsRange.ordinal()],
                    "{" + String.valueOf(limits.viewportBoundsRange[0]) + ", " + String.valueOf(limits.viewportBoundsRange[1]) + "}"));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.ViewportSubPixelBits.ordinal()], String.valueOf(limits.viewportSubPixelBits)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MinMemoryMapAlignment.ordinal()], String.valueOf(limits.minMemoryMapAlignment)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MinTexelBufferOffsetAlignment.ordinal()], String.valueOf(limits.minTexelBufferOffsetAlignment)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MinUniformBufferOffsetAlignment.ordinal()], String.valueOf(limits.minUniformBufferOffsetAlignment)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MinStorageBufferOffsetAlignment.ordinal()], String.valueOf(limits.minStorageBufferOffsetAlignment)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MinTexelOffset.ordinal()], String.valueOf(limits.minTexelOffset)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxTexelOffset.ordinal()], String.valueOf(limits.maxTexelOffset)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MinTexelGatherOffset.ordinal()], String.valueOf(limits.minTexelGatherOffset)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxTexelGatherOffset.ordinal()], String.valueOf(limits.maxTexelGatherOffset)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MinInterpolationOffset.ordinal()], String.valueOf(limits.minInterpolationOffset)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxInterpolationOffset.ordinal()], String.valueOf(limits.maxInterpolationOffset)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.SubPixelInterpolationOffsetBits.ordinal()], String.valueOf(limits.subPixelInterpolationOffsetBits)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxFramebufferWidth.ordinal()], String.valueOf(limits.maxFramebufferWidth)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxFramebufferHeight.ordinal()], String.valueOf(limits.maxFramebufferHeight)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxFramebufferLayers.ordinal()], String.valueOf(limits.maxFramebufferLayers)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.FramebufferColorSampleCounts.ordinal()], String.valueOf(limits.framebufferColorSampleCounts)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.FramebufferDepthSampleCounts.ordinal()], String.valueOf(limits.framebufferDepthSampleCounts)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.FramebufferStencilSampleCounts.ordinal()], String.valueOf(limits.framebufferStencilSampleCounts)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.FramebufferNoAttachmentsSampleCounts.ordinal()], String.valueOf(limits.framebufferNoAttachmentsSampleCounts)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxColorAttachments.ordinal()], String.valueOf(limits.maxColorAttachments)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.SampledImageColorSampleCounts.ordinal()], String.valueOf(limits.sampledImageColorSampleCounts)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.SampledImageIntegerSampleCounts.ordinal()], String.valueOf(limits.sampledImageIntegerSampleCounts)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.SampledImageDepthSampleCounts.ordinal()], String.valueOf(limits.sampledImageDepthSampleCounts)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.SampledImageStencilSampleCounts.ordinal()], String.valueOf(limits.sampledImageStencilSampleCounts)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.StorageImageSampleCounts.ordinal()], String.valueOf(limits.storageImageSampleCounts)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxSampleMaskWords.ordinal()], String.valueOf(limits.maxSampleMaskWords)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.TimestampComputeAndGraphics.ordinal()], String.valueOf(limits.timestampComputeAndGraphics)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.TimestampPeriod.ordinal()], String.valueOf(limits.timestampPeriod)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxClipDistances.ordinal()], String.valueOf(limits.maxClipDistances)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxCullDistances.ordinal()], String.valueOf(limits.maxCullDistances)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.MaxCombinedClipAndCullDistances.ordinal()], String.valueOf(limits.maxCombinedClipAndCullDistances)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.DiscreteQueuePriorities.ordinal()], String.valueOf(limits.discreteQueuePriorities)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.PointSizeRange.ordinal()],
                    "{" + String.valueOf(limits.pointSizeRange[0]) + ", " + String.valueOf(limits.pointSizeRange[1]) + "}"));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.LineWidthRange.ordinal()],
                    "{" + String.valueOf(limits.lineWidthRange[0]) + ", " + String.valueOf(limits.lineWidthRange[1]) + "}"));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.PointSizeGranularity.ordinal()], String.valueOf(limits.pointSizeGranularity)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.LineWidthGranularity.ordinal()], String.valueOf(limits.lineWidthGranularity)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.StrictLines.ordinal()], String.valueOf(limits.strictLines)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.StandardSampleLocations.ordinal()], String.valueOf(limits.standardSampleLocations)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.OptimalBufferCopyOffsetAlignment.ordinal()], String.valueOf(limits.optimalBufferCopyOffsetAlignment)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.OptimalBufferCopyRowPitchAlignment.ordinal()], String.valueOf(limits.optimalBufferCopyRowPitchAlignment)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceLimitsNames[PhysicalDeviceLimitsIndices.NonCoherentAtomSize.ordinal()], String.valueOf(limits.nonCoherentAtomSize)));
        }
    }

    private void populatePhysicalDeviceSparseProperties(PhysicalDeviceSparseProperties properties) {
        childList = new ArrayList<Pair<String, String>>();
        if (properties != null) {
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceSparsePropertyNames[PhysicalDeviceSparsePropertyIndices.ResidencyStandard2DBlockShape.ordinal()], String.valueOf(properties.residencyStandard2DBlockShape)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceSparsePropertyNames[PhysicalDeviceSparsePropertyIndices.ResidencyStandard2DMultisampleBlockShape.ordinal()], String.valueOf(properties.residencyStandard2DMultisampleBlockShape)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceSparsePropertyNames[PhysicalDeviceSparsePropertyIndices.ResidencyStandard3DBlockShape.ordinal()], String.valueOf(properties.residencyStandard3DBlockShape)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceSparsePropertyNames[PhysicalDeviceSparsePropertyIndices.ResidencyAlignedMipSize.ordinal()], String.valueOf(properties.residencyAlignedMipSize)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDeviceSparsePropertyNames[PhysicalDeviceSparsePropertyIndices.ResidencyNonResidentScrict.ordinal()], String.valueOf(properties.residencyNonResidentStrict)));
        }
    }

    private void populatePhysicalDeviceFeautures(PhysicalDeviceFeatures features) {
        childList = new ArrayList<Pair<String, String>>();
        if (features != null) {
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.RobustBufferAccess.ordinal()], String.valueOf(features.robustBufferAccess)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.FullDrawIndexUint32.ordinal()], String.valueOf(features.fullDrawIndexUint32)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ImageCubeArray.ordinal()], String.valueOf(features.imageCubeArray)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.IndependentBlend.ordinal()], String.valueOf(features.independentBlend)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.GeometryShader.ordinal()], String.valueOf(features.geometryShader)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.TessellationShader.ordinal()], String.valueOf(features.tessellationShader)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.SampleRateShading.ordinal()], String.valueOf(features.sampleRateShading)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.DualSrcBlend.ordinal()], String.valueOf(features.dualSrcBlend)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.LogicOp.ordinal()], String.valueOf(features.logicOp)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.MultiDrawIndirect.ordinal()], String.valueOf(features.multiDrawIndirect)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.DrawIndirectFirstInstance.ordinal()], String.valueOf(features.drawIndirectFirstInstance)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.DepthClamp.ordinal()], String.valueOf(features.depthClamp)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.DepthBiasClamp.ordinal()], String.valueOf(features.depthBiasClamp)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.FillModeNonSolid.ordinal()], String.valueOf(features.fillModeNonSolid)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.DepthBounds.ordinal()], String.valueOf(features.depthBounds)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.WideLines.ordinal()], String.valueOf(features.wideLines)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.LargePoints.ordinal()], String.valueOf(features.largePoints)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.AlphaToOne.ordinal()], String.valueOf(features.alphaToOne)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.MultiViewport.ordinal()], String.valueOf(features.multiViewport)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.SamplerAnisotropy.ordinal()], String.valueOf(features.samplerAnisotropy)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.TextureCompressionETC2.ordinal()], String.valueOf(features.textureCompressionETC2)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.TextureCompressionASTC_LDR.ordinal()], String.valueOf(features.textureCompressionASTC_LDR)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.TextureCompressionBC.ordinal()], String.valueOf(features.textureCompressionBC)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.OcclusionQueryPrecise.ordinal()], String.valueOf(features.occlusionQueryPrecise)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.PipelineStatisticsQuery.ordinal()], String.valueOf(features.pipelineStatisticsQuery)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.VertexPipelineStoresAndAtomics.ordinal()], String.valueOf(features.vertexPipelineStoresAndAtomics)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.FragmentStoresAndAtomics.ordinal()], String.valueOf(features.fragmentStoresAndAtomics)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderTessellationAndGeometryPointSize.ordinal()], String.valueOf(features.shaderTessellationAndGeometryPointSize)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderImageGatherExtended.ordinal()], String.valueOf(features.shaderImageGatherExtended)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderStorageImageExtendedFormats.ordinal()], String.valueOf(features.shaderStorageImageExtendedFormats)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderStorageImageMultisample.ordinal()], String.valueOf(features.shaderStorageImageMultisample)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderStorageImageReadWithoutFormat.ordinal()], String.valueOf(features.shaderStorageImageReadWithoutFormat)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderStorageImageWriteWithoutFormat.ordinal()], String.valueOf(features.shaderStorageImageWriteWithoutFormat)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderUniformBufferArrayDynamicIndexing.ordinal()], String.valueOf(features.shaderUniformBufferArrayDynamicIndexing)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderSampledImageArrayDynamicIndexing.ordinal()], String.valueOf(features.shaderSampledImageArrayDynamicIndexing)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderStorageBufferArrayDynamicIndexing.ordinal()], String.valueOf(features.shaderStorageBufferArrayDynamicIndexing)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderStorageImageArrayDynamicIndexing.ordinal()], String.valueOf(features.shaderStorageImageArrayDynamicIndexing)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderClipDistance.ordinal()], String.valueOf(features.shaderClipDistance)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderCullDistance.ordinal()], String.valueOf(features.shaderCullDistance)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderFloat64.ordinal()], String.valueOf(features.shaderFloat64)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderInt64.ordinal()], String.valueOf(features.shaderInt64)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderInt16.ordinal()], String.valueOf(features.shaderInt16)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderResourceResidency.ordinal()], String.valueOf(features.shaderResourceResidency)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.ShaderResourceMinLod.ordinal()], String.valueOf(features.shaderResourceMinLod)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.SparseBinding.ordinal()], String.valueOf(features.sparseBinding)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.SparseResidencyBuffer.ordinal()], String.valueOf(features.sparseResidencyBuffer)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.SparseResidencyImage2D.ordinal()], String.valueOf(features.sparseResidencyImage2D)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.SparseResidencyImage3D.ordinal()], String.valueOf(features.sparseResidencyImage3D)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.SparseResidency2Samples.ordinal()], String.valueOf(features.sparseResidency2Samples)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.SparseResidency4Samples.ordinal()], String.valueOf(features.sparseResidency4Samples)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.SparseResidency8Samples.ordinal()], String.valueOf(features.sparseResidency8Samples)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.SparseResidency16Samples.ordinal()], String.valueOf(features.sparseResidency16Samples)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.SparseResidencyAliased.ordinal()], String.valueOf(features.sparseResidencyAliased)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.VariableMultisampleRate.ordinal()], String.valueOf(features.variableMultisampleRate)));
            childList.add(new Pair(PhysicalDeviceFeatures.PhysicalDeviceFeatureNames[PhysicalDeviceFeaturesIndices.InheritedQueries.ordinal()], String.valueOf(features.inheritedQueries)));
        }
    }
    
    private native static VkInfo getVkInfo(String appName, String engineName);
}