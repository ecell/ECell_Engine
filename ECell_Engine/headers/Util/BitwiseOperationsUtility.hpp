#pragma once

namespace ECellEngine::Util
{
#pragma region IsFlagSet
	static inline const bool IsFlagSet(const unsigned char _flags, const unsigned char _flagToCheck) noexcept
	{
		return (_flags & _flagToCheck);
	}

	static inline const bool IsFlagSet(const unsigned short _flags, const unsigned short _flagToCheck) noexcept
	{
		return (_flags & _flagToCheck);
	}

	static inline const bool IsFlagSet(const unsigned int _flags, const unsigned int _flagToCheck) noexcept
	{
		return (_flags & _flagToCheck);
	}
	
	static inline const bool IsFlagSet(const unsigned long _flags, const unsigned long _flagToCheck) noexcept
	{
		return (_flags & _flagToCheck);
	}

	static inline const bool IsFlagSet(const unsigned long long _flags, const unsigned long long _flagToCheck) noexcept
	{
		return (_flags & _flagToCheck);
	}
#pragma endregion

#pragma region SetFlag
	static inline void SetFlag(unsigned char& _flags, const unsigned char _flagToSet) noexcept
	{
		_flags |= _flagToSet;
	}

	static inline void SetFlag(unsigned short& _flags, const unsigned short _flagToSet) noexcept
	{
		_flags |= _flagToSet;
	}

	static inline void SetFlag(unsigned int& _flags, const unsigned int _flagToSet) noexcept
	{
		_flags |= _flagToSet;
	}

	static inline void SetFlag(unsigned long& _flags, const unsigned long _flagToSet) noexcept
	{
		_flags |= _flagToSet;
	}

	static inline void SetFlag(unsigned long long& _flags, const unsigned long long _flagToSet) noexcept
	{
		_flags |= _flagToSet;
	}
#pragma endregion

#pragma region ClearFlag
	static inline const void ClearFlag(unsigned char& _flags, const unsigned char _flagToClear) noexcept
	{
		_flags &= ~_flagToClear;
	}
	static inline const void ClearFlag(unsigned short& _flags, const unsigned short _flagToClear) noexcept
	{
		_flags &= ~_flagToClear;
	}
	static inline const void ClearFlag(unsigned int& _flags, const unsigned int _flagToClear) noexcept
	{
		_flags &= ~_flagToClear;
	}
	static inline const void ClearFlag(unsigned long& _flags, const unsigned long _flagToClear) noexcept
	{
		_flags &= ~_flagToClear;
	}
	static inline const void ClearFlag(unsigned long long& _flags, const unsigned long long _flagToClear) noexcept
	{
		_flags &= ~_flagToClear;
	}
#pragma endregion

#pragma region ToggleFlag
	static inline const bool ToggleFlag(unsigned char& _flags, const unsigned char _flagToToggle) noexcept
	{
		_flags ^= _flagToToggle;
		return IsFlagSet(_flags, _flagToToggle);
	}

	static inline const bool ToggleFlag(unsigned short& _flags, const unsigned short _flagToToggle) noexcept
	{
		_flags ^= _flagToToggle;
		return IsFlagSet(_flags, _flagToToggle);
	}

	static inline const bool ToggleFlag(unsigned int& _flags, const unsigned int _flagToToggle) noexcept
	{
		_flags ^= _flagToToggle;
		return IsFlagSet(_flags, _flagToToggle);
	}

	static inline const bool ToggleFlag(unsigned long& _flags, const unsigned long _flagToToggle) noexcept
	{
		_flags ^= _flagToToggle;
		return IsFlagSet(_flags, _flagToToggle);
	}

	static inline const bool ToggleFlag(unsigned long long& _flags, const unsigned long long _flagToToggle) noexcept
	{
		_flags ^= _flagToToggle;
		return IsFlagSet(_flags, _flagToToggle);
	}
#pragma endregion
}