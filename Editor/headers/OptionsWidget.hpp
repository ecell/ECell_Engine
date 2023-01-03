#pragma once

#include "Widget.hpp"
#include "WidgetsVisibility.hpp"

using namespace ECellEngine::IO;

namespace ECellEngine::Editor
{
	class OptionsWidget : public Widget
	{
	private:
		bool showDemoWindow;

	public:
		OptionsWidget(CommandsManager* _cmdManager) :
			Widget(_cmdManager)
		{
			showDemoWindow = false;
		}

		/*
		@brief Accessor to get the @a showDemoWindow private member
		*/
		inline bool* getShowDemoWindow()
		{
			return &showDemoWindow;
		}

		void Draw() override;
	};
}
