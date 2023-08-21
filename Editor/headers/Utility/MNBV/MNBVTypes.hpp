#pragma once

/*!
@file MNBVTypes.h
@brief Contains the various types to describe graphical elements in a
		ECellEngine::Editor::Utility::ModelNodeBasedViewerWidget.
*/

namespace ECellEngine::Editor::Utility::MNBV
{
	/*!
	@brief The various type of nodes that can be exist in a
			ECellEngine::Editor::Utility::ModelNodeBasedViewerWidget.
	*/
	enum NodeType
	{
		NodeType_Default,

		//Asset-Related
		NodeType_Asset,
		NodeType_Parameter,
		NodeType_Reaction,
		NodeType_Solver,
		NodeType_Species,

		//Maths-Related
		NodeType_Maths,
		NodeType_ArithmeticOperation,
		NodeType_LogicOperation,
		NodeType_Equation,

		NodeType_Data,

		NodeType_Event,

		NodeType_Count
	};

	/*!
	@brief The various type of pins that can be exist in a
			ECellEngine::Editor::Utility::ModelNodeBasedViewerWidget.
	*/
	enum PinType
	{
		PinType_Default,

		//Asset-Related
		PinType_Asset,
		PinType_Parameter,
		PinType_Reaction,
		PinType_Solver,
		PinType_Species,

		//Maths-Related
		PinType_Equation,
		PinType_Operand,

		//Low level Data-Related
		PinType_DataStateValueFloat,//"DataState" for a value stored in a data within the DataState (-->there are constraints on how to access/change it).
		PinType_EquationValueFloat,//"Equation" for a value that is the result of the computation of an equation.
		PinType_FreeValueFloat,//"Free" means that the value does not have specific contraints.

		//Event-Related
		PinType_BooleanCallBackPublisher,
		PinType_BooleanCallBackSubscriber,
		PinType_FloatCallBackPublisher,
		PinType_FloatCallBackSubscriber,

		PinType_Count
	};
}