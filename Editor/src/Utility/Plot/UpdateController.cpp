#include "implot.h"
#include "Utility/Plot/UpdateController.hpp"

void ECellEngine::Editor::Utility::Plot::UpdateController_OnChange::Reset() noexcept
{
	previous = *current;
}

void ECellEngine::Editor::Utility::Plot::UpdateController_OnChange::Set(void* _controlValue) noexcept
{
	current = (float*)_controlValue;
}

bool ECellEngine::Editor::Utility::Plot::UpdateController_OnChange::TestUpdate() noexcept
{
    return *current != previous;
}

void ECellEngine::Editor::Utility::Plot::UpdateController_EveryNthFrame::Reset() noexcept
{
	frameCount = 0;
}

void ECellEngine::Editor::Utility::Plot::UpdateController_EveryNthFrame::Set(void* _controlValue) noexcept
{
	frameInterval = *(int*)_controlValue;
}

bool ECellEngine::Editor::Utility::Plot::UpdateController_EveryNthFrame::TestUpdate() noexcept
{
	return ++frameCount % frameInterval == 0;
}

void ECellEngine::Editor::Utility::Plot::UpdateController_EveryXSeconds::Reset() noexcept
{
	previousTime = timer.ReadHighResTimer();
}

void ECellEngine::Editor::Utility::Plot::UpdateController_EveryXSeconds::Set(void* _controlValue) noexcept
{
	timeInterval = *(float*)_controlValue;
}

bool ECellEngine::Editor::Utility::Plot::UpdateController_EveryXSeconds::TestUpdate() noexcept
{
	currentTime = timer.ReadHighResTimer();
	return timer.GetDuration(previousTime, currentTime) >= timeInterval;
}