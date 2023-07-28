#include <math.h>

namespace ECellEngine::Maths
{
	/// <summary>
	/// Computes !_n
	/// </summary>
	int fact(int _n);

	/*!
	@brief Computes the maximum of two values.
	@param a The first value.
	@param b The second value.
	@paramt T The type of the values.
	@return The maximum of a and b.
	*/
	template<class T>
	static inline const T& max(const T& a, const T& b)
	{
		return (a > b) ? a : b;
	}

	/*!
	@brief Computes the minimum of two values.
	@param a The first value.
	@param b The second value.
	@paramt T The type of the values.
	@return The minimum of a and b.
	*/
	template<class T>
	static inline const T& min(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	}

	///<summary>
	/// Computes the number of subsets of size _r made
	/// of distinguishable elements within a set of _n elements.
	///</summary>
	int nCr(int _n, int _r);

	/// <summary>
	/// Computes a reversed factorial of _n.
	/// Meaning: n * (n-1) * (n-2) * ... * (n-i).
	/// </summary>
	/// <remarks>Does not check if _i less or equal to 0 </remarks>
	int revFact(int _n, int _i);

	/// <summary>
	/// Writes the mathematical expression of !_n as a MathML string.
	/// </summary>
	//std::string str_fact(int _n);

	/// <summary>
	/// Writes the mathematical expression of nCr as a MathML string.
	/// </summary>
	/// <param name="_n">A string representing the name of the
	/// total quantity of distinct elements as a silent vaiable.</param>
	/// <param name="_r">An integer giving the size of the subset</param>
	/// <returns>A string such as n*(n-1)*0.5</returns>
	//std::string str_nCR(std::string _n, int _r);

	/// <summary>
	/// Writes the mathematical expression reversed factorial of _n
	/// as a MathML string.
	/// </summary>
	//std::string str_revFact(std::string _n, int _i);
}