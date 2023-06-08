#pragma once

#include "implot.h"

namespace ECellEngine::Editor::Utility::Plot
{
    enum UpdateScheme
    {
        UpdateScheme_Always,
        UpdateScheme_OnChange,
        UpdateScheme_EveryNthFrame,
        UpdateScheme_EveryXSeconds
    };
}