#pragma once

#include "InstanceInfo.hpp"
#include <vulkan/vulkan_core.h>
#include <string>

struct VkInfo
{
    Instance instance = {};
    VkPhysicalDevice selectedPhysicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties physicalDeviceProperties = {};
};
