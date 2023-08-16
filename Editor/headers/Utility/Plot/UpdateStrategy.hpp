#pragma once

#include "Core/Timer.hpp"

namespace ECellEngine::Editor::Utility::Plot
{
	/*!
	@brief Enums to describe various conditions on which an update should be
		   performed.
	@details This is defined in the context of plotting data. It controls when
			 data should be added to the structure that stores it and that is
			 plotteds.
	*/
	enum UpdateStrategyType : unsigned char
	{
		UpdateStrategyType_Never,// = 0, Never add data to the structure that stores it.
		UpdateStrategyType_Always,// = 1, Always add data to the structure that stores it.
		UpdateStrategyType_OnChange,// = 2, Add data to the structure that stores it only when the new value is different from the previous one.
		UpdateStrategyType_EveryNthFrame,// = 3, Add data to the structure that stores it every N frames.
		UpdateStrategyType_EveryXSeconds,// = 4, Add data to the structure that stores it every X seconds.

		UpdateStrategyType_Count//The number of update strategys.
	};

	/*!
	@brief A base class for the update controllers.
	*/
	struct UpdateStrategy
	{
		UpdateStrategyType strategyType = UpdateStrategyType_Never;
		static char* updateStrategyNames[5];

		virtual ~UpdateStrategy() = default;

		/*!
		@brief Get the pointer to the threshold value that controls the update
			   schem, if any.
		*/
		virtual void* Get() noexcept = 0;

		/*!
		@brief Get the name of this update strategy.
		*/
		virtual const char* GetName() noexcept = 0;

		/*!
		@brief Get the name of the given update strategy.
		@param _type The integer encoding the strategy type in the enum
					 ECellEngine::Editor::Utility::Plot::UpdateStrategyType.
		*/
		static const char* GetName(unsigned char _type) noexcept
		{
			return updateStrategyNames[_type];
		}

		/*!
		@brief Set the threshold value that controls the update strategy, if any.
		*/
		virtual void Set(void* _controlValue) noexcept = 0;

		/*!
		@brief Reset the update controller. In particular, reset the threshold
			   value that controls the update strategy, if any.
		*/
		virtual void Reset() noexcept = 0;

		/*!
		@brief Test if the update strategy is satisfied.
		@returns True if the update strategy is satisfied, false otherwise.
		*/
		virtual bool TestUpdate() noexcept = 0;

	};

	/*!
	@brief An update controller that never updates.
	*/
	struct UpdateStrategyNever final : public UpdateStrategy
	{
		UpdateStrategyNever() noexcept
		{
			strategyType = UpdateStrategyType_Never;
		}

		inline void* Get() noexcept override
		{
			return nullptr;
		}

		inline const char* GetName() noexcept override
		{
			return "Never";
		}

		void Reset() noexcept override {};

		void Set(void* _controlValue) noexcept override {};

		inline bool TestUpdate() noexcept override
		{
			return false;
		};
	};

	/*!
	@brief An update controller that always updates.
	*/
	struct UpdateStrategyAlways final : public UpdateStrategy
	{
		UpdateStrategyAlways() noexcept
		{
			strategyType = UpdateStrategyType_Always;
		}

		inline void* Get() noexcept override
		{
			return nullptr;
		}

		inline const char* GetName() noexcept override
		{
			return "Always";
		}

		void Reset() noexcept override {};

		void Set(void* _controlValue) noexcept override {};

		inline bool TestUpdate() noexcept override
		{
			return true;
		}
	};

	/*!
	@brief An update controller that updates only when the value changes.
	*/
	struct UpdateStrategyOnChange final : public UpdateStrategy
	{
		/*!
		@brief Pointer to the current value.
		*/
		float* current = nullptr;

		/*!
		@brief The previous value of ::current.
		@details This is used to check if the value has changed.
		*/
		float previous = 0.f;

		UpdateStrategyOnChange() noexcept
		{
			strategyType = UpdateStrategyType_OnChange;
		}

		inline void* Get() noexcept override
		{
			return nullptr;
		}

		inline const char* GetName() noexcept override
		{
			return "OnChange";
		}

		void Reset() noexcept override;

		void Set(void* _controlValue) noexcept override;

		bool TestUpdate() noexcept override;
	};

	/*!
	@brief An update controller that updates every N frames.
	*/
	struct UpdateStrategyEveryNthFrame final : public UpdateStrategy
	{
		/*!
		@brief The current frame count.
		*/
		int frameCount = 0;

		/*
		@brief The number of frames that must be reached before updating.
		*/
		int frameInterval = 100;

		UpdateStrategyEveryNthFrame() noexcept
		{
			strategyType = UpdateStrategyType_EveryNthFrame;
		}

		inline void* Get() noexcept override
		{
			return &frameInterval;
		}

		inline const char* GetName() noexcept override
		{
			return "EveryNthFrame";
		}

		void Reset() noexcept override;

		void Set(void* _controlValue) noexcept override;

		bool TestUpdate() noexcept override;
	};

	/*!
	@brief An update controller that updates every X seconds.
	*/
	struct UpdateStrategyEveryXSeconds final : public UpdateStrategy
	{
		/*!
		@brief The timer used to check the time interval.
		*/
		Core::Timer timer;

		/*
		@brief The current time.
		*/
		float currentTime = 0.f;

		/*!
		@brief The previous time.
		*/
		float previousTime = 0.f;

		/*!
		@brief The time interval that must be reached before updating.
		*/
		float timeInterval = 5.f;

		UpdateStrategyEveryXSeconds() noexcept
		{
			strategyType = UpdateStrategyType_EveryXSeconds;

			previousTime = timer.ReadHighResTimer();
		}

		inline void* Get() noexcept override
		{
			return &timeInterval;
		}

		inline const char* GetName() noexcept override
		{
			return "EveryXSeconds";
		}

		void Reset() noexcept override;

		void Set(void* _controlValue) noexcept override;
		
		bool TestUpdate() noexcept override;
	};
}