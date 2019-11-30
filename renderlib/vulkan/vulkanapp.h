#pragma once

#include <vulkan/vulkan.h>

#include <vector>

class vulkanapp
{
public:
  int initialize();
  void cleanup();

private:
  VkInstance m_instance = 0;
  VkDebugUtilsMessengerEXT m_debugMessenger;

  int createInstance();
  bool checkValidationLayerSupport();
  std::vector<const char*> getRequiredExtensions();
  void setupDebugMessenger();
};
