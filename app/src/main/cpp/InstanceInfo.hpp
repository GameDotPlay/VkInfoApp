#pragma once

#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>

struct InstanceInfo
{
    VkInstance instance = VK_NULL_HANDLE;
    std::string appName;
    std::string engineName;
    uint32_t deviceCount = 0;
    std::vector<VkPhysicalDevice> devices = {};
};