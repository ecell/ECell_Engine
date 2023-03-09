#pragma once

#include "imgui.h"

//forward declaration
namespace ECellEngine::Editor
{
	class Editor;
}

namespace ECellEngine::Editor
{
	/*
	@brief Base class to write any widget of the editor.
	*/
	class Widget
	{
	protected:
		Editor& editor;

	public:

		Widget(Editor& _editor) : editor(_editor)
		{

		}

		/*
		@brief To be executed once on its first frame.
		*/
		virtual void Awake() = 0;

		/*
		@brief The method to call each frame to draw the content of the widget.
		*/
		virtual void Draw() = 0;

	};
}

