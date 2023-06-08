#pragma once

namespace ECellEngine::Editor::Utility::Plot
{
    enum UpdateScheme
    {
        UpdateScheme_Never,
        UpdateScheme_Always,
        UpdateScheme_OnChange,
        UpdateScheme_EveryNthFrame,
        UpdateScheme_EveryXSeconds,

        UpdateScheme_Count
    };

    struct UpdateController
    {
        UpdateScheme scheme = UpdateScheme_Never;

        virtual ~UpdateController() = default;

        virtual void* Get() noexcept = 0;

        virtual void Set(void* _controlValue) noexcept = 0;

        virtual void Reset() noexcept = 0;

        inline virtual bool TestUpdate() noexcept = 0;

    };

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

    struct UpdateController_OnChange final : public UpdateController
    {
        float* ptrY = nullptr;
        float lastY = 0.f;

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

    struct UpdateController_EveryNthFrame final : public UpdateController
    {
		int frameCount = 0;
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

    struct UpdateController_EveryXSeconds final : public UpdateController
    {
		float time = 0.f;
        float timeInterval = 5.f;

        UpdateController_EveryXSeconds() noexcept
        {
            scheme = UpdateScheme_EveryXSeconds;
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