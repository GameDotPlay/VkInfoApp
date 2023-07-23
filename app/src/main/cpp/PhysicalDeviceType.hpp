#include <vulkan/vulkan_core.h>
#include <string>

struct PhysicalDeviceType
{
    static inline std::string PhysicalDeviceTypeNames[5] = {"Other", "Integrated", "Discrete", "Virtual", "CPU"};
};
