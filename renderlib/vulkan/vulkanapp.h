#pragma once

#include <vulkan/vulkan.h>

class vulkanapp
{
public:
  int initialize();
  void cleanup();

private:
  VkInstance m_instance;

  int createInstance();
};
