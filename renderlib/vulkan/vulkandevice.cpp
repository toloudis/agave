#include "vulkandevice.h"

#include "Logging.h"

#include <vector>

PhysicalDevice::PhysicalDevice(VkPhysicalDevice d)
  : m_physicalDevice(d)
{
  vkGetPhysicalDeviceProperties(m_physicalDevice, &m_properties);
  vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_features);
}

QueueFamilyIndices
PhysicalDevice::findQueueFamilies() const
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

Device*
Device::make(const PhysicalDevice& physicalDevice)
{
  QueueFamilyIndices indices = physicalDevice.findQueueFamilies();

  // create a small number of queues for each queue family and you don't really need more than one. That's because you
  // can create all of the command buffers on multiple threads and then submit them all at once on the main thread with
  // a single low-overhead call.
  VkDeviceQueueCreateInfo queueCreateInfo = {};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  // * operator is workaround for mac optional::value error
  queueCreateInfo.queueFamilyIndex = *(indices.graphicsFamily);
  queueCreateInfo.queueCount = 1;

  float queuePriority = 1.0f;
  queueCreateInfo.pQueuePriorities = &queuePriority;

  VkPhysicalDeviceFeatures deviceFeatures = {};

  VkDeviceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = &queueCreateInfo;
  createInfo.queueCreateInfoCount = 1;

  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount = 0;

  // in vulkan 1.1 device layers are deprecated but setting them anyway.
  //  if (enableValidationLayers) {
  //    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
  //    createInfo.ppEnabledLayerNames = validationLayers.data();
  //  } else {
  createInfo.enabledLayerCount = 0;
  //  }

  VkDevice device = VK_NULL_HANDLE;

  if (vkCreateDevice(physicalDevice.handle(), &createInfo, nullptr, &device) != VK_SUCCESS) {
    LOG_ERROR << "failed to create logical device!";
    return nullptr;
  }
  // * operator is workaround for mac optional::value error
  return new Device(device, physicalDevice, *(indices.graphicsFamily));
}

Device::Device(VkDevice device, const PhysicalDevice& physicalDevice, uint32_t graphicsQueueFamilyIndex)
  : m_device(device)
  , m_physicalDevice(physicalDevice)
{
  vkGetDeviceQueue(m_device, graphicsQueueFamilyIndex, 0, &m_graphicsQueue);
}

Device::~Device()
{
  vkDestroyDevice(m_device, nullptr);
}
