#pragma once

#include "InstanceInfo.hpp"
#include <vulkan/vulkan_core.h>
#include <string>

struct VkInfo
{
    InstanceInfo instanceInfo;
    VkPhysicalDevice selectedPhysicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties physicalDeviceProperties = {};
};
