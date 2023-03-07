#pragma once
#include "imgui.h"
#include "implot.h"

#include "BackendUtility.hpp"
#include "Engine.hpp"
#include "Logger.hpp"
#include "ExeConsoleLoggerSink.hpp"
#include "ConsoleWidget.hpp"
#include "OptionsWidget.hpp"
#include "SimulationWidget.hpp"

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
		
		Utility::BackendUtility backend;

		ECellEngine::Editor::Logging::ExeConsoleLoggerSink excLoggerSink;
		std::vector<Widget*> widgets;

		void InitializeImGui();

		/*
		@brief Reprocess the font Atlas. Mainly to have clearer glyphs in the node editor (but applied everywhere).
		@author thedmd at https://github.com/thedmd/imgui-node-editor/blob/2f99b2d613a400f6579762bd7e7c343a0d844158/examples/application/source/application.cpp#L88
		*/
		void RecreateFontAtlas();

	public:
		Engine engine;
		
		bool showDemoWindow; //for debug puproses

		Editor()
		{
			ECellEngine::Logging::Logger::GetSingleton().AddSink(&excLoggerSink);

			AddWidget<ConsoleWidget>();
			AddWidget<OptionsWidget>();
			AddWidget<SimulationWidget>();

			showDemoWindow = false;
		}

		template <typename WidgetType, typename = std::enable_if_t<std::is_base_of_v<Widget, WidgetType>>>
		inline WidgetType*	AddWidget()
		{
			widgets.push_back(new WidgetType(*this));
			return static_cast<WidgetType*>(widgets.back());
		}

		void Start();

		void Stop();

		void Update();
	};
}