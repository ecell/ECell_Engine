#pragma once
#include <type_traits>
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort


#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>


namespace ECellEngine::Editor::Utility
{
	static void CheckVkResult(VkResult err)
	{
		if (err == 0)
			return;
		fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
		if (err < 0)
			abort();
	}

#ifdef IMGUI_VULKAN_DEBUG_REPORT
	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode,
		const char* pLayerPrefix, const char* pMessage, void* pUserData)
	{
		(void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
		fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
		return VK_FALSE;
	}
#endif // IMGUI_VULKAN_DEBUG_REPORT

	static void GlfwErrorCallback(int error, const char* description)
	{
		fprintf(stderr, "Glfw Error %d: %s\n", error, description);
	}

	/**
	* @brief Utility to initialize glfw + Vulkan backends for ImGui.
	* @remarks Most of the code here comes from ImGui demo. See: 
	*			https://github.com/ocornut/imgui/blob/master/examples/example_glfw_vulkan/main.cpp
	*/
	struct BackendUtility
	{
		GLFWwindow* window;

		VkResult				 err;
		VkAllocationCallbacks*	 allocator;
		VkInstance               instance;
		VkPhysicalDevice         physicalDevice;
		VkDevice                 device;
		uint32_t                 queueFamily;
		VkQueue                  queue;
		VkDebugReportCallbackEXT debugReport;
		VkPipelineCache          pipelineCache;
		VkDescriptorPool         descriptorPool;

		ImGui_ImplVulkanH_Window mainWindowData;
		int                      minImageCount;
		bool                     swapChainRebuild;

		BackendUtility()
		{
			window = NULL;

			err = VK_ERROR_UNKNOWN;
			allocator = NULL;
			instance = VK_NULL_HANDLE;
			physicalDevice = VK_NULL_HANDLE;
			device = VK_NULL_HANDLE;
			queueFamily = (uint32_t)-1;
			queue = VK_NULL_HANDLE;
			debugReport = VK_NULL_HANDLE;
			pipelineCache = VK_NULL_HANDLE;
			descriptorPool = VK_NULL_HANDLE;

			mainWindowData;
			minImageCount = 2;
			swapChainRebuild = false;
		}

		/**
		* @brief To call on window closing to clean everything.
		*/
		inline void CleanUp()
		{
			err = vkDeviceWaitIdle(device);
			CheckVkResult(err);
			ImGui_ImplVulkan_Shutdown();
			ImGui_ImplGlfw_Shutdown();

			CleanupVulkanWindow();
			CleanupVulkan();

			glfwDestroyWindow(window);
			glfwTerminate();
		}

		void CleanupVulkan();

		void CleanupVulkanWindow();

		void FrameRender(ImDrawData* _draw_data);

		void FramePresent();

		void InitializeEditorWindow();

		void InitializeVulkan(const char** _extensions, uint32_t _extensions_count);

		void InitializeVulkanWindow(ImGui_ImplVulkanH_Window* _wd, VkSurfaceKHR _surface, int _width, int _height);

		void ResizeSwapChain();

		void VulkanFontUpload();
	};
}