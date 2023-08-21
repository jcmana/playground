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

    void drawFrame();

	std::vector<char> readSharedFile(const std::string & filename) const;

private:
	/// \brief		Main application window.
	GLFWwindow * m_window_ptr = nullptr;

    // Pipeline:
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

    VkPipelineLayout m_pipelineLayout;
    VkRenderPass m_renderPass;
    VkPipeline m_pipeline;

    // Framebuffers:
    std::vector<VkFramebuffer> m_framebuffers;

    // Commands:
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;

    // Synchronization:
    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
};
