#pragma once

#define VK_USE_PLATFORM_ANDROID_KHR
#include "vulkan/vulkan.h"

class LogicalDevice
{
public:
    LogicalDevice(VkPhysicalDevice physicalDevice);
    ~LogicalDevice();
private:
    VkDevice handle = VK_NULL_HANDLE;
};