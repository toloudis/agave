#pragma once

#include "vulkandevice.h"

#include <vulkan/vulkan.h>

#include <string>
#include <vector>

class vulkanapp
{
public:
  int initialize();
  void cleanup();

private:
  VkInstance m_instance = 0;
  VkDebugUtilsMessengerEXT m_debugMessenger;
  std::vector<PhysicalDevice> m_physicalDevices;
  VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

  int createInstance();
  bool checkValidationLayerSupport();
  std::vector<std::string> getRequiredExtensions();
  void setupDebugMessenger();
  void pickPhysicalDevice();
};
