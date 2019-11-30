#include "vulkandevice.h"

PhysicalDevice::PhysicalDevice(VkPhysicalDevice d)
  : m_physicalDevice(d)
{
  vkGetPhysicalDeviceProperties(m_physicalDevice, &m_properties);
  vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_features);
}
