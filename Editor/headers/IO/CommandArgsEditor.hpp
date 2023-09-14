#pragma once

/*!
@file AddMNBVContextCommandArgs.h
@brief Defines convenience data structures to be used as arguments for commands
		in the editor.
@remark This could be more concise if we used a template class to define the
		arguments of the commands. But it would be less readable and more
		complex to use.
*/

#include "IO/CommandArgs.hpp"

namespace ECellEngine::Editor::IO
{
	struct AddMNBVContextCommandArgs
	{
		unsigned long long simulationID = 0;
	};

	struct FocusNodeCommandArgs
	{
		unsigned short contextIdx = 0;
		unsigned long long nodeID = 0;
	};
}