#pragma once

/*!
@file DragAndDrop.hpp
@brief Static utility class for drag and drop operations.
@details ImGui also defines a drag and drop API, but we couldn't find a way to
		 make "defer" drag and drop with it. For instance, there are restriction
		 on setting the payload inside a Begin/End block. On the other hand, the
		 ImGui API encapsulates some nice additional features like the tooltip
		 eased setup or the target highlight which we will not implement here.
*/

#include "imgui.h"

#include "Util/StrUtil.hpp"

namespace ECellEngine::Editor::Utility
{
	/*!
	@brief Very simple drag and drop utility class.
	@details Only encapsulate static variables to store the payload ID and data.
	*/
	struct DragAndDrop
	{
		/*!
		@brief The payload ID.
		@details Use this ID to compare with the targets. If there is a match, we expect
				 the payload data to be usable there
		*/
		static char payloadID[32];

		/*!
		@brief A pointer to the payload data.
		@details It is up to the user to make the appropriate cast to use the data.
		*/
		static const void* payloadData;

		/*!
		@brief A flag to check whether the payload was set.
		@see ::SetPayload(const char* _id, const void* _data)
		*/
		static bool payloadCharged;

		/*!
		@brief Get the casted payload data.
		@details The user must make sure the cast is valid.
		@tparam OutType The type to cast the payload data to.
		@returns A pointer to the casted payload data.
		*/
		template<typename OutType>
		inline static OutType* GetPayload() noexcept
		{
			return (OutType*)payloadData;
		}

		/*!
		@brief Set the payload ID and data.
		*/
		inline static void SetPayload(const char* _id, const void* _data) noexcept
		{
			ECellEngine::Util::StrCopy(payloadID, _id, 32);
			payloadData = _data;
			payloadCharged = true;
		}

		/*!
		@brief Checks if the a target ID matches the payload ID when the mouse
				left button is released.
		@returns @a True if the target ID matches the payload ID and the mouse left
				 button is released; @a False otherwise.
		*/
		static bool AcceptPayload(const char* _targetID) noexcept
		{
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			{
				payloadCharged = false;
				return strcmp(payloadID, _targetID) == 0;
			}
			return false;
		}

	};
}