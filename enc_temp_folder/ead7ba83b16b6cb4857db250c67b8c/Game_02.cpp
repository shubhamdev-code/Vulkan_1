#include <iostream>
#include <vulkan/vulkan.h>
#include <volk.h>
#include <vector>
#include <GLFW/glfw3.h>

static const int WIDTH = 1280;
static const int HEIGHT	= 720;
static const char* APP_NAME = "Game_02";

GLFWwindow* Window = nullptr;
VkInstance Instance = VK_NULL_HANDLE;


void Terminate() {

    if (Instance != VK_NULL_HANDLE) {
        //pointer to function vkDestroyInstance
        vkDestroyInstance(Instance, nullptr); 
    }

    glfwTerminate();
}

VkResult SetupVulkan(uint32_t layers_count,
    const char** layer_names,
    uint32_t extensions_count,
    const char** extension_names)
{
    VkResult res;
    
    // create vulkan instance
    {

        VkInstanceCreateInfo instance_ci;
        instance_ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_ci.pNext = nullptr;
        instance_ci.flags = 0;
        instance_ci.pApplicationInfo = nullptr;
        instance_ci.enabledLayerCount = layers_count;
        instance_ci.ppEnabledLayerNames = layer_names;
        instance_ci.enabledExtensionCount = 1;
        instance_ci.ppEnabledExtensionNames = extension_names;

        res = vkCreateInstance(&instance_ci, nullptr, &Instance);
        if (res != VK_SUCCESS) {
            std::cerr << "failed to create vulkan instance\n";
            return res;
        }

        volkLoadInstanceOnly(Instance); 
    }
    // Select Physical Device
    {

        uint32_t NumGPUs = 0;
      
        res = vkEnumeratePhysicalDevices(Instance, &NumGPUs, nullptr); 

        if (res != VK_SUCCESS) 
            return res;
        
        std::vector<VkPhysicalDevice> GPUs(NumGPUs);
        vkEnumeratePhysicalDevices(Instance, &NumGPUs, GPUs.data());

        for (uint32_t i = 0; i < GPUs.size(); i++) {

            VkPhysicalDeviceProperties properties;

            vkGetPhysicalDeviceProperties(GPUs[i], &properties);

            std::cout << properties.deviceName<<std::endl;
        }  
    }
    return VK_SUCCESS;
}

int main()
{
    if (!glfwInit())
        return EXIT_FAILURE;

    if (volkInitialize()) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    Window = glfwCreateWindow(WIDTH, HEIGHT, APP_NAME, nullptr, nullptr);
    if (!Window) {
        std::cout << "Failed to create GLFW window\n";
        return EXIT_FAILURE;
    }

    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    if (SetupVulkan(0, nullptr, glfw_extension_count, glfw_extensions) != VK_SUCCESS) {
        Terminate();
        return EXIT_FAILURE;
    }

    while (!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
    }

    Terminate();
    return EXIT_SUCCESS;
}
