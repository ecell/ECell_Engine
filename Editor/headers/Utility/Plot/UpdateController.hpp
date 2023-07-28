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
	enum UpdateScheme
	{
		UpdateScheme_Never,// = 0, Never add data to the structure that stores it.
		UpdateScheme_Always,// = 1, Always add data to the structure that stores it.
		UpdateScheme_OnChange,// = 2, Add data to the structure that stores it only when the new value is different from the previous one.
		UpdateScheme_EveryNthFrame,// = 3, Add data to the structure that stores it every N frames.
		UpdateScheme_EveryXSeconds,// = 4, Add data to the structure that stores it every X seconds.

		UpdateScheme_Count//The number of update schemes.
	};

	/*!
	@brief A base class for the update controllers.
	*/
	struct UpdateController
	{
		UpdateScheme scheme = UpdateScheme_Never;

		virtual ~UpdateController() = default;

		/*!
		@brief Get the pointer to the threshold value that controls the update
			   schem, if any.
		*/
		virtual void* Get() noexcept = 0;

		/*!
		@brief Set the threshold value that controls the update scheme, if any.
		*/
		virtual void Set(void* _controlValue) noexcept = 0;

		/*!
		@brief Reset the update controller. In particular, reset the threshold
			   value that controls the update scheme, if any.
		*/
		virtual void Reset() noexcept = 0;

		/*!
		@brief Test if the update scheme is satisfied.
		@returns True if the update scheme is satisfied, false otherwise.
		*/
		inline virtual bool TestUpdate() noexcept = 0;

	};

	/*!
	@brief An update controller that never updates.
	*/
	struct UpdateController_Never final : public UpdateController
	{
		UpdateController_Never() noexcept
		{
			scheme = UpdateScheme_Never;
		}

		inline void* Get() noexcept override
		{
			return nullptr;
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
	struct UpdateController_Always final : public UpdateController
	{
		UpdateController_Always() noexcept
		{
			scheme = UpdateScheme_Always;
		}

		inline void* Get() noexcept override
		{
			return nullptr;
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
	struct UpdateController_OnChange final : public UpdateController
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

		UpdateController_OnChange() noexcept
		{
			scheme = UpdateScheme_OnChange;
		}

		inline void* Get() noexcept override
		{
			return nullptr;
		}

		void Reset() noexcept override;

		void Set(void* _controlValue) noexcept override;

		bool TestUpdate() noexcept override;
	};

	/*!
	@brief An update controller that updates every N frames.
	*/
	struct UpdateController_EveryNthFrame final : public UpdateController
	{
		/*!
		@brief The current frame count.
		*/
		int frameCount = 0;

		/*
		@brief The number of frames that must be reached before updating.
		*/
		int frameInterval = 100;

		UpdateController_EveryNthFrame() noexcept
		{
			scheme = UpdateScheme_EveryNthFrame;
		}

		inline void* Get() noexcept override
		{
			return &frameInterval;
		}

		void Reset() noexcept override;

		void Set(void* _controlValue) noexcept override;

		bool TestUpdate() noexcept override;
	};

	/*!
	@brief An update controller that updates every X seconds.
	*/
	struct UpdateController_EveryXSeconds final : public UpdateController
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

		UpdateController_EveryXSeconds() noexcept
		{
			scheme = UpdateScheme_EveryXSeconds;

			previousTime = timer.ReadHighResTimer();
		}

		inline void* Get() noexcept override
		{
			return &timeInterval;
		}

		void Reset() noexcept override;

		void Set(void* _controlValue) noexcept override;
		
		bool TestUpdate() noexcept override;
	};
}