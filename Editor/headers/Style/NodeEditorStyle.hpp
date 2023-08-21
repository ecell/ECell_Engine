#pragma once
#include <imgui.h>
#include "Utility/MNBV/MNBVTypes.hpp"

/*!
@file NodeEditorStyle.hpp
@brief Enums and data to encode the style of each ECellEngine::Editor::Utility::ModelNodeBasedViewerContext.
*/

namespace ECellEngine::Editor::Style
{
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
		@details To replace the x and y values of ax::NodeEditor::PinSize.
		*/
		float pinWidth;

		/*!
		@brief The rounding of the node corners.
		@details To replace the value of ax::NodeEditor::NodeRounding.
		*/
		float nodeRounding;

		/*!
		@brief The width of the node border.
		@details To replace the value of ax::NodeEditor::NodeBorderWidth.
		*/
		float nodeBorderWidth;

		/*!
		@brief The width of the node border when hovered.
		@details To replace the value of ax::NodeEditor::HoveredNodeBorderWidth.
		*/
		float hoveredNodeBorderWidth;

		/*!
		@brief The width of the node border when selected.
		@details To replace the value of ax::NodeEditor::SelectedNodeBorderWidth.
		*/
		float selectedNodeBorderWidth;

		/*!
		@brief The width of the pin border.
		@details To replace the value of ax::NodeEditor::PinBorderWidth.
		*/
		float pinBorderWidth;

		/*!
		@brief The rounding of the pin corners.
		@details To replace the value of ax::NodeEditor::PinRounding.
		*/
		float pinRounding;

		/*!
		@brief The matrix to store all color sets for all node types.
		*/
		ImVec4 nodeColors[Utility::MNBV::NodeType_Count][NodeColorType_Count];

		/*!
		@brief The matrix to store all color sets for all pin types.
		*/
		ImVec4 pinColors[Utility::MNBV::PinType_Count][PinColorType_Count];

		NodeEditorStyle()
		{

			nodeCenterAreaMinWidth = 200.f;
			nodeRounding = 6.f; //instead of 12 by default.
			nodeBorderWidth = 3.f; //instead of 2 by default.
			hoveredNodeBorderWidth = 6.f; //instead of 3 by default.
			selectedNodeBorderWidth = 6.f; //instead of 3 by default.
			pinBorderWidth = 3.f; //instead of 0 by default.
			pinRounding = 0.f; //instead of 4 by default.
			pinWidth = 8.f;

#pragma region nodeColors
			nodeColors[Utility::MNBV::NodeType_Default][NodeColorType_Bg] = ImVec4(0.125f, 0.125f, 0.125f, 0.784f);//ax::NodeEditor::StyleColor_NodeBg;
			nodeColors[Utility::MNBV::NodeType_Default][NodeColorType_Border] = ImVec4(1.000f, 1.000f, 1.000f, 0.376f);//ax::NodeEditor::StyleColor_NodeBorder;
			nodeColors[Utility::MNBV::NodeType_Default][NodeColorType_HeaderBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.376f);//ax::NodeEditor::StyleColor_NodeBorder;
			nodeColors[Utility::MNBV::NodeType_Default][NodeColorType_HeaderActivated] = ImVec4(1.000f, 0.690f, 0.196f, 1.000f);//ax::NodeEditor::StyleColor_SelNodeBorder;
			nodeColors[Utility::MNBV::NodeType_Default][NodeColorType_HeaderHovered] = ImVec4(0.196f, 0.690f, 1.000f, 1.000f);//ax::NodeEditor::StyleColor_HovNodeBorder;

			nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Border] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
			nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderBg] = ImVec4(0.7f, 0.7f, 0.7f, 0.25f);
			nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderHovered] = ImVec4(0.7f, 0.7f, 0.7f, 0.5f);

			nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);
			nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_HeaderBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.25f);
			nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_HeaderHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);

			nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);
			nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_HeaderBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.25f);
			nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_HeaderHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);

			nodeColors[Utility::MNBV::NodeType_ArithmeticOperation][NodeColorType_Bg] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_Bg];
			nodeColors[Utility::MNBV::NodeType_ArithmeticOperation][NodeColorType_Border] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_Border];
			nodeColors[Utility::MNBV::NodeType_ArithmeticOperation][NodeColorType_HeaderBg] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_HeaderBg];
			nodeColors[Utility::MNBV::NodeType_ArithmeticOperation][NodeColorType_HeaderActivated] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_HeaderActivated];
			nodeColors[Utility::MNBV::NodeType_ArithmeticOperation][NodeColorType_HeaderHovered] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_HeaderHovered];

			nodeColors[Utility::MNBV::NodeType_LogicOperation][NodeColorType_Bg] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_Bg];
			nodeColors[Utility::MNBV::NodeType_LogicOperation][NodeColorType_Border] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_Border];
			nodeColors[Utility::MNBV::NodeType_LogicOperation][NodeColorType_HeaderBg] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_HeaderBg];
			nodeColors[Utility::MNBV::NodeType_LogicOperation][NodeColorType_HeaderActivated] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_HeaderActivated];
			nodeColors[Utility::MNBV::NodeType_LogicOperation][NodeColorType_HeaderHovered] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_HeaderHovered];

			nodeColors[Utility::MNBV::NodeType_Equation][NodeColorType_Bg] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_Bg];
			nodeColors[Utility::MNBV::NodeType_Equation][NodeColorType_Border] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_Border];
			nodeColors[Utility::MNBV::NodeType_Equation][NodeColorType_HeaderBg] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_HeaderBg];
			nodeColors[Utility::MNBV::NodeType_Equation][NodeColorType_HeaderActivated] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_HeaderActivated];
			nodeColors[Utility::MNBV::NodeType_Equation][NodeColorType_HeaderHovered] = nodeColors[Utility::MNBV::NodeType_Maths][NodeColorType_HeaderHovered];

			nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Border] = ImVec4(0.591f, 0.257f, 0.9f, 1.f);
			nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_HeaderBg] = ImVec4(0.591f, 0.257f, 0.9f, 0.25f);
			nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_HeaderHovered] = ImVec4(0.591f, 0.257f, 0.9f, 0.5f);

			nodeColors[Utility::MNBV::NodeType_Parameter][NodeColorType_Bg] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Bg];
			nodeColors[Utility::MNBV::NodeType_Parameter][NodeColorType_Border] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Border];
			nodeColors[Utility::MNBV::NodeType_Parameter][NodeColorType_HeaderBg] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderBg];
			nodeColors[Utility::MNBV::NodeType_Parameter][NodeColorType_HeaderActivated] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderActivated];
			nodeColors[Utility::MNBV::NodeType_Parameter][NodeColorType_HeaderHovered] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderHovered];

			nodeColors[Utility::MNBV::NodeType_Reaction][NodeColorType_Bg] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Bg];
			nodeColors[Utility::MNBV::NodeType_Reaction][NodeColorType_Border] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Border];
			nodeColors[Utility::MNBV::NodeType_Reaction][NodeColorType_HeaderBg] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderBg];
			nodeColors[Utility::MNBV::NodeType_Reaction][NodeColorType_HeaderActivated] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderActivated];
			nodeColors[Utility::MNBV::NodeType_Reaction][NodeColorType_HeaderHovered] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderHovered];

			nodeColors[Utility::MNBV::NodeType_Solver][NodeColorType_Bg] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Bg];
			nodeColors[Utility::MNBV::NodeType_Solver][NodeColorType_Border] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Border];
			nodeColors[Utility::MNBV::NodeType_Solver][NodeColorType_HeaderBg] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderBg];
			nodeColors[Utility::MNBV::NodeType_Solver][NodeColorType_HeaderActivated] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderActivated];
			nodeColors[Utility::MNBV::NodeType_Solver][NodeColorType_HeaderHovered] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderHovered];

			nodeColors[Utility::MNBV::NodeType_Species][NodeColorType_Bg] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Bg];
			nodeColors[Utility::MNBV::NodeType_Species][NodeColorType_Border] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Border];
			nodeColors[Utility::MNBV::NodeType_Species][NodeColorType_HeaderBg] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderBg];
			nodeColors[Utility::MNBV::NodeType_Species][NodeColorType_HeaderActivated] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderActivated];
			nodeColors[Utility::MNBV::NodeType_Species][NodeColorType_HeaderHovered] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_HeaderHovered];
#pragma endregion

#pragma region Pin Colors
			pinColors[Utility::MNBV::PinType_Default][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Default][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_Default][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Default][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_Default][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Default][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_Asset][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_Asset][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_Asset][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_DataStateValueFloat][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_DataStateValueFloat][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_DataStateValueFloat][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_Equation][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Equation][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_Equation][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Equation][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_Equation][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Equation][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_EquationValueFloat][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_EquationValueFloat][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_EquationValueFloat][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_FreeValueFloat][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_FreeValueFloat][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_FreeValueFloat][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Data][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_BooleanCallBackPublisher][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_BooleanCallBackPublisher][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_BooleanCallBackPublisher][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_BooleanCallBackSubscriber][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_BooleanCallBackSubscriber][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_BooleanCallBackSubscriber][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_FloatCallBackPublisher][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_FloatCallBackPublisher][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_FloatCallBackPublisher][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_FloatCallBackSubscriber][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_FloatCallBackSubscriber][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_FloatCallBackSubscriber][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Event][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_Operand][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_Operand][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_Operand][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Asset][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_Parameter][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Parameter][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_Parameter][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Parameter][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_Parameter][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Parameter][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_Reaction][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Reaction][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_Reaction][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Reaction][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_Reaction][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Reaction][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_Solver][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Solver][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_Solver][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Solver][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_Solver][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Solver][NodeColorType_Border];

			pinColors[Utility::MNBV::PinType_Species][PinColorType_BgActivated] = nodeColors[Utility::MNBV::NodeType_Species][NodeColorType_Border];
			pinColors[Utility::MNBV::PinType_Species][PinColorType_BgInactivated] = nodeColors[Utility::MNBV::NodeType_Species][NodeColorType_Bg];
			pinColors[Utility::MNBV::PinType_Species][PinColorType_Border] = nodeColors[Utility::MNBV::NodeType_Species][NodeColorType_Border];
#pragma endregion
		}

		/*!
		@brief Pops the number of color styles pushed with ::PushNodeStyle(const ImVec4 _colorSet[])
		*/
		static void PopNodeStyle();

		/*!
		@brief Pushes a bunch of colors to customize a node' style.
		@details Don't forget to call ::PopNodeStyle() after you've drawn the
				 node.
		@see ::PopNodeStyle()
		*/
		static void PushNodeStyle(const ImVec4 _colorSet[]);
	};
}