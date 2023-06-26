#include <stdio.h>
#include <string.h>
#include <SDL.h>

#include <vulkan/vulkan.h>

#include "typedefs.h"

global b32 Running = 1;

void HandleEvents()
{
    SDL_Event Event;
    while(SDL_PollEvent(&Event))
    {
        switch(Event.type)
        {
            case SDL_QUIT:
            {
                Running = 0;
                break;
            }
            case SDL_KEYDOWN:
            {
                if(Event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    Running = 0;
                }
                break;
            }
            case SDL_KEYUP:
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

/*
  --- TODO LIST ---

  - Move to C++
  - Implement logging to stdout & text file
  - Log everything we have up until now
  - Separate into functions, or at least into {} scopes
  - Fix Validation error

 */

int main(int Argc, char **Argv)
{
    Argc; Argv;

    SDL_Init(SDL_INIT_VIDEO);

    i32 Width = 1280;
    i32 Height = 720;
    SDL_Window *Window = SDL_CreateWindow("Catnip", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

    // Query vulkan instance extensions before create the vulkan insta
    // TODO(Jorge): Log, write to stdou all available extensions
    u32 InstanceExtensionsCount = 0;
    VkExtensionProperties *InstanceExtensionProperties = NULL;
    vkEnumerateInstanceExtensionProperties(NULL, &InstanceExtensionsCount, NULL);
    if(InstanceExtensionsCount > 0)
    {
        InstanceExtensionProperties = malloc(sizeof(VkExtensionProperties) * InstanceExtensionsCount);
        vkEnumerateInstanceExtensionProperties(NULL, &InstanceExtensionsCount, InstanceExtensionProperties);
    }

    VkApplicationInfo AppInfo = {0};
    VkInstanceCreateInfo InstanceInfo = {0};
    VkInstance VulkanInstance = {0};

    AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    AppInfo.pNext = NULL;
    AppInfo.pApplicationName = "Catnip";
    AppInfo.applicationVersion = 0;
    AppInfo.pEngineName = "Catnip Renderer";
    AppInfo.engineVersion = 0;
    AppInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

#if 0
    //
    // TODO: When enabling extensions, make sure to add SDLExtentions & ExtensionCount to the other extensions enabled
    //

    // Get the names of the Vulkan instance extensions needed to
    // create a surface that works with SDL, as of 4/03/2023
    // SDLExtensions is always null, probably no extra extensions are
    // needed to create a windows vulkan instance
    u32 ExtensionCount = 0;
    char *SDLExtensions = NULL;
    SDL_Vulkan_GetInstanceExtensions(Window, &ExtensionCount, &SDLExtensions);
#endif

    // Query available instance layers
    // TODO(Jorge): Print, Log all available instance layers
    u32 InstanceLayerCount = 0;
    VkLayerProperties *InstanceLayerProperties = NULL;
    vkEnumerateInstanceLayerProperties(&InstanceLayerCount, NULL);
    if(InstanceLayerCount > 0)
    {
        InstanceLayerProperties = malloc(sizeof(VkLayerProperties) * InstanceLayerCount);
        vkEnumerateInstanceLayerProperties(&InstanceLayerCount, InstanceLayerProperties);
    }

    const char *ValidationLayerName = "VK_LAYER_KHRONOS_validation";

    InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    InstanceInfo.pNext = NULL;
    InstanceInfo.flags = 0;
    InstanceInfo.pApplicationInfo = &AppInfo;
    InstanceInfo.enabledLayerCount = 1;      // Number of instance layers to enable
    InstanceInfo.ppEnabledLayerNames = &ValidationLayerName; // Name of the instance layers to enable
    InstanceInfo.enabledExtensionCount = 0;
    InstanceInfo.ppEnabledExtensionNames = NULL;

    VkResult InstanceResult = vkCreateInstance(&InstanceInfo, NULL, &VulkanInstance);
    Assert(InstanceResult == VK_SUCCESS);

    u32 PhysicalDeviceCount = 0;
    VkPhysicalDevice *PhysicalDevices = NULL;
    vkEnumeratePhysicalDevices(VulkanInstance, &PhysicalDeviceCount, NULL);
    Assert(PhysicalDeviceCount == 1); // My dev PC only has 1 device, no logic for multiple devices implemented
    PhysicalDevices = malloc(sizeof(VkPhysicalDevice) * PhysicalDeviceCount);
    vkEnumeratePhysicalDevices(VulkanInstance, &PhysicalDeviceCount, PhysicalDevices);

    VkPhysicalDeviceProperties PhysicalDeviceProperties = {0};
    VkPhysicalDeviceFeatures PhysicalDeviceFeatures = {0};
    VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties = {0};
    vkGetPhysicalDeviceProperties(PhysicalDevices[0], &PhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(PhysicalDevices[0], &PhysicalDeviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(PhysicalDevices[0], &PhysicalDeviceMemoryProperties);

    // Query the physical device for its queue families
    // TODO(Jorge): LOG all the queue families to stdout or a log file
    u32 QueueFamilyCount = 0;
    VkQueueFamilyProperties *QueueFamilyProperties = NULL;
    vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevices[0], &QueueFamilyCount, NULL);
    QueueFamilyProperties = malloc(sizeof(VkQueueFamilyProperties) * QueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevices[0], &QueueFamilyCount, QueueFamilyProperties);

    // Query the physical device avialable layers
    // TODO(Jorge): Log, print available layers
    u32 PhysicalDeviceLayerCount = 0;
    VkLayerProperties *PhysicalDeviceLayerProperties = NULL;
    vkEnumerateDeviceLayerProperties(PhysicalDevices[0], &PhysicalDeviceLayerCount, NULL);
    if(PhysicalDeviceLayerCount > 0)
    {
        PhysicalDeviceLayerProperties = malloc(sizeof(VkLayerProperties) * PhysicalDeviceLayerCount);
        vkEnumerateDeviceLayerProperties(PhysicalDevices[0], &PhysicalDeviceLayerCount, PhysicalDeviceLayerProperties);
    }

    // Create the logical device

    // Query device extensions
    // TODO(Jorge): Log to file || stdout
    u32 DeviceExtensionPropertyCount = 0;
    VkExtensionProperties *DeviceExtensionProperties = NULL;
    vkEnumerateDeviceExtensionProperties(PhysicalDevices[0], NULL, &DeviceExtensionPropertyCount, NULL);
    if(DeviceExtensionPropertyCount > 0)
    {
        DeviceExtensionProperties = malloc(sizeof(VkExtensionProperties) * DeviceExtensionPropertyCount);
        vkEnumerateDeviceExtensionProperties(PhysicalDevices[0], NULL, &DeviceExtensionPropertyCount, DeviceExtensionProperties);
    }


    VkDevice LogicalDevice;
    VkResult LogicalDeviceResult;
    VkPhysicalDeviceFeatures SupportedFeatures = {0};
    VkPhysicalDeviceFeatures RequiredFeatures = {0};

    vkGetPhysicalDeviceFeatures(PhysicalDevices[0], &SupportedFeatures);

    // TODO(Jorge): Store RequiredFeatures so we can poll for features. Maybe rename to EnabledFeatures?
    RequiredFeatures.multiDrawIndirect = SupportedFeatures.multiDrawIndirect;
    RequiredFeatures.tessellationShader = VK_TRUE;
    RequiredFeatures.geometryShader = VK_TRUE;

    VkDeviceQueueCreateInfo DeviceQueueCreateInfo = {0};
    DeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    DeviceQueueCreateInfo.pNext = NULL;
    DeviceQueueCreateInfo.flags = 0;
    DeviceQueueCreateInfo.queueFamilyIndex = 0;
    DeviceQueueCreateInfo.queueCount = 1;
    DeviceQueueCreateInfo.pQueuePriorities = NULL;

    VkDeviceCreateInfo DeviceCreateInfo = {0};
    DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    DeviceCreateInfo.pNext = NULL;
    DeviceCreateInfo.flags = 0;
    DeviceCreateInfo.queueCreateInfoCount = 1;
    DeviceCreateInfo.pQueueCreateInfos = &DeviceQueueCreateInfo;
    DeviceCreateInfo.enabledLayerCount = 0;
    DeviceCreateInfo.ppEnabledLayerNames = NULL;
    DeviceCreateInfo.enabledExtensionCount = 0;
    DeviceCreateInfo.ppEnabledExtensionNames = NULL;
    DeviceCreateInfo.pEnabledFeatures = &RequiredFeatures;

    LogicalDeviceResult = vkCreateDevice(PhysicalDevices[0], &DeviceCreateInfo, NULL, &LogicalDevice);

    while(Running)
    {
        HandleEvents();
    }

    // Destroy Vulkan in reverse order
    vkDeviceWaitIdle(LogicalDevice); // Complete all the work on the specified device
    vkDestroyDevice(LogicalDevice, NULL);
    // There is no need to destroy physical devices. Physical devices
    // are not created with a dedicated creation function as logical
    // devices are. Physical devices are returned from a call to
    // vkEnumeratePhysicalDevices and are considered to be owned byu
    // the instance.
    vkDestroyInstance(VulkanInstance, NULL);

    SDL_DestroyWindow(Window);

    return 0;
}
