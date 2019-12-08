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
  Device* m_device;

  int createInstance();
  bool checkValidationLayerSupport();
  std::vector<std::string> getRequiredExtensions();
  void setupDebugMessenger();
  void findAllPhysicalDevices();
  void createLogicalDevice();
};
