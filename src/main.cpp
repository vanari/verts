#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//#define NDEBUG

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 800;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

bool checkValidationLayerSupport();

class HelloTriangleApp {
	public:
		void run() {
			initWindow();
			initVulkan();
			mainLoop();
			cleanup();
		}
	
	private:
		GLFWwindow *window;

		VkInstance instance;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	private:

		void initWindow() {
			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		}

		void initVulkan() {
			createInstance();
			//setupDebugMessenger();
			pickPhysicalDevice();
		}

		void pickPhysicalDevice() {
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

			if (deviceCount == 0) {
				throw std::runtime_error("failde to find GPUs with Vulkan support!");
			}

			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

			std::cout << std::endl << "devices(" << deviceCount << "):" << std::endl;
			for (const auto& device : devices) {
				if (isDeviceSuitable(device)) {
					physicalDevice = device;
					//break;
				}
			}

			if (physicalDevice == VK_NULL_HANDLE) {
				throw std::runtime_error("failed to find a suitable GPU!");
			}
		}

		bool isDeviceSuitable(VkPhysicalDevice device) {
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			std::cout << '\t' << deviceProperties.deviceName << std::endl;

			return true;
		}

		void createInstance() {
			if (enableValidationLayers && !checkValidationLayerSupport()) {
				throw std::runtime_error("validation layers requested, but not available!");
			}

			VkApplicationInfo appInfo{
				.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
				.pApplicationName = "Hello Triangle",
				.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
				.pEngineName = "No Engine",
				.engineVersion = VK_MAKE_VERSION(1, 0, 0),
				.apiVersion = VK_API_VERSION_1_0
			};
			
			VkInstanceCreateInfo createInfo {
				.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				.pApplicationInfo = &appInfo
			};

			if (enableValidationLayers) {
				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				createInfo.ppEnabledLayerNames = validationLayers.data();
			} else {
				createInfo.enabledLayerCount = 0;
			}

			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;

			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			createInfo.enabledExtensionCount = glfwExtensionCount;
			createInfo.ppEnabledExtensionNames = glfwExtensions;

			if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
				throw std::runtime_error("failed to create instance!");
			}
			
			uint32_t extensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> extensions(extensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

			std::cout << "available extensions:" << std::endl;

			for (const auto& extension : extensions) {
				std::cout << '\t' << extension.extensionName << std::endl;
			}

			std::cout << std::endl << "needed extensions:" << std::endl;

			for (int i = 0; i < glfwExtensionCount; ++i) {
				std::cout << '\t' << glfwExtensions[i] << std::endl;
			}
		}



		void mainLoop() {
			while (!glfwWindowShouldClose(window)) {
				glfwPollEvents();
			}
		}

		void cleanup() {
			vkDestroyInstance(instance, nullptr);

			glfwDestroyWindow(window);

			glfwTerminate();
		}

};

int main() {
	HelloTriangleApp app;

	try {
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

bool checkValidationLayerSupport() {
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

		if (!layerFound)
			return false;
	}
	
	return true;
}
