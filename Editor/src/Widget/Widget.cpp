#include "Editor.hpp"	// includes Widget.hpp

ECellEngine::Editor::Widget::Widget::Widget(ECellEngine::Editor::Editor* _editor)
{
	editor = _editor;
	id = ++(editor->idProvider);
}