#include "Main.hpp"

/*
Initializes and runs the engine & editor.
*/
int main()
{
	ECellEngine::Editor::Editor editor;

	editor.start();
	editor.update();
	
	editor.stop();

	return 0;
}
