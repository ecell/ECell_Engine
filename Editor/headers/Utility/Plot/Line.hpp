#pragma once

#include <cstring>

#include "Utility/Plot/UpdateController.hpp"
#include "Utility/Plot/ScrollingBuffer.hpp"

//forward declaration of NodeId
namespace ax::NodeEditor
{
	struct NodeId;
}

namespace ECellEngine::Editor::Utility::Plot
{
	struct Line
	{
	private:
		UpdateController_Never updateControllerNever;
		UpdateController_Always updateControllerAlways;
		UpdateController_OnChange updateControllerOnChange;
		UpdateController_EveryNthFrame updateControllerEveryNthFrame;
		UpdateController_EveryXSeconds updateControllerEveryXSeconds;

	public:
		std::size_t id;

		UpdateController* updateController = &updateControllerNever;
		char* updateSchemeNames[5] { "Never", "Always", "OnChange", "EveryNthFrame", "EveryXSeconds" };

		ScrollingBuffer dataPoints;
		float* ptrY = nullptr;

		ImVec4 color{ 1.f, 1.f, 1.f, 1.f };
		char lineLegend[64] = "f(x)";

		Line(std::size_t _matchingDataId, unsigned short _maxNbDataPoints) :
			id{ _matchingDataId }
		{
			dataPoints.Data.reserve(_maxNbDataPoints);
			dataPoints.AddPoint(0.f, 0.f);
		}

		Line(const Line& _other) :
			id(_other.id), dataPoints(_other.dataPoints), ptrY(_other.ptrY)
		{
			std::memcpy(lineLegend, _other.lineLegend, 64);
		}

		//Defining the operators to be able to use lower_bound (binary search)
		//on the vector of lines.
		operator std::size_t();
		friend inline bool operator< (const Line& lhs, const Line& rhs)
		{
			return lhs.id < rhs.id;
		}

		inline const char* GetCurrentUpdateSchemeName() noexcept
		{
			return updateSchemeNames[updateController->scheme];
		}

		inline const char* GetUpdateSchemeName(unsigned short _updateSheme) noexcept
		{
			return updateSchemeNames[_updateSheme];
		}

		void Draw();

		void SwitchUpdateController(unsigned short _updateScheme) noexcept;

		void Update(float _x) noexcept;

	};
}