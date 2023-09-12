#pragma once

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui.h"
#include "implot.h"

#include "Core/Engine.hpp"
#include "Core/IDProvider.hpp"
#include "IO/CommandsManager.hpp"
#include "IO/CommandsEditor.hpp"
#include "Logging/ExeConsoleLoggerSink.hpp"
#include "Utility/BackendUtility.hpp"
#include "Widget/Widget.hpp"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

namespace ECellEngine::Editor
{
	class Editor
	{
	private:
		
		/*!
		@brief Instance of the utility to handle start and clean up of Vulkan and GLFW.
		*/
		Utility::BackendUtility backend;

		/*!
		@brief The commands manager to handle the commands of the editor.
		*/
		ECellEngine::IO::CommandsManager commandsManager;

		/*!
		@brief The Logger to print information in the console of the executable.
		*/
		ECellEngine::Editor::Logging::ExeConsoleLoggerSink exeLoggerSink;
		
		/*!
		@brief List of newly created widgets.
		@details This contains pointers to the widgets only for 1 frame. Just to
				 give them enough time to execute Widget::Awake()
		*/
		std::vector<Widget::Widget*> newWidgets;

		/*!
		@brief List of all widgets to draw.
		@details This contains pointers to the widgets as long as we need to draw
				 them. Widget::Draw() is called for each of them every frame.
		*/
		std::vector<Widget::Widget*> widgets;

		/*!
		@brief Creates ImGui contexts, sets the style, the main flags and generates the fonts.
		*/
		void InitializeImGui();

		/*!
		@brief Reprocess the font Atlas. Mainly to have clearer glyphs in the node editor (but applied everywhere).
		@author thedmd at https://github.com/thedmd/imgui-node-editor/blob/2f99b2d613a400f6579762bd7e7c343a0d844158/examples/application/source/application.cpp#L88
		*/
		void RecreateFontAtlas();

	public:

		/*!
		@brief The ID provider to generate unique IDs for the widgets.
		*/
		ECellEngine::Core::IDProvider idProvider;

		/*!
		@brief The ECellEngine instance.
		*/
		ECellEngine::Core::Engine engine;

		Editor();

		/*!
		@brief Returns the commands manager of the editor.
		*/
		inline ECellEngine::IO::CommandsManager& GetCommandsManager()
		{
			return commandsManager;
		}

		/*!
		@brief Opens a new window in the editor.
		@details Instantiates a new widget of type @p WidgetType and adds it to
				 widgets as well as newWidgets.
		@tparam WidgetType MUST be deriving from Widget.
		*/
		template <typename WidgetType, typename = std::enable_if_t<std::is_base_of_v<Widget::Widget, WidgetType>>>
		inline WidgetType*	AddWidget()
		{
			widgets.push_back(new WidgetType(this));
			newWidgets.push_back(widgets.back());
			return static_cast<WidgetType*>(widgets.back());
		}

		/*! 
		@brief Initialize ImGui contexts, Vulkan + Glfw backends and starts the Engine.
		*/
		void Start();

		/*!
		@brief Stops the engine, destroy ImGui contexts and clean ups Vulkan + Glfw backends.
		*/
		void Stop();

		/*!
		@brief The main loop of the editor.
		@details Runs on a while loop as long as Glfw window context is running.
				 One loop cycle is one frame. It starts by polling events, updating
				 the engine. Then comes the ImGui new frame and renders.
		*/
		void Update();
	};
}