#include <cmath>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>

#include "Application.h"

static constexpr char	MAINWINDOW_TITLE[] = "Vulkan Tutorial - Hello triangle!";

static constexpr int	MAINWINDOW_SIZE_W = 800;
static constexpr int	MAINWINDOW_SIZE_H = 600;

static constexpr char	SHADER_FILE_VERTEX[] = "Shader.vert.spv";
static constexpr char	SHADER_FILE_FRAGMENT[] = "Shader.frag.spv";

Application::Application()
{
	initialize();
}

Application::~Application()
{
	deinitialize();
}

void Application::run()
{
	while (glfwWindowShouldClose(m_window_ptr) == false)
	{
		glfwPollEvents();
	}
}

void Application::initialize()
{
	// GLFW initialization:
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Create GLFM main window
	m_window_ptr = glfwCreateWindow(MAINWINDOW_SIZE_W, MAINWINDOW_SIZE_H, MAINWINDOW_TITLE, nullptr, nullptr);

	// Vulkan initialization:
	uint32_t glfwExtensionCount = 0;
	const char ** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "The Absofuckulently Best Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
	{
		throw std::exception("vkCreateInstance() failed.");
	}

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::exception("vkEnumeratePhysicalDevices() returned 0 devices.");
	}

	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);

	// Enumerate physical devices
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, physicalDevices.data());

	// Select first device (cos I don't give a shit)
	physicalDevice = physicalDevices[0];

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

	// Enumerate physical device's queue families
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	uint32_t graphicsQueueFamilyIndex = -1;
	for (uint32_t index = 0; index < queueFamilyCount; ++index)
	{
		if (queueFamilies[index].queueCount < 0)
		{
			continue;
		}
		
		if ((queueFamilies[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0x0)
		{
			continue;
		}

		// Remember graphics queue family
		graphicsQueueFamilyIndex = index;
	}

	float queuePriority = 1.0f;

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {};

	std::vector<const char * > deviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = uint32_t(deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceCreateInfo.enabledLayerCount = 0;

	// Create logical device
	if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &m_device) != VK_SUCCESS)
	{
		throw std::exception("vkCreateDevice() failed.");
	}

	// Fetch logical device command queue
	vkGetDeviceQueue(m_device, graphicsQueueFamilyIndex, 0, &m_graphicsQueue);

	// Create presentation surface in GLFW window
	if (glfwCreateWindowSurface(m_instance, m_window_ptr, nullptr, &m_surface) != VK_SUCCESS)
	{
		throw std::exception("glfwCreateWindowSurface() failed.");
	}

	// Fetch surface capabilities
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_surface, &surfaceCapabilities);

	// Fetch device's supported surface formats
	uint32_t surfaceFormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &surfaceFormatCount, nullptr);

	if (surfaceFormatCount == 0)
	{
		throw std::exception("vkGetPhysicalDeviceSurfaceFormatsKHR() returned 0 surface formats.");
	}

	std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &surfaceFormatCount, surfaceFormats.data());

	m_surfaceFormat = surfaceFormats[0];

	// Fetch device's surface supported presentation modes
	uint32_t surfacePresentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &surfacePresentModeCount, nullptr);

	if (surfacePresentModeCount == 0)
	{
		throw std::exception("vkGetPhysicalDeviceSurfacePresentModesKHR() returned 0 presentation modes.");
	}

	std::vector<VkPresentModeKHR> surfacePresentModes(surfacePresentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &surfacePresentModeCount, surfacePresentModes.data());
	
	VkPresentModeKHR surfacePresentMode = surfacePresentModes[0];
		
	// Setup surface extent
	m_surfaceExtent = surfaceCapabilities.currentExtent;
	
	if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
	{
		m_surfaceExtent.width = MAINWINDOW_SIZE_W;
	}

	if (surfaceCapabilities.currentExtent.height == std::numeric_limits<uint32_t>::max())
	{
		m_surfaceExtent.height = MAINWINDOW_SIZE_H;
	}

	// Find surface dimensions capabilities and size intersection
	m_surfaceExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, m_surfaceExtent.width));
	m_surfaceExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, m_surfaceExtent.height));

	// Swapchain image count
	uint32_t swapchainImageCountRequest = surfaceCapabilities.minImageCount + 1;

	if (swapchainImageCountRequest > surfaceCapabilities.maxImageCount)
	{
		swapchainImageCountRequest = surfaceCapabilities.maxImageCount;
	}

	// Swapchain queues
	uint32_t queueFamilyIndices[] = {graphicsQueueFamilyIndex};

	// Create swapchain
	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = m_surface;
	swapchainCreateInfo.minImageCount = swapchainImageCountRequest;
	swapchainCreateInfo.imageFormat = m_surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = m_surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = m_surfaceExtent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount = sizeof(queueFamilyIndices);
	swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = surfacePresentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &m_swapchain) != VK_SUCCESS)
	{
		throw std::exception("vkCreateSwapchainKHR() failed.");
	}

	// Swapchain images
	uint32_t swapchainImageCount = 0;
	vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchainImageCount, nullptr);

	if (swapchainImageCount != swapchainImageCountRequest)
	{
		throw std::exception("vkGetSwapchainImagesKHR() returned different image count than requested.");
	}

	m_swapchainImages.resize(swapchainImageCount);

	vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchainImageCount, m_swapchainImages.data());

	// Swapchain images view
	m_swapchainImagesViews.resize(swapchainImageCount);

	for (std::size_t index = 0; index < m_swapchainImages.size(); ++index)
	{
		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = m_swapchainImages[index];
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = m_surfaceFormat.format;
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &m_swapchainImagesViews[index]) != VK_SUCCESS)
		{
			throw std::exception("vkCreateImageView() failed.");
		}
	}

	// Load compiled vertex shader:
	auto vertexShaderBytecode = readFile(SHADER_FILE_VERTEX);

	VkShaderModuleCreateInfo vertexShaderCreateInfo = {};
	vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vertexShaderCreateInfo.codeSize = vertexShaderBytecode.size();
	vertexShaderCreateInfo.pCode = vertexShaderBytecode.data();

	if (vkCreateShaderModule(m_device, &vertexShaderCreateInfo, nullptr, &m_vertexShader) != VK_SUCCESS)
	{
		throw std::exception("vkCreateShaderModule() failed for vertex shader moddule.");
	}

	// Load compiled fragment shader:
	auto fragmentShaderBytecode = readFile(SHADER_FILE_FRAGMENT);

	VkShaderModuleCreateInfo fragmentShaderCreateInfo = {};
	fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	fragmentShaderCreateInfo.codeSize = vertexShaderBytecode.size();
	fragmentShaderCreateInfo.pCode = vertexShaderBytecode.data();

	if (vkCreateShaderModule(m_device, &fragmentShaderCreateInfo, nullptr, &m_fragmentShader) != VK_SUCCESS)
	{
		throw std::exception("vkCreateShaderModule() failed for fragment shader moddule.");
	}

	// Vertex shader stage:
	VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
	vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderStageInfo.module = m_vertexShader;
	vertexShaderStageInfo.pName = "main";

	// Fragment shader stage:
	VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
	fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentShaderStageInfo.module = m_fragmentShader;
	fragmentShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = 
	{
		vertexShaderStageInfo,
		fragmentShaderStageInfo
	};
}

void Application::deinitialize()
{
	vkDestroyShaderModule(m_device, m_fragmentShader, nullptr);
	vkDestroyShaderModule(m_device, m_vertexShader, nullptr);

	for (auto imageView : m_swapchainImagesViews)
	{
		vkDestroyImageView(m_device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	vkDestroyDevice(m_device, nullptr);
	vkDestroyInstance(m_instance, nullptr);

	if (m_window_ptr != nullptr)
	{
		glfwDestroyWindow(m_window_ptr);
	}
	
	glfwTerminate();
}

std::vector<std::uint32_t> Application::readFile(const std::string & filename) const
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (file.is_open() == false)
	{
		throw std::exception("std::ifstream() failed (couldn't open the file).");
	}

	std::size_t size = std::size_t(file.tellg());

	std::vector<std::uint32_t> bytecode(size);
	file.seekg(0);

	for (std::size_t n = 0; n < size; ++n)
	{
		char streamchar;
		file.get(streamchar);
		bytecode[n] = std::uint32_t(streamchar);
	}

	file.close();

	return bytecode;
}
