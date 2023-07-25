#pragma once

#define VK_USE_PLATFORM_ANDROID_KHR
#include "vulkan/vulkan.h"

#include <string>
#include <vector>
#include <jni.h>

/**
 * Wrapper class for a <code>VkInstance</code>.
 */
class Instance
{
public:
    Instance();
    Instance(const std::string& appName, const std::string& engineName, const std::vector<const char*>& extensions, const std::vector<const char*>& layers);
    ~Instance();
    VkInstance getHandle() const;
    std::string getAppName() const;
    std::string getEngineName() const;
    uint32_t getNumberPhysicalDevices() const;
    std::vector<VkPhysicalDevice> getPhysicalDevices() const;

private:
    VkInstance handle = VK_NULL_HANDLE;
    std::string appName;
    std::string engineName;
};