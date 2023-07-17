#include "PhysicalDevice.h"

namespace PhysicalDevice
{
    VkPhysicalDeviceProperties getDeviceProperties(VkPhysicalDevice device)
    {
        VkPhysicalDeviceProperties deviceProperties = {};

        if (device != VK_NULL_HANDLE)
        {
            vkGetPhysicalDeviceProperties(device, &deviceProperties);
        }

        return deviceProperties;
    }

    VkPhysicalDeviceFeatures getDeviceFeatures(VkPhysicalDevice device)
    {
        VkPhysicalDeviceFeatures deviceFeatures = {};

        if (device != VK_NULL_HANDLE)
        {
            vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
        }

        return deviceFeatures;
    }

    VkPhysicalDeviceMemoryProperties getMemoryProperties(VkPhysicalDevice device)
    {
        VkPhysicalDeviceMemoryProperties memoryProperties = {};

        if (device != VK_NULL_HANDLE)
        {
            vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);
        }

        return memoryProperties;
    }

    std::vector<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice device)
    {
        std::vector<VkQueueFamilyProperties> queueFamilyProperties = {};

        if (device != VK_NULL_HANDLE)
        {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            queueFamilyProperties.resize(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());
        }

        return queueFamilyProperties;
    }

    std::vector<std::string> getMemoryHeapFlags(VkMemoryHeapFlags memoryHeap)
    {
        std::vector<std::string> memoryHeapFlags = {};

        if (memoryHeap & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
        {
            memoryHeapFlags.push_back("Device local");
        }

        if (memoryHeap & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT)
        {
            memoryHeapFlags.push_back("Multi-instance");
        }

        return memoryHeapFlags;
    }
}