#pragma once

#include <cmath>
#include <cstddef>
#include <vector>

namespace ECellEngine::Data
{
	class IndexedMinHeap
	{
	private:
		std::vector<std::pair<std::size_t, float>> heap;
		std::size_t heapSize;
		std::vector<std::size_t> indeces;

		/*
		@brief Reorganizes the heap in place to achieve a MinHeap.
		@remark Used in the constructor only.
		*/
		void BuildMinHeap();
		
		/*
		@brief Adjust position of the heap node _i if its value is
				lower than its parent genealogy.
		*/
		void BubbleUp(const std::size_t& _nodeIdx);
		
		/*
		@brief Computes the index the left node.
		@param _parentNodeIdx Index of the current node (parent from which we
				are looking for the left child).
		@return The index in the heap of the left node.
		*/
		inline const std::size_t Left(const std::size_t& _parentNodeIdx) const
		{
			return 2 * _parentNodeIdx + 1;
		}

		/*
		@brief Recursively goes through the Heap starting with node _i
			   to achieve the MinHeap structure.
		*/
		void MinHeapify(const std::size_t& _nodeIdx);

		/*
		@brief Computes the index of the parent node.
		@param _childNodeIndex Index of the current node.
		@return The index in the heap of the parent node.
		*/
		inline const std::size_t Parent(const std::size_t& _childNodeIndex) const
		{
			return std::floor(0.5f * (_childNodeIndex - 1));
		}

		/*
		@brief Computes the index the right node.
		@param _parentNodeIdx Index of the current node (parent from which we
				are looking for the right child).
		@return The index in the heap of the right node.
		*/
		inline const std::size_t Right(const std::size_t& _parentNodeIdx) const
		{
			return 2 * _parentNodeIdx + 2;
		}

		/*
		@brief Swaps nodes _nodeIdx1 and _nodeIdx2 of the heap.
		*/
		void Swap(const std::size_t& _nodeIdx1, const std::size_t& _nodeIdx2);

	public:
		IndexedMinHeap(){}

		/*
		@brief Gets the root node.
		@remark Contrary to classic MinHeaps, the root is not popped/extracted
				when we get the root.
		@returns The pair at the root of the heap.
		*/
		inline const std::pair<std::size_t, float> GetRoot() const
		{
			return heap[0];
		}

		/*
		@brief Gets the value of the node indicated by the index stored in @see indeces 
				at position @p _positionIdx.
		@param _positionIdx Position in @see indeces of the index of the node in the heap
							 for which we want to get the value.
		*/
		inline const float GetValueAtIndex( const std::size_t& _positionIdx) const
		{
			return heap[indeces[_positionIdx]].second;
		}

		/*
		@brief Changes the value in the node at the root of the MinHeap.
		@param _value The new value to put in the root node.
		*/
		inline void SetValueInRoot(const float _value)
		{
			heap[0].second = _value;
		}

		/*
		@brief Changes the value in the node indicated by the index stored in @see indeces 
				at position @p _positionIdx.
		@param _positionIdx Position in @see indeces of the index of the node in the heap
							for which we want to change the value.
		@param _value The new value to put in the node.
		*/
		inline void SetValueAtIndex(const std::size_t& _positionIdx, const float _value)
		{
			heap[indeces[_positionIdx]].second = _value;
		}

		/*
		@brief Initializes the Indexed MinHeap.
		@param _heap The heap to be indexed Min-Heapified.
		*/
		void Initialize(const std::vector<std::pair<std::size_t, float>> _heap);

		/*
		@brief Min-heapifies the heap root.
		*/
		inline void UpdateHeapFromRoot()
		{
			MinHeapify(0);
		}

		/*
		@brief Updates the indexed Minheap. Either bubles up the node or min-heapifies it.
		@param _positionIdx Position in @see indeces of the index of the node in the heap
							for which we want to change the value.
		@param _compareValue A threshold value to which the value of the node is compared to.
							 If the value of the node if strictly below the threshold, it is
							 bubbled up. Otherwise, it is min-heapified.
		
		*/
		void UpdateHeapFrom(const std::size_t& _positionIdx, const float& _compareValue);

	};
}