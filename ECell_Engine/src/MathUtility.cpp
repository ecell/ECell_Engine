#include "MathUtility.hpp"

int ECellEngine::Maths::fact(int _n)
{
	int res = 1;
	for (int i = 2; i <= _n; i++)
	{
		res *= i;
	}
	return res;
}

int ECellEngine::Maths::nCr(int _n, int _r)
{
	return revFact(_n, _r) / fact(_r);
}

int ECellEngine::Maths::revFact(int _n, int _i)
{
	int res = _n;
	for (int i = _n - 1; i >= _i; i--)
	{
		res *= _i;
	}
	return res;
}

std::string ECellEngine::Maths::str_fact(int _n)
{
	int res = 1;
	std::string str_res = "    <cn type=\"integer\" sbml:units=\"dimensionless\"> ";

	for (int i = 2; i <= _n; i++)
	{
		res *= i;
	}
	
	str_res += std::to_string(res);
	str_res += " </cn>";

	return str_res;
}

std::string ECellEngine::Maths::str_nCR(std::string _n, int _r)
{
	std::string str_res = "<apply>";
	str_res += "<times/>";

	if (_r == 1)
	{
		str_res += "    <ci> ";
		str_res += _n;
		str_res += " </ci>";
	}

	else
	{
		str_res += "<apply>";

		str_res += "<divide/>";
		str_res += str_revFact(_n, _r);
		str_res += str_fact(_r);

		str_res += "</apply>";
	}

	str_res += "</apply>";

	return str_res;
}

std::string ECellEngine::Maths::str_revFact(std::string _n, int _i)
{
	std::string str_res = "<apply>";
	str_res += "<times/>";
	str_res += "    <ci> ";
	str_res += _n;
	str_res += " </ci>";
	for (int i = 1; i < _i; i++)
	{
		str_res += "<apply>";

		str_res += "<minus/>";
		str_res += "    <ci> ";
		str_res += _n;
		str_res += " </ci>";
		str_res += "    <cn type=\"integer\" sbml:units=\"item\">";
		str_res += std::to_string(i);
		str_res += " </cn>";

		str_res += "</apply>";
	}

	str_res += "</apply>";

	return str_res;
}