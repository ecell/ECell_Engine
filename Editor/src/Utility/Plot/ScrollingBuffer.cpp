#include "Utility/Plot/ScrollingBuffer.hpp"

void ECellEngine::Editor::Utility::Plot::ScrollingBuffer::AddPoint(float x, float y)
{
    if (Data.size() < MaxSize)
        Data.push_back(ImVec2(x, y));
    else
    {
        Data[Offset] = ImVec2(x, y);
        Offset = (Offset + 1) % MaxSize;
    }
}

void ECellEngine::Editor::Utility::Plot::ScrollingBuffer::Erase()
{
    if (Data.size() > 0)
    {
        Data.shrink(0);
        Offset = 0;
    }
}

void ECellEngine::Editor::Utility::Plot::ScrollingBuffer::Reset()
{
    Data.shrink(0);
    Offset = 0;
    Data.reserve(MaxSize);
}