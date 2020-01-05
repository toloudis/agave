#include "vulkanapp.h"

#include "Logging.h"

const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
              void* pUserData)
{

  LOG_DEBUG << "validation layer: " << pCallbackData->pMessage;
  return VK_FALSE;
}

VkResult
CreateDebugUtilsMessengerEXT(VkInstance instance,
                             const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator,
                             VkDebugUtilsMessengerEXT* pDebugMessenger)
{
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void
DestroyDebugUtilsMessengerEXT(VkInstance instance,
                              VkDebugUtilsMessengerEXT debugMessenger,
                              const VkAllocationCallbacks* pAllocator)
{
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

void
populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
}

bool
isDeviceSuitable(PhysicalDevice device)
{
  QueueFamilyIndices indices = device.findQueueFamilies();

  return indices.isComplete();
}

int
vulkanapp::initialize()
{
  int ret = createInstance();
  setupDebugMessenger();
  findAllPhysicalDevices();
  createLogicalDevice();
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
  std::vector<const char*> cstrings{};
  for (const auto& string : extensions) {
    cstrings.push_back(string.c_str());
  }

  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = cstrings.data();

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
  if (enableValidationLayers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;

    createInfo.pNext = nullptr;
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
  delete m_device;

  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
  }

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

std::vector<std::string>
vulkanapp::getRequiredExtensions()
{
  std::vector<std::string> names;

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
    // need to copy string here or else ptr will go out of scope
    names.push_back(extension.extensionName);
    LOG_DEBUG << "Extension " << extension.extensionName;
  }

  if (enableValidationLayers) {
    names.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return names;
}

void
vulkanapp::setupDebugMessenger()
{
  if (!enableValidationLayers)
    return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  populateDebugMessengerCreateInfo(createInfo);
  if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
    LOG_ERROR << "Vulkan failed to set up debug messenger!";
  }
}

void
vulkanapp::findAllPhysicalDevices()
{
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    LOG_ERROR << "Failed to find GPUs with Vulkan support!";
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
  for (const auto& device : devices) {
    PhysicalDevice pd(device);
    m_physicalDevices.push_back(pd);
  }
}

void
vulkanapp::createLogicalDevice()
{
  std::optional<PhysicalDevice> physicalDevice;
  for (const PhysicalDevice& device : m_physicalDevices) {
    // take the first suitable device in the enumeration
    if (isDeviceSuitable(device.handle())) {
      physicalDevice = device;
      break;
    }
  }

  if (!physicalDevice) {
    LOG_ERROR << "Failed to find a suitable GPU!";
    return;
  }

  // std::error_code ec;
  Device* result = Device::make(*physicalDevice);
  if (result) {
    // everything alright
    m_device = result;
  } else {
    LOG_ERROR << "Failed to make Device";
  }
}