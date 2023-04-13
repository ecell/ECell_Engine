#pragma once

namespace ECellEngine::Editor::Utility
{
	enum NodeType
	{
		NodeType_Default,

		NodeType_Asset,
		NodeType_Parameter,
		NodeType_Reaction,
		NodeType_Solver,
		NodeType_Species,

		NodeType_Count
	};

	enum PinType
	{
		PinType_Default,

		PinType_Asset,
		PinType_Parameter,
		PinType_Reaction,
		PinType_Solver,
		PinType_Species,

		PinType_Count
	};

	enum NodeColorType
	{
		NodeColorType_Bg,
		NodeColorType_Border,
		//NodeColorType_BorderHovered,
		//NodeColorType_BorderSelected,
		NodeColorType_HeaderBg,
		NodeColorType_HeaderActivated,
		NodeColorType_HeaderHovered,

		NodeColorType_Count
	};

	enum PinColorType
	{
		PinColorType_BgActivated,
		PinColorType_BgInactivated,
		PinColorType_Border,

		PinColorType_Count
	};

	struct NodeEditorStyle
	{
		float nodeCenterAreaMinWidth;
		float pinWidth;

		ImVec4 nodeColors[NodeType_Count][NodeColorType_Count];
		ImVec4 pinColors[PinType_Count][PinColorType_Count];

		NodeEditorStyle()
		{
			pinWidth = 8.f;
			nodeCenterAreaMinWidth = 200.f;

			nodeColors[NodeType_Default][NodeColorType_Bg] = ImVec4(0.125f, 0.125f, 0.125f, 0.784f);//ax::NodeEditor::StyleColor_NodeBg;
			nodeColors[NodeType_Default][NodeColorType_Border] = ImVec4(1.000f, 1.000f, 1.000f, 0.376f);//ax::NodeEditor::StyleColor_NodeBorder;
			//nodeColors[NodeType_Default][NodeColorType_BorderHovered] = ImVec4(0.196f, 0.690f, 1.000f, 1.000f);//ax::NodeEditor::StyleColor_HovNodeBorder;
			//nodeColors[NodeType_Default][NodeColorType_BorderSelected] = ImVec4(1.000f, 0.690f, 0.196f, 1.000f);//ax::NodeEditor::StyleColor_SelNodeBorder;
			nodeColors[NodeType_Default][NodeColorType_HeaderBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.376f);//ax::NodeEditor::StyleColor_NodeBorder;
			nodeColors[NodeType_Default][NodeColorType_HeaderActivated] = ImVec4(1.000f, 0.690f, 0.196f, 1.000f);//ax::NodeEditor::StyleColor_SelNodeBorder;
			nodeColors[NodeType_Default][NodeColorType_HeaderHovered] = ImVec4(0.196f, 0.690f, 1.000f, 1.000f);//ax::NodeEditor::StyleColor_HovNodeBorder;

			nodeColors[NodeType_Asset][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Asset][NodeColorType_Border] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
			//nodeColors[NodeType_Asset][NodeColorType_BorderHovered] =		ImVec4(0.f, 1.f, 0.f, 1.f);
			//nodeColors[NodeType_Asset][NodeColorType_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			nodeColors[NodeType_Asset][NodeColorType_HeaderBg] = ImVec4(0.7f, 0.7f, 0.7f, 0.25f);
			nodeColors[NodeType_Asset][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Asset][NodeColorType_HeaderHovered] = ImVec4(0.7f, 0.7f, 0.7f, 0.5f);

			nodeColors[NodeType_Parameter][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Parameter][NodeColorType_Border] = ImVec4(0.05f, 0.463f, 0.297f, 1.f);
			//nodeColors[NodeType_Parameter][NodeColorType_BorderHovered] =	ImVec4(0.f, 1.f, 0.f, 1.f);
			//nodeColors[NodeType_Parameter][NodeColorType_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			nodeColors[NodeType_Parameter][NodeColorType_HeaderBg] = ImVec4(0.05f, 0.463f, 0.297f, 0.25f);;
			nodeColors[NodeType_Parameter][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);;
			nodeColors[NodeType_Parameter][NodeColorType_HeaderHovered] = ImVec4(0.05f, 0.463f, 0.297f, 0.5f);;

			nodeColors[NodeType_Reaction][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Reaction][NodeColorType_Border] = ImVec4(0.141f, 0.391f, 0.485f, 1.f);
			//nodeColors[NodeType_Reaction][NodeColorType_BorderHovered] =	ImVec4(0.f, 1.f, 0.f, 1.f);
			//nodeColors[NodeType_Reaction][NodeColorType_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			nodeColors[NodeType_Reaction][NodeColorType_HeaderBg] = ImVec4(0.141f, 0.391f, 0.485f, 1.f);
			nodeColors[NodeType_Reaction][NodeColorType_HeaderActivated] = ImVec4(1.f, 0.f, 0.f, 1.f);
			nodeColors[NodeType_Reaction][NodeColorType_HeaderHovered] = ImVec4(0.141f, 0.391f, 0.485f, 1.f);

			nodeColors[NodeType_Solver][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Solver][NodeColorType_Border] = ImVec4(1.f, 0.365f, 0.f, 1.f);
			//nodeColors[NodeType_Solver][NodeColorType_BorderHovered] =	ImVec4(0.f, 1.f, 0.f, 1.f);
			//nodeColors[NodeType_Solver][NodeColorType_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			nodeColors[NodeType_Solver][NodeColorType_HeaderBg] = ImVec4(1.f, 0.365f, 0.f, 0.25f);
			nodeColors[NodeType_Solver][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Solver][NodeColorType_HeaderHovered] = ImVec4(1.f, 0.365f, 0.f, 0.5f);

			nodeColors[NodeType_Species][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Species][NodeColorType_Border] = ImVec4(1.f, 0.f, 0.f, 1.f);
			//nodeColors[NodeType_Species][NodeColorType_BorderHovered] =	ImVec4(0.f, 1.f, 0.f, 1.f);.f);
			//nodeColors[NodeType_Species][NodeColorType_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			nodeColors[NodeType_Species][NodeColorType_HeaderBg] = ImVec4(1.f, 0.f, 0.f, 0.25f);
			nodeColors[NodeType_Species][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Species][NodeColorType_HeaderHovered] = ImVec4(1.f, 0.f, 0.f, 0.5f);

			pinColors[PinType_Default][PinColorType_BgActivated] = nodeColors[NodeType_Default][NodeColorType_Border];
			pinColors[PinType_Default][PinColorType_BgInactivated] = nodeColors[NodeType_Default][NodeColorType_Bg];
			pinColors[PinType_Default][PinColorType_Border] = nodeColors[NodeType_Default][NodeColorType_Border];

			pinColors[PinType_Asset][PinColorType_BgActivated] = nodeColors[NodeType_Asset][NodeColorType_Border];
			pinColors[PinType_Asset][PinColorType_BgInactivated] = nodeColors[NodeType_Asset][NodeColorType_Bg];
			pinColors[PinType_Asset][PinColorType_Border] = nodeColors[NodeType_Asset][NodeColorType_Border];

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

		}
	};
}