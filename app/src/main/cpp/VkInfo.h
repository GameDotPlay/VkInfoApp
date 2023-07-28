#pragma once

#include "InstanceInfo.hpp"
#include <vulkan/vulkan_core.h>

struct VkInfo
{
    Instance* instance = nullptr;
    VkPhysicalDevice selectedPhysicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties physicalDeviceProperties = {};
    VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties = {};
};