#include "Style/EditorStyle.hpp"

ECellEngine::Editor::Style::LogStyle ECellEngine::Editor::Style::EditorStyle::logStyle;

ECellEngine::Editor::Style::NodeEditorStyle ECellEngine::Editor::Style::EditorStyle::mnbvStyle;

ECellEngine::Editor::Style::LogStyle& ECellEngine::Editor::Style::EditorStyle::GetLogStyle() noexcept
{
	return logStyle;
}

ECellEngine::Editor::Style::NodeEditorStyle& ECellEngine::Editor::Style::EditorStyle::GetMNBVStyle() noexcept
{
	return mnbvStyle;
}

ImVec4* ECellEngine::Editor::Style::EditorStyle::GetNodeColors(Utility::MNBV::NodeType _nodeType) noexcept
{
	return mnbvStyle.nodeColors[_nodeType];
}

ImVec4* ECellEngine::Editor::Style::EditorStyle::GetPinColors(Utility::MNBV::PinType _pinType) noexcept
{
	return mnbvStyle.pinColors[_pinType];
}

char* ECellEngine::Editor::Style::EditorStyle::GetNodeColorTypeName(NodeColorType _nodeColorType) noexcept
{
	switch (_nodeColorType)
	{
	case NodeColorType_Bg:
		return "NodeColorType_Bg";
	case NodeColorType_Border:
		return "NodeColorType_Border";
	case NodeColorType_HeaderBg:
		return "NodeColorType_HeaderBg";
	case NodeColorType_HeaderActivated:
		return "NodeColorType_HeaderActivated";
	case NodeColorType_HeaderHovered:
		return "NodeColorType_HeaderHovered";
	default:
		return "Unknown NodeColorType";
	}
}

char* ECellEngine::Editor::Style::EditorStyle::GetNodeTypeName(Utility::MNBV::NodeType _nodeType) noexcept
{
	switch (_nodeType)
	{
	case Utility::MNBV::NodeType_Default:
		return "NodeType_Default";
	case Utility::MNBV::NodeType_Asset:
		return "NodeType_Asset";
	case Utility::MNBV::NodeType_Data:
		return "NodeType_Data";
	case Utility::MNBV::NodeType_Equation:
		return "NodeType_Equation";
	case Utility::MNBV::NodeType_Event:
		return "NodeType_Event";
	case Utility::MNBV::NodeType_Maths:
		return "NodeType_Maths";
	case Utility::MNBV::NodeType_ArithmeticOperation:
		return "NodeType_ArithmeticOperation";
	case Utility::MNBV::NodeType_LogicOperation:
		return "NodeType_LogicOperation";
	case Utility::MNBV::NodeType_Parameter:
		return "NodeType_Parameter";
	case Utility::MNBV::NodeType_Reaction:
		return "NodeType_Reaction";
	case Utility::MNBV::NodeType_Solver:
		return "NodeType_Solver";
	case Utility::MNBV::NodeType_Species:
		return "NodeType_Species";
	default:
		return "Unknown NodeType";
	}
}

char* ECellEngine::Editor::Style::EditorStyle::GetPinColorTypeName(PinColorType _pinColorType) noexcept
{
	switch (_pinColorType)
	{
	case PinColorType_BgActivated:
		return "PinColorType_BgActivated";
	case PinColorType_BgInactivated:
		return "PinColorType_BgInactivated";
	case PinColorType_Border:
		return "PinColorType_Border";
	default:
		return "Unknown PinColorType";
	}
}

char* ECellEngine::Editor::Style::EditorStyle::GetPinTypeName(Utility::MNBV::PinType _pinType) noexcept
{
	switch (_pinType)
	{
	case Utility::MNBV::PinType_Default:
		return "PinType_Default";
	case Utility::MNBV::PinType_Asset:
		return "PinType_Asset";
	case Utility::MNBV::PinType_DataStateValueFloat:
		return "PinType_DataStateValueFloat";
	case Utility::MNBV::PinType_Equation:
		return "PinType_Equation";
	case Utility::MNBV::PinType_EquationValueFloat:
		return "PinType_EquationValueFloat";
	case Utility::MNBV::PinType_FreeValueFloat:
		return "PinType_FreeValueFloat";
	case Utility::MNBV::PinType_BooleanCallBackPublisher:
		return "PinType_BooleanCallBackPublisher";
	case Utility::MNBV::PinType_BooleanCallBackSubscriber:
		return "PinType_BooleanCallBackSubscriber";
	case Utility::MNBV::PinType_FloatCallBackPublisher:
		return "PinType_FloatCallBackPublisher";
	case Utility::MNBV::PinType_FloatCallBackSubscriber:
		return "PinType_FloatCallBackSubscriber";
	case Utility::MNBV::PinType_Operand:
		return "PinType_Operand";
	case Utility::MNBV::PinType_Parameter:
		return "PinType_Parameter";
	case Utility::MNBV::PinType_Reaction:
		return "PinType_Reaction";
	case Utility::MNBV::PinType_Solver:
		return "PinType_Solver";
	case Utility::MNBV::PinType_Species:
		return "PinType_Species";
	default:
		return "Unknown PinType";
	}
}

void ECellEngine::Editor::Style::EditorStyle::PopScrollBarStyle()
{
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(2);
}

void ECellEngine::Editor::Style::EditorStyle::PushScrollBarStyle(const ImGuiStyle& _style)
{
	//Style the vertical slider like a scroll bar
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, _style.ScrollbarRounding);
	ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, _style.ScrollbarRounding);
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarGrab));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarBg));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarBg));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarBg));
}