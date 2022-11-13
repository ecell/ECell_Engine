#pragma once

#include "DataState.hpp"

namespace ECellEngine::Data
{
	class Module
	{
	protected:
		DataState* dataState;

	public:
		Module(DataState* _dataState) :
			dataState{_dataState}
		{

		}

		virtual void Initialize() = 0;
	};
}