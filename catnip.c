/*
  --- TODO LIST ---

  - Open ChatGPT to see what's next after creating the logical device and set as todo the next steps
  - Enable all the vulkan validation layers
  - Create a Vulkan Logical Device

  --- WOULD BE NICE ---

  - Use the full sources version of SDL so we can debug comfortably when needed
*/


#include <stdio.h>
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

VkInstance Vulkan_CreateInstance(SDL_Window *Window, char *ApplicationName, char *EngineName)
{
    // TODO: Application Version, Engine Version as inputs!

    VkInstance Instance = {0};

    // VkApplicationInfo - Structure specifying application information
    VkApplicationInfo ApplicationInfo = {0};
    ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    ApplicationInfo.pNext = NULL;
    ApplicationInfo.pApplicationName = ApplicationName;
    ApplicationInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    ApplicationInfo.pEngineName = EngineName;
    ApplicationInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    ApplicationInfo.apiVersion = VK_API_VERSION_1_3;

    // Get the names of the Vulkan instance extensions needed to
    // create a surface that works with SDL, as of 4/03/2023
    // SDLExtensions is always null, probably no extra extensions are
    // needed to create a windows vulkan instance
    u32 ExtensionCount = 0;
    char *SDLExtensions = NULL;
#pragma warning(suppress: 4013)
    SDL_Vulkan_GetInstanceExtensions(Window, &ExtensionCount, &SDLExtensions);

    // VkInstanceCreateInfo - Structure specifying parameters of a newly created instance
    VkInstanceCreateInfo CreateInfo = {0};
    CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    CreateInfo.pNext = NULL;
    CreateInfo.flags = 0;
    CreateInfo.pApplicationInfo = &ApplicationInfo;
    CreateInfo.enabledLayerCount = 0;
    CreateInfo.ppEnabledLayerNames = NULL;
    CreateInfo.enabledExtensionCount = ExtensionCount;
    CreateInfo.ppEnabledExtensionNames = &SDLExtensions;

    VkResult Result = vkCreateInstance(&CreateInfo, NULL, &Instance);

    if(Result != VK_SUCCESS)
    {
        SDL_LogError(0, "Could not create a vulkan instnace, vkCreateInstance failed");
    }
    else
    {
        SDL_LogInfo(0, "Vulkan instance created successfully");
    }

    { // Print supported extensions

        // TODO: Once we have our custom dynamic array implementation
        // or our custom memory allocator, replace malloc and free
        // with them

        u32 SupportedExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(0, &SupportedExtensionCount, 0);

        VkExtensionProperties *ExtensionProperties = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties)*SupportedExtensionCount);
        vkEnumerateInstanceExtensionProperties(0, &SupportedExtensionCount, ExtensionProperties);

        SDL_Log("Found %d Supported vulkan extensions", SupportedExtensionCount);
        for(u32 i = 0; i < SupportedExtensionCount; i++)
        {
            SDL_Log("\t- %s", ExtensionProperties[i].extensionName);
        }

        free(ExtensionProperties);
    }

    return Instance;
}

void Vulkan_EnableValidationLayers()
{
    // TODO: Enable the validation layers!
}

int main(int Argc, char **Argv)
{
    Argc; Argv;

    SDL_Init(SDL_INIT_VIDEO);

    i32 Width = 1280;
    i32 Height = 720;
    SDL_Window *Window = SDL_CreateWindow("Catnip", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

    VkInstance VulkanInstance = Vulkan_CreateInstance(Window, "Catnip", "Catnip Engine");
    Vulkan_EnableValidationLayer();

    while(Running)
    {
        HandleEvents();
    }

    vkDestroyInstance(VulkanInstance, 0);
    SDL_DestroyWindow(Window);

    return 0;
}
