#pragma once
#include <cstddef> // std::size_t

namespace ECellEngine::Util
{
	/*!
	@brief Copies a string into another.
	@details Copies the minimal number of characters between the @p _src and the @p _count.
	@param _dest The destination string.
	@param _src The source string.
	@param count The maximum number of characters to copy.
	*/
	extern void StrCopy(char* _dest, const char* _src, const std::size_t _count);

	/*!
	@brief Finds the first occurrence of a character in a string.
	@param _str The string to search.
	@param _c The character to search for.
	@return The index of the first occurrence of the character in the string.
	@remark Throws out_of_range if the character is not found.
	*/
	extern unsigned int FindFirstOccurrence(const char* _str, char _c);

	/*!
	@brief Compares two strings to see if _str1 is before _str2 in alphabetical
			order.
	@details This function is not case sensitive as characters are converted to
			 lowercase before comparison. Also, if it happens that one string is
			 a prefix of the other, the shorter string is considered to be before
			 the longer one. For example, "abc" is before "abcd" in alphabetical
			 order. So if @p _str1 is "abc" and @p _str2 is "abcd", this function
			 will return true (false, otherwise).
	@param _str1 The first string.
	@param _str2 The second string.
	@return True if _str1 is before _str2 in alphabetical order, false otherwise.
	*/
	extern bool IsBeforeInAlphabeticalOrder(const char* _str1, const char* _str2);

	/*!
	@brief Computes the size of a null-terminated array of strings (represented
			as pointers to char).
	@param _strArray The array of strings.
	@return The size of the array.
	*/
	extern std::size_t StrArraySize(char** _strArray);

	/*!
	@brief Splits a string into tokens according to delimiters into a newly
			allocated array of strings.
	@param _str The string to split.
	@param _delim The delimiters.
	@param _out_nbTokens The number of tokens found.
	@param _maxPower The maximum power of 2 for the max number of tokens.
	@return A newly allocated array of strings containing the tokens.
	*/
	extern char** SplitString(char* _str, const char* _delim, unsigned int* _out_nbTokens, unsigned char _maxPower = 16);

}