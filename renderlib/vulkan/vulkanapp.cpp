#include "vulkanapp.h"

#include "Logging.h"

const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

int
vulkanapp::initialize()
{
  int ret = createInstance();
  return ret;
}

int
vulkanapp::createInstance()
{
  if (enableValidationLayers && !checkValidationLayerSupport()) {
    LOG_ERROR << "Vulkan validation layers requested, but not available!";
  }

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

  auto extensions = getRequiredExtensions();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  if (enableValidationLayers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
    LOG_ERROR << "Failed to create Vulkan instance!";
    return 0;
  }
  return 1;
}

void
vulkanapp::cleanup()
{
  vkDestroyInstance(m_instance, nullptr);
}

bool
vulkanapp::checkValidationLayerSupport()
{
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char* layerName : validationLayers) {
    bool layerFound = false;

    for (const auto& layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

std::vector<const char*>
vulkanapp::getRequiredExtensions()
{
  std::vector<const char*> names;

  VkResult err;
  uint32_t extensionCount = 0;

  // count the extensions

  err = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  if (err != VK_SUCCESS) {
    LOG_ERROR << "vkEnumerateInstanceExtensionProperties failed to get a count";
    return names;
  }
  std::vector<VkExtensionProperties> extensions(extensionCount);

  // get all the extensions
  err = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions.data());
  if (err != VK_SUCCESS) {
    LOG_ERROR << "vkEnumerateInstanceExtensionProperties failed to get VkExtensionProperties";
    return names;
  }

  // gather up all the names
  for (const auto& extension : extensions) {
    names.push_back(extension.extensionName);
    LOG_DEBUG << "Extension " << extension.extensionName;
  }

  if (enableValidationLayers) {
    names.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return names;
}