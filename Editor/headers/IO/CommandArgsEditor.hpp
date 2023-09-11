#pragma once

/*!
@file AddMNBVContextCommandArgs.h
@brief Defines convenience data structures to be used as arguments for commands
		in the editor.
*/

#include "IO/CommandArgs.hpp"

namespace ECellEngine::Editor::IO
{
	struct AddMNBVContextCommandArgs
	{
		unsigned long long simulationID;
	};
}