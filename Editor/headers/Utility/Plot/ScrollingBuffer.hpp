#pragma once

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui.h"

namespace ECellEngine::Editor::Utility::Plot
{
    /*!
    @brief Utility structure for realtime plot
    @remark Copied from implot_demo.cpp
    @remark Added the ::Reset function.
    */
    struct ScrollingBuffer
    {
        int MaxSize;//The capacity of the buffer.
        int Offset;//Offset of the buffer when adding point after the ::MaxSize has been reached.
        ImVector<ImVec2> Data;//The data points.
        ScrollingBuffer(int max_size = 2000)
        {
            MaxSize = max_size;
            Offset = 0;
            Data.reserve(MaxSize);
        }

        ScrollingBuffer(const ScrollingBuffer& _sb) :
            MaxSize{ _sb.MaxSize }, Offset{ _sb.Offset }, Data{ _sb.Data }
        {

        }

        /*!
        @brief Adds a point to the buffer.
        */
        void AddPoint(float x, float y);

        /*!
        @brief Erases the buffer.
        */
        void Erase();

        /*!
        @brief Erases the buffer and reserve space to ::MaxSize
        */
        void Reset();
    };
}