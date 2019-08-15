#pragma once

#include <exception>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void initialize();
	void deinitialize();

private:
	/// \brief		Main application window.
	GLFWwindow * m_window_ptr = nullptr;

	VkInstance m_instance;

	VkDevice m_device;

	VkSurfaceKHR m_surface;
	VkSurfaceFormatKHR m_surfaceFormat;
	VkExtent2D m_surfaceExtent;

	VkQueue m_graphicsQueue;
	VkQueue m_presentationQueue;

	VkSwapchainKHR m_swapchain;

	std::vector<VkImage> m_swapchainImages;
	std::vector<VkImageView> m_swapchainImagesViews;
};
