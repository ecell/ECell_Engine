#include <cstddef>
#include "Gillespie_NRM_R_Data.hpp"


std::ostream& operator<<(std::ostream& os, const IndexedTauMinHeap& _itmh)
{
	for (auto it = _itmh.heap->cbegin(); it != _itmh.heap->cend(); ++it)
	{
		std::cout << "(" << it->first << ", " << it->second <<") ";
	}
	std::cout << std::endl;
}

std::pair<int, float> IndexedTauMinHeap::GetRoot()
{
	return heap->at(0);
}

float IndexedTauMinHeap::GetTauFromPointer(int _ptrID)
{
	return heap->at(pointers[_ptrID]).second;
}

void IndexedTauMinHeap::BuildMinHeap()
{
	for (int i = Parent(heapSize) - 1; i > -1; i--)
	{
		MinHeapify(i);
	}
}

void IndexedTauMinHeap::BubbleUp(int _i)
{
	int parent = Parent(_i);
	if (heap->at(_i).second < heap->at(parent).second)
	{
		Swap(_i, parent);
		BubbleUp(parent);
	}
}

void IndexedTauMinHeap::Initialize(std::vector<std::pair<int, float>>* _heap)
{
	heap = _heap;
	heapSize = heap->size();
	pointers.reserve(heapSize);
	for (int i = 0; i < heapSize; i++)
	{
		pointers.push_back(i);
	}

	BuildMinHeap();
}

int IndexedTauMinHeap::Left(int _i)
{
	return 2*_i+1;
}

void IndexedTauMinHeap::MinHeapify(int _i)
{
	int r = Right(_i);
	int l = Left(_i);
	int lowest = _i;

	if (l < heapSize && heap->at(l).second < heap->at(lowest).second)
	{
		lowest = l;
	}

	if (r < heapSize && heap->at(r).second < heap->at(lowest).second)
	{
		lowest = r;
	}

	if (lowest != _i)
	{
		Swap(_i, lowest);
		MinHeapify(lowest);
	}
}

int IndexedTauMinHeap::Parent(int _i)
{
	return static_cast<int>(0.5f * (_i - 1));
}

int IndexedTauMinHeap::Right(int _i)
{
	return 2 * _i + 2;
}

void IndexedTauMinHeap::SetTauInRoot(float _tau)
{
	heap->at(0).second = _tau;
}

void IndexedTauMinHeap::SetTauFromPointer(int _ptrID, float _tau)
{
	heap->at(pointers[_ptrID]).second = _tau;
}

void IndexedTauMinHeap::Swap(int _i, int _j)
{
	pointers[heap->at(_i).first] = _j;
	pointers[heap->at(_j).first] = _i;

	std::pair<int, float> temp = heap->at(_i);
	heap->at(_i) = heap->at(_j);
	heap->at(_j) = temp;
}

void IndexedTauMinHeap::UpdateRoot()
{
	MinHeapify(0);
}

void IndexedTauMinHeap::UpdateFromPointer(int _ptrID, float old_tau)
{
	if (heap->at(pointers[_ptrID]).second < old_tau)
	{
		BubbleUp(pointers[_ptrID]);
	}
	else
	{
		MinHeapify(pointers[_ptrID]);
	}
}