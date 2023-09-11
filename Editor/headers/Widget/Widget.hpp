#pragma once

/*!
@file Widget.h
@brief The base class to write any widget of the editor.
*/

#include <cstddef>
#include "imgui.h"

//forward declaration
namespace ECellEngine::Editor
{
	class Editor;
}

namespace ECellEngine::Editor::Widget
{
	/*!
	@brief Base class to write any widget of the editor.
	*/
	class Widget
	{
	protected:

		std::size_t id = SIZE_MAX;

		ECellEngine::Editor::Editor* editor = nullptr;

	public:

		Widget(ECellEngine::Editor::Editor* _editor);

		virtual ~Widget() = default;

		/*!
		@brief Get the id of the widget.
		*/
		inline std::size_t GetId() const noexcept
		{
			return id;
		}

		/*!
		@brief To be executed once on its first frame.
		*/
		virtual void Awake() = 0;

		/*!
		@brief The method to call each frame to draw the content of the widget.
		*/
		virtual void Draw() = 0;

	};
}

