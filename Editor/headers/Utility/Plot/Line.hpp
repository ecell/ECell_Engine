#pragma once

#include <cstring>

#include "Utility/Plot/PlotUtility.hpp"
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
		std::size_t id;

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

		void Draw() noexcept;

		void Update(float _x) noexcept;

	};
}