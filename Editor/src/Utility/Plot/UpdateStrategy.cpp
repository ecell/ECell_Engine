#include "implot.h"
#include "Utility/Plot/UpdateStrategy.hpp"

char* ECellEngine::Editor::Utility::Plot::UpdateStrategy::updateStrategyNames[5] = { "Never", "Always", "OnChange", "EveryNthFrame", "EveryXSeconds" };

void ECellEngine::Editor::Utility::Plot::UpdateStrategyOnChange::Reset() noexcept
{
	previous = *current;
}

void ECellEngine::Editor::Utility::Plot::UpdateStrategyOnChange::Set(void* _controlValue) noexcept
{
	current = (float*)_controlValue;
}

bool ECellEngine::Editor::Utility::Plot::UpdateStrategyOnChange::TestUpdate() noexcept
{
    return *current != previous;
}

void ECellEngine::Editor::Utility::Plot::UpdateStrategyEveryNthFrame::Reset() noexcept
{
	frameCount = 0;
}

void ECellEngine::Editor::Utility::Plot::UpdateStrategyEveryNthFrame::Set(void* _controlValue) noexcept
{
	frameInterval = *(int*)_controlValue;
}

bool ECellEngine::Editor::Utility::Plot::UpdateStrategyEveryNthFrame::TestUpdate() noexcept
{
	return ++frameCount % frameInterval == 0;
}

void ECellEngine::Editor::Utility::Plot::UpdateStrategyEveryXSeconds::Reset() noexcept
{
	previousTime = timer.ReadHighResTimer();
}

void ECellEngine::Editor::Utility::Plot::UpdateStrategyEveryXSeconds::Set(void* _controlValue) noexcept
{
	timeInterval = *(float*)_controlValue;
}

bool ECellEngine::Editor::Utility::Plot::UpdateStrategyEveryXSeconds::TestUpdate() noexcept
{
	currentTime = timer.ReadHighResTimer();
	return timer.Duration(previousTime, currentTime) >= timeInterval;
}