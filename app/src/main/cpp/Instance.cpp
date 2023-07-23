#include "Instance.h"
#include <stdexcept>
#include <jni.h>

Instance::Instance(const std::string& appName, const std::string& engineName, const std::vector<const char*>& extensions, const std::vector<const char*>& layers)
{
    this->appName = appName;
    this->engineName = engineName;

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName.c_str();
    appInfo.pEngineName = engineName.c_str();
    appInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
    appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = (uint32_t)extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = (uint32_t)layers.size();
    createInfo.ppEnabledLayerNames = layers.data();

    if (vkCreateInstance(&createInfo, nullptr, &this->handle) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create VkInstance!");
    }
}

VkInstance Instance::getInstance() const
{
    return this->handle;
}

std::string Instance::getAppName() const
{
    return this->appName;
}

std::string Instance::getEngineName() const
{
    return this->engineName;
}

Instance::~Instance()
{
    if (this->handle != VK_NULL_HANDLE)
    {
        vkDestroyInstance(this->handle, nullptr);
    }
}

uint32_t Instance::getNumberPhysicalDevices() const
{
    if (this->handle == VK_NULL_HANDLE)
    {
        return 0;
    }

    uint32_t numDevices = 0;
    if (vkEnumeratePhysicalDevices(this->handle, &numDevices, nullptr) != VK_SUCCESS)
    {
        return 0;
    }

    return numDevices;
}

std::vector<VkPhysicalDevice> Instance::getPhysicalDevices() const
{
    if (this->handle == VK_NULL_HANDLE)
    {
        return {};
    }

    uint32_t numDevices = 0;
    if (vkEnumeratePhysicalDevices(this->handle, &numDevices, nullptr) != VK_SUCCESS)
    {
        return {};
    }

    std::vector<VkPhysicalDevice> devices(numDevices);
    if (vkEnumeratePhysicalDevices(this->handle, &numDevices, devices.data()) != VK_SUCCESS)
    {
        return {};
    }

    return devices;
}

Instance::Instance()
{

}
