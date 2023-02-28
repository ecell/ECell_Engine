#include "Main.hpp"

/*
Initializes and runs the engine & editor.
*/
int main()
{
	ECellEngine::Editor::Editor editor;

	editor.Start();
	editor.Update();
	
	editor.Stop();

	return 0;
}
