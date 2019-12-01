#include "vulkandevice.h"

#include <vector>

PhysicalDevice::PhysicalDevice(VkPhysicalDevice d)
  : m_physicalDevice(d)
{
  vkGetPhysicalDeviceProperties(m_physicalDevice, &m_properties);
  vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_features);
}

QueueFamilyIndices
PhysicalDevice::findQueueFamilies()
{
  QueueFamilyIndices indices;
  // Logic to find queue family indices to populate struct with
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }

    // exit on the first "complete" queue family
    if (indices.isComplete()) {
      break;
    }
    i++;
  }

  return indices;
}