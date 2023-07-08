#pragma once

/*!
@file NodeEditorStyle.hpp
@brief Enums and data to encode the style of each ECellEngine::Editor::Utility::ModelNodeBasedViewerContext.
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
		PinType_ValueBool,

		//Event-Related
		PinType_ModifyDataStateEvent,
		PinType_Trigger,

		PinType_Count
	};

	/*!
	@brief Identifiers for the different aspect of a node which color can be
			customized.
	*/
	enum NodeColorType
	{
		NodeColorType_Bg,
		NodeColorType_Border,
		NodeColorType_HeaderBg,
		NodeColorType_HeaderActivated,
		NodeColorType_HeaderHovered,

		NodeColorType_Count
	};

	/*!
	@brief Identifiers for the different aspect of a pin which color can be
			customized.
	*/
	enum PinColorType
	{
		PinColorType_BgActivated,
		PinColorType_BgInactivated,
		PinColorType_Border,

		PinColorType_Count
	};

	/*!
	@brief The data struct to store the styling variables and colors that are
			used to customize the visuals of the elements in a
			ECellEngine::Editor::Utility::ModelNodeBasedViewerContext.
	*/
	struct NodeEditorStyle
	{
		/*!
		@brief The minimal width of the area of a node that is in-between
				the input and output pins.
		@details This indirectly regulates the minimal width of a node.
		*/
		float nodeCenterAreaMinWidth;

		/*!
		@brief The width of the graphic item representing a node pin.
		*/
		float pinWidth;

		/*!
		@brief The matrix to store all color sets for all node types.
		*/
		ImVec4 nodeColors[NodeType_Count][NodeColorType_Count];

		/*!
		@brief The matrix to store all color sets for all pin types.
		*/
		ImVec4 pinColors[PinType_Count][PinColorType_Count];

		NodeEditorStyle()
		{
			pinWidth = 8.f;
			nodeCenterAreaMinWidth = 200.f;

			nodeColors[NodeType_Default][NodeColorType_Bg] = ImVec4(0.125f, 0.125f, 0.125f, 0.784f);//ax::NodeEditor::StyleColor_NodeBg;
			nodeColors[NodeType_Default][NodeColorType_Border] = ImVec4(1.000f, 1.000f, 1.000f, 0.376f);//ax::NodeEditor::StyleColor_NodeBorder;
			nodeColors[NodeType_Default][NodeColorType_HeaderBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.376f);//ax::NodeEditor::StyleColor_NodeBorder;
			nodeColors[NodeType_Default][NodeColorType_HeaderActivated] = ImVec4(1.000f, 0.690f, 0.196f, 1.000f);//ax::NodeEditor::StyleColor_SelNodeBorder;
			nodeColors[NodeType_Default][NodeColorType_HeaderHovered] = ImVec4(0.196f, 0.690f, 1.000f, 1.000f);//ax::NodeEditor::StyleColor_HovNodeBorder;

			nodeColors[NodeType_Asset][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Asset][NodeColorType_Border] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
			nodeColors[NodeType_Asset][NodeColorType_HeaderBg] = ImVec4(0.7f, 0.7f, 0.7f, 0.25f);
			nodeColors[NodeType_Asset][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Asset][NodeColorType_HeaderHovered] = ImVec4(0.7f, 0.7f, 0.7f, 0.5f);

			nodeColors[NodeType_Data][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Data][NodeColorType_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);
			nodeColors[NodeType_Data][NodeColorType_HeaderBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.25f);
			nodeColors[NodeType_Data][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Data][NodeColorType_HeaderHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);

			nodeColors[NodeType_Maths][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Maths][NodeColorType_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);
			nodeColors[NodeType_Maths][NodeColorType_HeaderBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.25f);
			nodeColors[NodeType_Maths][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Maths][NodeColorType_HeaderHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
			
			nodeColors[NodeType_ArithmeticOperation][NodeColorType_Bg] = nodeColors[NodeType_Maths][NodeColorType_Bg];
			nodeColors[NodeType_ArithmeticOperation][NodeColorType_Border] = nodeColors[NodeType_Maths][NodeColorType_Border];
			nodeColors[NodeType_ArithmeticOperation][NodeColorType_HeaderBg] = nodeColors[NodeType_Maths][NodeColorType_HeaderBg];
			nodeColors[NodeType_ArithmeticOperation][NodeColorType_HeaderActivated] = nodeColors[NodeType_Maths][NodeColorType_HeaderActivated];
			nodeColors[NodeType_ArithmeticOperation][NodeColorType_HeaderHovered] = nodeColors[NodeType_Maths][NodeColorType_HeaderHovered];
			
			nodeColors[NodeType_LogicOperation][NodeColorType_Bg] = nodeColors[NodeType_Maths][NodeColorType_Bg];
			nodeColors[NodeType_LogicOperation][NodeColorType_Border] = nodeColors[NodeType_Maths][NodeColorType_Border];
			nodeColors[NodeType_LogicOperation][NodeColorType_HeaderBg] = nodeColors[NodeType_Maths][NodeColorType_HeaderBg];
			nodeColors[NodeType_LogicOperation][NodeColorType_HeaderActivated] = nodeColors[NodeType_Maths][NodeColorType_HeaderActivated];
			nodeColors[NodeType_LogicOperation][NodeColorType_HeaderHovered] = nodeColors[NodeType_Maths][NodeColorType_HeaderHovered];
			
			nodeColors[NodeType_Equation][NodeColorType_Bg] = nodeColors[NodeType_Maths][NodeColorType_Bg];
			nodeColors[NodeType_Equation][NodeColorType_Border] = nodeColors[NodeType_Maths][NodeColorType_Border];
			nodeColors[NodeType_Equation][NodeColorType_HeaderBg] = nodeColors[NodeType_Maths][NodeColorType_HeaderBg];
			nodeColors[NodeType_Equation][NodeColorType_HeaderActivated] = nodeColors[NodeType_Maths][NodeColorType_HeaderActivated];
			nodeColors[NodeType_Equation][NodeColorType_HeaderHovered] = nodeColors[NodeType_Maths][NodeColorType_HeaderHovered];

			nodeColors[NodeType_Event][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Event][NodeColorType_Border] = ImVec4(0.591f, 0.257f, 0.9f, 1.f);
			nodeColors[NodeType_Event][NodeColorType_HeaderBg] = ImVec4(0.591f, 0.257f, 0.9f, 0.25f);
			nodeColors[NodeType_Event][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Event][NodeColorType_HeaderHovered] = ImVec4(0.591f, 0.257f, 0.9f, 0.5f);

			nodeColors[NodeType_Parameter][NodeColorType_Bg] = nodeColors[NodeType_Asset][NodeColorType_Bg];
			nodeColors[NodeType_Parameter][NodeColorType_Border] = nodeColors[NodeType_Asset][NodeColorType_Border];
			nodeColors[NodeType_Parameter][NodeColorType_HeaderBg] = nodeColors[NodeType_Asset][NodeColorType_HeaderBg];
			nodeColors[NodeType_Parameter][NodeColorType_HeaderActivated] = nodeColors[NodeType_Asset][NodeColorType_HeaderActivated];
			nodeColors[NodeType_Parameter][NodeColorType_HeaderHovered] = nodeColors[NodeType_Asset][NodeColorType_HeaderHovered];

			nodeColors[NodeType_Reaction][NodeColorType_Bg] = nodeColors[NodeType_Asset][NodeColorType_Bg];
			nodeColors[NodeType_Reaction][NodeColorType_Border] = nodeColors[NodeType_Asset][NodeColorType_Border];
			nodeColors[NodeType_Reaction][NodeColorType_HeaderBg] = nodeColors[NodeType_Asset][NodeColorType_HeaderBg];
			nodeColors[NodeType_Reaction][NodeColorType_HeaderActivated] = nodeColors[NodeType_Asset][NodeColorType_HeaderActivated];
			nodeColors[NodeType_Reaction][NodeColorType_HeaderHovered] = nodeColors[NodeType_Asset][NodeColorType_HeaderHovered];

			nodeColors[NodeType_Solver][NodeColorType_Bg] = nodeColors[NodeType_Asset][NodeColorType_Bg];
			nodeColors[NodeType_Solver][NodeColorType_Border] = nodeColors[NodeType_Asset][NodeColorType_Border];
			nodeColors[NodeType_Solver][NodeColorType_HeaderBg] = nodeColors[NodeType_Asset][NodeColorType_HeaderBg];
			nodeColors[NodeType_Solver][NodeColorType_HeaderActivated] = nodeColors[NodeType_Asset][NodeColorType_HeaderActivated];
			nodeColors[NodeType_Solver][NodeColorType_HeaderHovered] = nodeColors[NodeType_Asset][NodeColorType_HeaderHovered];

			nodeColors[NodeType_Species][NodeColorType_Bg] = nodeColors[NodeType_Asset][NodeColorType_Bg];
			nodeColors[NodeType_Species][NodeColorType_Border] = nodeColors[NodeType_Asset][NodeColorType_Border];
			nodeColors[NodeType_Species][NodeColorType_HeaderBg] = nodeColors[NodeType_Asset][NodeColorType_HeaderBg];
			nodeColors[NodeType_Species][NodeColorType_HeaderActivated] = nodeColors[NodeType_Asset][NodeColorType_HeaderActivated];
			nodeColors[NodeType_Species][NodeColorType_HeaderHovered] = nodeColors[NodeType_Asset][NodeColorType_HeaderHovered];

			pinColors[PinType_Default][PinColorType_BgActivated] = nodeColors[NodeType_Default][NodeColorType_Border];
			pinColors[PinType_Default][PinColorType_BgInactivated] = nodeColors[NodeType_Default][NodeColorType_Bg];
			pinColors[PinType_Default][PinColorType_Border] = nodeColors[NodeType_Default][NodeColorType_Border];

			pinColors[PinType_Asset][PinColorType_BgActivated] = nodeColors[NodeType_Asset][NodeColorType_Border];
			pinColors[PinType_Asset][PinColorType_BgInactivated] = nodeColors[NodeType_Asset][NodeColorType_Bg];
			pinColors[PinType_Asset][PinColorType_Border] = nodeColors[NodeType_Asset][NodeColorType_Border];
			
			pinColors[PinType_DataStateValueFloat][PinColorType_BgActivated] = nodeColors[NodeType_Data][NodeColorType_Border];
			pinColors[PinType_DataStateValueFloat][PinColorType_BgInactivated] = nodeColors[NodeType_Data][NodeColorType_Bg];
			pinColors[PinType_DataStateValueFloat][PinColorType_Border] = nodeColors[NodeType_Data][NodeColorType_Border];

			pinColors[PinType_Equation][PinColorType_BgActivated] = nodeColors[NodeType_Equation][NodeColorType_Border];
			pinColors[PinType_Equation][PinColorType_BgInactivated] = nodeColors[NodeType_Equation][NodeColorType_Bg];
			pinColors[PinType_Equation][PinColorType_Border] = nodeColors[NodeType_Equation][NodeColorType_Border];
			
			pinColors[PinType_EquationValueFloat][PinColorType_BgActivated] = nodeColors[NodeType_Data][NodeColorType_Border];
			pinColors[PinType_EquationValueFloat][PinColorType_BgInactivated] = nodeColors[NodeType_Data][NodeColorType_Bg];
			pinColors[PinType_EquationValueFloat][PinColorType_Border] = nodeColors[NodeType_Data][NodeColorType_Border];
			
			pinColors[PinType_FreeValueFloat][PinColorType_BgActivated] = nodeColors[NodeType_Data][NodeColorType_Border];
			pinColors[PinType_FreeValueFloat][PinColorType_BgInactivated] = nodeColors[NodeType_Data][NodeColorType_Bg];
			pinColors[PinType_FreeValueFloat][PinColorType_Border] = nodeColors[NodeType_Data][NodeColorType_Border];
			
			pinColors[PinType_ValueBool][PinColorType_BgActivated] = nodeColors[NodeType_Data][NodeColorType_Border];
			pinColors[PinType_ValueBool][PinColorType_BgInactivated] = nodeColors[NodeType_Data][NodeColorType_Bg];
			pinColors[PinType_ValueBool][PinColorType_Border] = nodeColors[NodeType_Data][NodeColorType_Border];
			
			pinColors[PinType_ModifyDataStateEvent][PinColorType_BgActivated] = nodeColors[NodeType_Event][NodeColorType_Border];
			pinColors[PinType_ModifyDataStateEvent][PinColorType_BgInactivated] = nodeColors[NodeType_Event][NodeColorType_Bg];
			pinColors[PinType_ModifyDataStateEvent][PinColorType_Border] = nodeColors[NodeType_Event][NodeColorType_Border];

			pinColors[PinType_Operand][PinColorType_BgActivated] = nodeColors[NodeType_Asset][NodeColorType_Border];
			pinColors[PinType_Operand][PinColorType_BgInactivated] = nodeColors[NodeType_Asset][NodeColorType_Bg];
			pinColors[PinType_Operand][PinColorType_Border] = nodeColors[NodeType_Asset][NodeColorType_Border];

			pinColors[PinType_Parameter][PinColorType_BgActivated] = nodeColors[NodeType_Parameter][NodeColorType_Border];
			pinColors[PinType_Parameter][PinColorType_BgInactivated] = nodeColors[NodeType_Parameter][NodeColorType_Bg];
			pinColors[PinType_Parameter][PinColorType_Border] = nodeColors[NodeType_Parameter][NodeColorType_Border];

			pinColors[PinType_Reaction][PinColorType_BgActivated] = nodeColors[NodeType_Reaction][NodeColorType_Border];
			pinColors[PinType_Reaction][PinColorType_BgInactivated] = nodeColors[NodeType_Reaction][NodeColorType_Bg];
			pinColors[PinType_Reaction][PinColorType_Border] = nodeColors[NodeType_Reaction][NodeColorType_Border];

			pinColors[PinType_Solver][PinColorType_BgActivated] = nodeColors[NodeType_Solver][NodeColorType_Border];
			pinColors[PinType_Solver][PinColorType_BgInactivated] = nodeColors[NodeType_Solver][NodeColorType_Bg];
			pinColors[PinType_Solver][PinColorType_Border] = nodeColors[NodeType_Solver][NodeColorType_Border];

			pinColors[PinType_Species][PinColorType_BgActivated] = nodeColors[NodeType_Species][NodeColorType_Border];
			pinColors[PinType_Species][PinColorType_BgInactivated] = nodeColors[NodeType_Species][NodeColorType_Bg];
			pinColors[PinType_Species][PinColorType_Border] = nodeColors[NodeType_Species][NodeColorType_Border];
			
			pinColors[PinType_Trigger][PinColorType_BgActivated] = nodeColors[NodeType_Event][NodeColorType_Border];
			pinColors[PinType_Trigger][PinColorType_BgInactivated] = nodeColors[NodeType_Event][NodeColorType_Bg];
			pinColors[PinType_Trigger][PinColorType_Border] = nodeColors[NodeType_Event][NodeColorType_Border];
		}
	};
}