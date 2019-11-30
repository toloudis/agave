#include <vulkan/vulkan.h>

class PhysicalDevice
{
public:
  PhysicalDevice(VkPhysicalDevice d);

  const VkPhysicalDeviceProperties& properties() { return m_properties; }
  const VkPhysicalDeviceFeatures& features() { return m_features; }

private:
  VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
  VkPhysicalDeviceProperties m_properties = {};
  VkPhysicalDeviceFeatures m_features = {};
};
