#pragma once

#include "Widget.hpp"

namespace ECellEngine::Editor::Widget
{
	/*!
	  @brief The window with a variety of options for the editor.
	  @remarks May be completely refactored or deleted if not needed anymore.
	*/
	class OptionsWidget : public Widget
	{
	public:
		OptionsWidget(Editor& _editor) :
			Widget(_editor)
		{

		}

		inline void Awake() override {};

		void Draw() override;
	};
}
