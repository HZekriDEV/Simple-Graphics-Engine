#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;
class HelloTriangleApplication 
{
public:
    void run() 
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window = nullptr;
	vk::raii::Context context;
    vk::raii::Instance instance = nullptr;

    void initWindow()
    {
		glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }
    void initVulkan() 
    {
		std::cout << "Initializing Vulkan..." << std::endl; 
        createVulkanInstance();
    }

    void mainLoop()
    {
        while (!glfwWindowShouldClose(window))
        {
			glfwPollEvents();
        }
    }

    void cleanup() 
    {
		glfwDestroyWindow(window);
		glfwTerminate();
    }

    void createVulkanInstance()
    {
        constexpr vk::ApplicationInfo appInfo{
            .pApplicationName = "Helloo Triangle",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "No Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = vk::ApiVersion14
        };

		// Get required extensions from GLFW
		uint32_t glfwExtensionCount = 0;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::cout << "Available Vulkan extensions:" << std::endl;
        auto extensionProperties = context.enumerateInstanceExtensionProperties();

        for (const auto& extension : extensionProperties)
        {
            std::cout << "\t" << extension.extensionName << std::endl;
		}

        // Check if GLFW extensions are supported by Vulkan
        for (uint32_t i = 0; i < glfwExtensionCount; i++)
        {
            if(std::ranges::none_of(extensionProperties, 
                [glfwExtension = glfwExtensions[i]](auto const& extensionProperty)
				{ return strcmp(extensionProperty.extensionName, glfwExtension) == 0; }))
            {
				throw std::runtime_error("Required GLFW extension not supported by Vulkan: " + std::string(glfwExtensions[i]));
            }
        }

        vk::InstanceCreateInfo createInfo{
            .pApplicationInfo = &appInfo,
			.enabledExtensionCount = glfwExtensionCount,
			.ppEnabledExtensionNames = glfwExtensions
		};

		instance = vk::raii::Instance(context, createInfo);
    }
};

int main()
{
    try
    {
        HelloTriangleApplication app;
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
