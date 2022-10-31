#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "implot.h"

#include "Engine.hpp"
#include "EngineDataVisualizationWidget.hpp"
#include "FileIOWidget.hpp"
#include "OptionsWidget.hpp"
#include "SimulationFlowControlWidget.hpp"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

using namespace ECellEngine::Core;

namespace ECellEngine::Editor
{
	class Editor
	{
	private:
		Engine engine;

		GLFWwindow* window;

		VkResult				 err;
		VkAllocationCallbacks* allocator;
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

		EngineDataVisualizationWidget edvWidget;
		FileIOWidget fileIOWidget;
		OptionsWidget optionsWidget;
		SimulationFlowControlWidget sfcWidget;


		void cleanupVulkan();

		void cleanupVulkanWindow();

		void frameRender(ImGui_ImplVulkanH_Window* _wd, ImDrawData* _draw_data);

		void framePresent(ImGui_ImplVulkanH_Window* _wd);

		void initializeEditorWindow();

		void initializeVulkan(const char** _extensions, uint32_t _extensions_count);

		void initializeVulkanWindow(ImGui_ImplVulkanH_Window* _wd, VkSurfaceKHR _surface, int _width, int _height);

	public:
		Editor() :
			edvWidget(engine.getCommandsManager(), engine.getSimulationLoop()),
			fileIOWidget(engine.getCommandsManager(), engine.getLoadedSBMLDocuments()),
			optionsWidget(engine.getCommandsManager()),
			sfcWidget(engine.getCommandsManager(), engine.getSimulationLoop())
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

		void start();

		void stop();

		void update();
	};
}