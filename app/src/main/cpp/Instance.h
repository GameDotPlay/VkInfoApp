#pragma once

#define VK_USE_PLATFORM_ANDROID_KHR
#include "vulkan/vulkan.h"

#include <string>
#include <vector>
#include <jni.h>

class Instance
{
public:
    Instance(const std::string& appName, const std::string& engineName, const std::vector<const char*>& extensions, const std::vector<const char*>& layers);
    ~Instance();
    VkInstance getInstance();
    std::string getAppName() const;
    std::string getEngineName() const;
    uint32_t getNumberPhysicalDevices();
    std::vector<VkPhysicalDevice> getPhysicalDevices();

private:
    VkInstance handle = VK_NULL_HANDLE;
    std::string appName;
    std::string engineName;

};