
///<summary>
/// Computes the number of subsets of size _r made
/// of distinguishable elements within a set of _n elements.
///</summary>
int nCr(int _n, int _r);

/// <summary>
/// Computes !_n
/// </summary>
int fact(int _n);

/// <summary>
/// Computes a reversed factorial of _n.
/// Meaning: n * (n-1) * (n-2) * ... * (n-i).
/// </summary>
/// <remarks>Does not check if _i less or equal to 0 </remarks>
int revFact(int _n, int _i);