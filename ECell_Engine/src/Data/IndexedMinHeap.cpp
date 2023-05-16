#include "Data/IndexedMinHeap.hpp"

void ECellEngine::Data::IndexedMinHeap::BuildMinHeap()
{
	for (int i = Parent(heapSize) - 1; i > -1; i--)
	{
		MinHeapify(i);
	}
}

void ECellEngine::Data::IndexedMinHeap::BubbleUp(const std::size_t& _nodeIdx)
{
	int parent = Parent(_nodeIdx);
	if (heap[_nodeIdx].second < heap[parent].second)
	{
		Swap(_nodeIdx, parent);
		BubbleUp(parent);
	}
}

void ECellEngine::Data::IndexedMinHeap::Initialize(const std::vector<std::pair<std::size_t, float>> _heap)
{
	heap = _heap;
	heapSize = heap.size();
	indeces.reserve(heapSize);
	for (int i = 0; i < heapSize; i++)
	{
		indeces.push_back(i);
	}

	BuildMinHeap();
}

void ECellEngine::Data::IndexedMinHeap::MinHeapify(const std::size_t& _nodeIdx)
{
	int r = Right(_nodeIdx);
	int l = Left(_nodeIdx);
	int lowest = _nodeIdx;

	if (l < heapSize && heap[l].second < heap[lowest].second)
	{
		lowest = l;
	}

	if (r < heapSize && heap[r].second < heap[lowest].second)
	{
		lowest = r;
	}

	if (lowest != _nodeIdx)
	{
		Swap(_nodeIdx, lowest);
		MinHeapify(lowest);
	}
}

void ECellEngine::Data::IndexedMinHeap::Swap(const std::size_t& _nodeIdx1, const std::size_t& _nodeIdx2)
{
	indeces[heap[_nodeIdx1].first] = _nodeIdx2;
	indeces[heap[_nodeIdx2].first] = _nodeIdx1;

	std::pair<int, float> temp = heap[_nodeIdx1];
	heap[_nodeIdx1] = heap[_nodeIdx2];
	heap[_nodeIdx2] = temp;
}

void ECellEngine::Data::IndexedMinHeap::UpdateHeapFrom(const std::size_t& _positionIdx, const float& _compareValue)
{
	if (heap[indeces[_positionIdx]].second < _compareValue)
	{
		BubbleUp(indeces[_positionIdx]);
	}
	else
	{
		MinHeapify(indeces[_positionIdx]);
	}
}