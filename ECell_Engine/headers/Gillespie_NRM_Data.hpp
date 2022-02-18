#include <iostream>
#include <vector>

struct InkRow
{
	std::vector<int>* in;
	float k;

	InkRow(std::vector<int>* _in, float _k) : in(_in), k(_k)
	{

	}
};

struct OutRow
{
	std::vector<int>* out;
	OutRow(std::vector<int>* _out) : out(_out)
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
	IndexedTauMinHeap(){}

	/*IndexedTauMinHeap(std::vector<std::pair<int, float>>* _heap) : heap(_heap)
	{
		heapSize = heap->size();
		pointers.reserve(heapSize);
		for (int i = 0; i < heapSize; i++)
		{
			pointers.push_back(i);
		}

		BuildMinHeap();
	}*/

	friend std::ostream& operator<<(std::ostream& os, const IndexedTauMinHeap& _itmh);

	std::pair<int, float> GetRoot();
	float GetTauFromPointer(int);


	void SetTauInRoot(float);
	void SetTauFromPointer(int, float);

	void Initialize(std::vector<std::pair<int, float>>* _heap);

	void UpdateRoot();
	void UpdateFromPointer(int, float);
};