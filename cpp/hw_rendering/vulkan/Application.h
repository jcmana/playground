#pragma once

#include <cstdint>
#include <exception>
#include <vector>
#include <string>

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

	std::vector<std::uint32_t> readFile(const std::string & filename) const;

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

	VkShaderModule m_vertexShader;
	VkShaderModule m_fragmentShader;
};
