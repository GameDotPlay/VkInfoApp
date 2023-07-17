#pragma once

#define VK_USE_PLATFORM_ANDROID_KHR
#include "vulkan/vulkan.h"
#include <vector>
#include <string>

namespace PhysicalDevice
{
    //std::string deviceTypes[] = {"Other", "Integrated", "Discrete", "Virtual", "CPU"};
    VkPhysicalDeviceProperties getDeviceProperties(VkPhysicalDevice device);
    VkPhysicalDeviceFeatures getDeviceFeatures(VkPhysicalDevice device);
    VkPhysicalDeviceMemoryProperties getMemoryProperties(VkPhysicalDevice device);
    std::vector<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice device);
    std::vector<std::string> getMemoryHeapFlags(VkMemoryHeapFlags memoryHeap);
}