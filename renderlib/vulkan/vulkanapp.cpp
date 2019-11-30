#include "vulkanapp.h"

#include "Logging.h"

int
vulkanapp::initialize()
{
  int ret = createInstance();
  return ret;
}

int
vulkanapp::createInstance()
{
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "MY APP NAME HERE";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "MY ENGINE NAME HERE";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  VkResult err;
  uint32_t extensionCount = 0;

  // count the extensions

  err = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  if (err != VK_SUCCESS) {
    LOG_ERROR << "vkEnumerateInstanceExtensionProperties failed to get a count";
    return 0;
  }
  std::vector<VkExtensionProperties> extensions(extensionCount);

  // get all the extensions
  err = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions.data());
  if (err != VK_SUCCESS) {
    LOG_ERROR << "vkEnumerateInstanceExtensionProperties failed to get VkExtensionProperties";
    return 0;
  }

  // gather up all the names
  std::vector<const char*> names;
  for (const auto& extension : extensions) {
    names.push_back(extension.extensionName);
    LOG_DEBUG << "Extension " << extension.extensionName;
  }

  createInfo.enabledExtensionCount = extensionCount;
  createInfo.ppEnabledExtensionNames = names.data();

  createInfo.enabledLayerCount = 0;
  if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
    LOG_ERROR << "Failed to create Vulkan instance!";
    return 0;
  }
  return 1;
}

void
vulkanapp::cleanup()
{}