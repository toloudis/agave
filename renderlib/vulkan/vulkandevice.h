#include <vulkan/vulkan.h>

#include <optional>

struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily;

  bool isComplete() { return graphicsFamily.has_value(); }
};

class PhysicalDevice
{
public:
  PhysicalDevice(VkPhysicalDevice d);

  const VkPhysicalDeviceProperties& properties() { return m_properties; }
  const VkPhysicalDeviceFeatures& features() { return m_features; }

  QueueFamilyIndices findQueueFamilies();

private:
  VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
  VkPhysicalDeviceProperties m_properties = {};
  VkPhysicalDeviceFeatures m_features = {};
};

class Device
{
public:
  void create();
  void destroy();

private:
  VkDevice m_device = VK_NULL_HANDLE;
  VkQueue m_graphicsQueue;
};