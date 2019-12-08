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

  QueueFamilyIndices findQueueFamilies() const;
  VkPhysicalDevice handle() const { return m_physicalDevice; }

private:
  VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
  VkPhysicalDeviceProperties m_properties = {};
  VkPhysicalDeviceFeatures m_features = {};
};

class Device
{
public:
  static Device* make(const PhysicalDevice& physicalDevice);

  Device();
  ~Device();

  VkDevice handle() const { return m_device; }

private:
  Device(VkDevice device, const PhysicalDevice& physicalDevice, uint32_t graphicsFamilyQueueIndex);

  VkDevice m_device = VK_NULL_HANDLE;
  VkQueue m_graphicsQueue;
  PhysicalDevice m_physicalDevice;
};