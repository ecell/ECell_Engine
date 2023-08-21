#include "LogStyle.hpp"
#include "NodeEditorStyle.hpp"

/*!
@file EditorStyle.hpp
@brief Header file for the EditorStyle struct.
@details API to manipulate the style of the editor.
*/

namespace ECellEngine::Editor::Style
{
	/*!
	@brief Struct to set and retrieve style variables for the editor.
	@details The style variables are then fed to ImGui::Style or 
			 ax::NodeEditor::Style.
	*/
	struct EditorStyle
	{
		/*!
		@brief Styling of the log messages.
		*/
		static LogStyle logStyle;

		/*!
		@brief Styling of the node editor.
		*/
		static NodeEditorStyle mnbvStyle;

		/*!
		@brief Gets the ::logStyle.
		*/
		static LogStyle& GetLogStyle() noexcept;

		/*!
		@brief Gets the ::mnbvStyle.
		*/
		static NodeEditorStyle& GetMNBVStyle() noexcept;

		/*!
		@brief Gets the pointer to the array of colors used to cutomize the aspect of a
				node of type @p _nodeType.
		@param _nodeType The type of node of which we want to retrieve the color set.
		@remarks A reminder that in c++ the pointer to the first element of an array
				 is the pointer to the array itself.
		@see ECellEngine::Editor::Style::NodeEditorStyle::nodeColors
		*/
		static ImVec4* GetNodeColors(Utility::MNBV::NodeType _nodeType) noexcept;

		/*!
		@brief Translates a value of the enum ECellEngine::Editor::Style::NodeColorType
				to a character string.
		@param _nodeColorType The type of node color for which we want to retrieve
				the name.
		*/
		static char* GetNodeColorTypeName(NodeColorType _nodeColorType) noexcept;

		/*!
		@brief Translates a value of the enum ECellEngine::Editor::Utility::MNBV::NodeType
				to a character string.
		@param _nodeType The type of node for which we want to retrieve the name.
		*/
		static char* GetNodeTypeName(Utility::MNBV::NodeType _nodeType) noexcept;

		/*!
		@brief Gets the pointer to the array of colors used to cutomize the aspect of a
				pin of type @p _pinType.
		@param _pinType The type of pin for which we want to retrieve the color set.
		@remarks A reminder that in c++ the pointer to the first element of an array
				 is the pointer to the array itself.
		@see ECellEngine::Editor::Style::NodeEditorStyle::pinColors
		*/
		static ImVec4* GetPinColors(Utility::MNBV::PinType _pinType) noexcept;

		/*!
		@brief Translates a value of the enum ECellEngine::Editor::Style::PinColorType
				to a character string.
		@param _pinColorType The type of node pin color for which we want to retrieve
				the name.
		*/
		static char* GetPinColorTypeName(PinColorType _pinColorType) noexcept;

		/*!
		@brief Translates a value of the enum ECellEngine::Editor::Utility::MNBV::PinType
				to a character string.
		@param _pinType The type of pin for which we want to retrieve the name.
		*/
		static char* GetPinTypeName(Utility::MNBV::PinType _pinType) noexcept;

		/*!
		@brief Pops the number of styles pushed with ::PushScrollBarStyle(ImGuiStyle& _style)
		*/
		static void PopScrollBarStyle();

		/*!
		@brief Pushes a bunch of styles to match ImGui's default Scroll Bar style.
				Can be used with sliders in nodes.
		@details Don't forget to call ::PopScrollBarStyle() after you've drawn the
				 slider.
		@see ::PopScrollBarStyle()
		*/
		static void PushScrollBarStyle(const ImGuiStyle& _style);
	};
}