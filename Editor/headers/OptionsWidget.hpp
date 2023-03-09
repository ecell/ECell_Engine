#pragma once

#include "Widget.hpp"

namespace ECellEngine::Editor
{
	class OptionsWidget : public Widget
	{
	private:

	public:
		OptionsWidget(Editor& _editor) :
			Widget(_editor)
		{

		}

		inline void Awake() override {};

		void Draw() override;
	};
}
