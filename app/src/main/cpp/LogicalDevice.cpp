#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "QueueFamilyIndicies.hpp"
#include <vector>

LogicalDevice::LogicalDevice(VkPhysicalDevice physicalDevice)
{
    std::vector<VkQueueFamilyProperties> queues = {};
    QueueFamilyIndicies indices = {};

    if (physicalDevice != VK_NULL_HANDLE)
    {
        queues = PhysicalDevice::getQueueFamilyProperties(physicalDevice);

        for (uint32_t i = 0; i < queues.size(); i++)
        {
            if (queues[i].queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
            }
        }
    }

    VkDeviceQueueCreateInfo queueInfo = {};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = nullptr;
    queueInfo.flags = 0;
    queueInfo.queueFamilyIndex = indices.graphicsFamily.value();
    float queuePriorities = 1.0f;
    queueInfo.pQueuePriorities = &queuePriorities;
    queueInfo.queueCount = 0;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.queueCreateInfoCount = 0;
    createInfo.pQueueCreateInfos = nullptr;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
    createInfo.enabledExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = nullptr;
    createInfo.pEnabledFeatures = nullptr;

    vkCreateDevice(physicalDevice, &createInfo, nullptr, &this->handle);
}
