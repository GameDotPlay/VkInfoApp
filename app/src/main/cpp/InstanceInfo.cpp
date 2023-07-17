#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>

struct InstanceInfo
{
    VkInstance instance = VK_NULL_HANDLE;
    std::string appName;
    std::string engineName;
    uint32_t deviceCount;
    std::vector<VkPhysicalDevice> devices = {};
};