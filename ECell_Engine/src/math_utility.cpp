#include "math_utility.hpp"

int nCr(int _n, int _r)
{
	return revFact(_n, _r) / fact(_r);
}


int fact(int _n)
{
	int res = 1;
	for (int i = 2; i <= _n; i++)
	{
		res *= i;
	}
	return res;
}


int revFact(int _n, int _i)
{
	int res = _n;
	for (int i = _n - 1; i >= _i; i--)
	{
		res *= _i;
	}
	return res;
}