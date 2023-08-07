#include "Instance.h"
#include <stdexcept>
#include <jni.h>

/**
 * The default class constructor.
 * Does not initialize a <code>VkInstance</code>.
 */
Instance::Instance()
{
    this->appName.clear();
    this->engineName.clear();
    this->handle = VK_NULL_HANDLE;
}

/**
 * Copy constructor.
 * @param other The other <code>Instance</code>.
 */
Instance::Instance(const Instance& other)
{
    this->appName = other.appName;
    this->engineName = other.engineName;

    if (other.handle != VK_NULL_HANDLE)
    {
        this->handle = other.handle;
    }
    else
    {
        this->handle = VK_NULL_HANDLE;
    }
}

/**
 * Constructor for <code>Instance</code> class.
 * @param appName The application name to give to the <code>VkInstance</code>.
 * @param engineName The engine name to give to the <code>VkInstance</code>.
 * @param extensions The list of extension names to initialize the <code>VkInstance</code> with.
 * @param layers The list of layer names to initialize the <code>VkInstance</code> with.
 */
Instance::Instance(const std::string& appName, const std::string& engineName, const std::vector<const char*>& extensions, const std::vector<const char*>& layers)
{
    this->appName = appName;
    this->engineName = engineName;

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName.c_str();
    appInfo.pEngineName = engineName.c_str();
    appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
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
        this->handle = VK_NULL_HANDLE;
        this->appName.clear();
        this->engineName.clear();
    }
}

/**
 * Gets the <code>VkInstance</code> handle.
 * @return the <code>VkInstance</code> handle.
 */
VkInstance Instance::getHandle() const
{
    return this->handle;
}

/**
 * Gets the application name.
 * @return the application name.
 */
std::string Instance::getAppName() const
{
    return this->appName;
}

/**
 * Gets the engine name.
 * @return the engine name.
 */
std::string Instance::getEngineName() const
{
    return this->engineName;
}

/**
 * Class destructor.
 * Calls <code>vkDestroyInstance</code> on the <code>VkInstance</code>.
 */
Instance::~Instance()
{
    if (this->handle != VK_NULL_HANDLE)
    {
        vkDestroyInstance(this->handle, nullptr);
        this->handle = VK_NULL_HANDLE;
    }
}

/**
 * Gets the number of physical devices accessible by this Vulkan instance.
 * @return the number of physical devices.
 */
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

/**
 * Gets the list of physical devices accessible by this Vulkan instance.
 * @return the list of physical devices.
 */
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

/**
 * Get the number of available extensions.
 */
uint32_t Instance::getNumAvailableExtensions() const
{
    uint32_t numExtensions = 0;
    if (vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, nullptr) != VK_SUCCESS)
    {
        return 0;
    }

    return numExtensions;
}

/**
 * Get a list of all available extensions.
 */
std::vector<VkExtensionProperties> Instance::getAllAvailableExtensions() const
{
    uint32_t numExtensions = 0;
    std::vector<VkExtensionProperties> extensionProperties = {};

    vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, nullptr);

    extensionProperties.resize(numExtensions);
    vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, extensionProperties.data());

    return extensionProperties;
}

/**
 * Get the properties of the given extension.
 * @param extensionName The name of the extension to get properties of.
 */
VkExtensionProperties Instance::getExtensionProperties(const char *extensionName) const
{
    uint32_t numExtensions = 1;
    VkExtensionProperties extensionProperties = {};
    vkEnumerateInstanceExtensionProperties(extensionName, &numExtensions, &extensionProperties);

    return extensionProperties;
}

/**
 * Get the number of available layers.
 */
uint32_t Instance::getNumAvailableLayers() const
{
    uint32_t numLayers = 0;
    if (vkEnumerateInstanceLayerProperties(&numLayers, nullptr) != VK_SUCCESS)
    {
        return 0;
    }

    return numLayers;
}

/**
 * Get a list of all available layers.
 */
std::vector<VkLayerProperties> Instance::getAllAvailableLayers() const
{
    uint32_t numLayers = 0;
    std::vector<VkLayerProperties> layerProperties = {};

    vkEnumerateInstanceLayerProperties(&numLayers, nullptr);

    layerProperties.resize(numLayers);
    vkEnumerateInstanceLayerProperties(&numLayers, layerProperties.data());

    return layerProperties;
}
