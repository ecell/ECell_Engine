#pragma once

#include <iostream>
#include <vector>

namespace ECellEngine::Data
{
	struct InRow
	{
		std::vector<int> in;
		std::vector<int> s;//stoichiometry

		InRow(std::vector<int>* _in, std::vector<int>* _s) : in(*_in), s(*_s)
		{

		}
	};

	struct OutRow
	{
		std::vector<int> out;
		std::vector<int> s;//stoichiometry
		OutRow(std::vector<int>* _out, std::vector<int>* _s) : out(*_out), s(*_s)
		{

		}
	};


	struct DepRow
	{
		std::vector<int> dep;

		DepRow(std::vector<int> _dep) : dep(_dep)
		{

		}
	};


	class IndexedTauMinHeap
	{
	private:
		std::vector<std::pair<int, float>>* heap;
		int heapSize;
		std::vector<int> pointers;

		/// <summary>
		/// Reorganizes the heap in place to achieve a MinHeap.
		/// </summary>
		/// <remarks>
		///	Used in the constructor only.	
		/// </remarks>
		void BuildMinHeap();

		/// <summary>
		/// Adjust position of the heap node _i if its value is
		/// lower than its parent genealogy.
		/// </summary>
		void BubbleUp(int _i);

		/// <summary>
		/// Returns the index in the heap of the left node.
		/// </summary>
		/// <param name="_i">Index of the current node.</param>
		int Left(int _i);

		/// <summary>
		/// Recursively goes through the Heap starting with node _i
		/// to achieve the MinHeap structure.
		/// </summary>
		/// <param name="_i"></param>
		void MinHeapify(int _i);

		/// <summary>
		/// Returns the index in the heap of the parent node.
		/// </summary>
		/// <param name="_i">Index of the current node.</param>
		int Parent(int _i);

		/// <summary>
		/// Returns the index in the heap of the right node.
		/// </summary>
		/// <param name="_i">Index of the current node.</param>
		int Right(int _i);

		/// <summary>
		/// Swaps nodes _i and _j of the heap.
		/// </summary>
		void Swap(int _i, int _j);

	public:
		IndexedTauMinHeap() {}

		friend std::ostream& operator<<(std::ostream& os, const IndexedTauMinHeap& _itmh);

#pragma region - Accessors -
		/// <summary>
		/// Gets the root node.
		/// </summary>
		/// <reamrks>Contrary to classic MinHeaps, the root is not popped/extracted.</remarks>
		std::pair<int, float> GetRoot();

		/// <summary>
		/// Gets the value tau of the node indicated by the pointer <paramref name="_ptr"/>.
		/// </summary>
		/// <param name="_ptr">Index of the node in <see cref="pointers"/>.</param>
		float GetTauFromPointer(int _ptr);
#pragma endregion

#pragma region - Mutators -
		/// <summary>
		/// Sets the value tau in the node at the root of the MinHeap.
		/// </summary>
		void SetTauInRoot(float _newTau);

		/// <summary>
		/// Sets the value tau in the node indicated by the pointer <paramref name="_ptr".
		/// </summary>
		void SetTauFromPointer(int _ptr, float _newTau);
#pragma endregion 

#pragma region - Logic -
		/// <summary>
		/// Initializes the Indexed MinHeap.
		/// </summary>
		/// <param name="_heap">Pointer to the vector to be indexed Min-Heapified.</param>
		void Initialize(std::vector<std::pair<int, float>>* _heap);

		/// <summary>
		/// Min-heapifies the heap root.
		/// </summary>
		void UpdateRoot();

		/// <summary>
		/// Updates the indexed Minheap. Either bubles up the node (if <paramref name="_oldTau"/>
		/// is lower than its parent), or min-heapifies it.
		/// </summary>
		void UpdateFromPointer(int _ptr, float _oldTau);
#pragma endregion
	};
}