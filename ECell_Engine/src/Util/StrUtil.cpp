#include "Maths/MathUtility.hpp"
#include "Util/StrUtil.hpp"

#include <cctype> // std::tolower
#include <stdlib.h> // malloc, free
#include <string.h> // strlen, strcpy, strtok
#include <stdexcept>

void ECellEngine::Util::StrCopy(char* _dest, const char* _src, const std::size_t _count)
{
	memcpy(_dest, _src, _count);
}

bool ECellEngine::Util::IsBeforeInAlphabeticalOrder(const char* _lhs, const char* _rhs)
{
	std::size_t lhsLen = strlen(_lhs);
	std::size_t rhsLen = strlen(_rhs);
	std::size_t minLen = lhsLen < rhsLen ? lhsLen : rhsLen;

	for (std::size_t i = 0; i < minLen; ++i)
	{
		if (std::tolower(_lhs[i]) < std::tolower(_rhs[i]))
		{
			return true;
		}
		else if (std::tolower(_lhs[i]) > std::tolower(_rhs[i]))
		{
			return false;
		}
	}

	return lhsLen <= rhsLen;
}


unsigned int ECellEngine::Util::FindFirstOccurrence(const char* _str, char _c)
{
	std::size_t len = strlen(_str);
	for (unsigned int i = 0; i < len; ++i)
	{
		if (_str[i] == _c)
		{
			return i;
		}
	}
	throw std::out_of_range("[ERROR] Character not found.");
}

std::size_t ECellEngine::Util::StrArraySize(char** _strArray)
{
	std::size_t size = 0;

	while (*_strArray++)
	{
		++size;
	}

	return size;
}

char** ECellEngine::Util::SplitString(char* _str, const char* _delim, unsigned int* _out_nbTokens, unsigned char _maxPower)
{
	char** tokens = nullptr;
	char** tmp = nullptr;
	char* token = strtok(_str, _delim);
	unsigned int nbTokens = 0;
	unsigned char power = 6;

	if (tokens = (char**)malloc(sizeof(char*) * ((unsigned long long)1 << ++power)))
	{
		memcpy(tokens, tmp, sizeof(char*) * nbTokens);
	}
	else
	{
		return nullptr;
	}

	while (token)
	{
		if (++nbTokens == (1 << power) - 1)
		{
			if (power == _maxPower)
			{
				free(tokens);
				return nullptr;
			}

			if (!(tokens = (char**)realloc(tmp = tokens, sizeof(char*) * ((unsigned long long)1 << ++power))))
			{
				free(tmp);
				return nullptr;
			}
		}

		tokens[nbTokens - 1] = token;
		token = strtok(nullptr, _delim);
	}

	*_out_nbTokens = nbTokens;
	return tokens;
}