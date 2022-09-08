#include "ModelStateVisualizationWidget.hpp"

// utility structure for realtime plot
struct RollingBuffer
{
    float Span;
    ImVector<ImVec2> Data;
    RollingBuffer()
    {
        Span = 10.0f;
        Data.reserve(2000);
    }
    void AddPoint(float x, float y)
    {
        float xmod = fmodf(x, Span);
        if (!Data.empty() && xmod < Data.back().x)
            Data.shrink(0);
        Data.push_back(ImVec2(xmod, y));
    }
};

// utility structure for realtime plot
struct ScrollingBuffer
{
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000)
    {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y)
    {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else
        {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase()
    {
        if (Data.size() > 0)
        {
            Data.shrink(0);
            Offset = 0;
        }
    }
    void Reset()
    {
        Data.shrink(0);
        Offset = 0;
        Data.reserve(MaxSize);
    }
};

void ECellEngine::Editor::ModelStateVisualizationWidget::draw()
{
    ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);

    ImGui::Begin("Model State Visualization");

    if (simulationLoop->getSimulationEnvironment()->isInitialized())
    {
        nbSpecies = simulationLoop->getSimulationEnvironment()->getQuantities()->size();
        astEvaluator = simulationLoop->getSimulationEnvironment()->astEvaluator;

        ImGui::TextColored(ImVec4(0, 1, 0, 1), "A simulation environment has been set.");
        if (ImGui::BeginTabBar("Visualization"))
        {
            if (ImGui::BeginTabItem("String"))
            {
                drawRawString();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Table"))
            {
                drawTable();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("LinePlot"))
            {
                drawLinePlot();
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
    else
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "No simulation environment has been set yet.");
    }
    
    ImGui::End();
}

void ECellEngine::Editor::ModelStateVisualizationWidget::drawLinePlot()
{
    static ImGuiTableFlags flags =
         ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable
        | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY;

    static float x, y;
    static ScrollingBuffer   buffer;
    static std::string labelX = "time (sec)";
    static std::string labelY = "time (sec)";

    ImGui::BulletText("This example assumes 60 FPS. Higher FPS requires larger buffer size.");
    ImGui::Text("dataIdxToPlotOnX: %1d; dataIdxToPlotOnY: %1d;", dataIdxToPlotOnX, dataIdxToPlotOnY);
    ImGui::Text("elapsedTime: %3f; deltaTime: %f;", simulationLoop->simulationTimer.elapsedTime, simulationLoop->simulationTimer.deltaTime);
    ImGui::Text("x: %3f; y: %f;", x, y);

    if (ImGui::BeginTable("Choose the data to plot against each other", 2, flags, ImVec2(150,300)))
    {
        ImGui::TableSetupColumn("Data to plot on X");
        ImGui::TableSetupColumn("Data to plot on Y");
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        for (int i = 0; i < 2 * nbSpecies; i += 2)
        {
            ImGui::TableNextColumn();
            /*if (ImGui::Button(astEvaluator->getNode(i)->getNameEx().c_str()))
            {
                dataIdxToPlotOnX = i;
            }*/
            ImGui::Text(astEvaluator->getNode(i)->getNameEx().c_str());
            ImGui::TableNextColumn();
            if (ImGui::Button(astEvaluator->getNode(i)->getNameEx().c_str()))
            {
                dataIdxToPlotOnY = i;
                buffer.Reset();
            }
            ImGui::TableNextRow();
        }
        ImGui::EndTable();
    }

    ImGui::SameLine();

    
    x = simulationLoop->simulationTimer.elapsedTime;
    if (dataIdxToPlotOnX > -1)
    {
        labelX = astEvaluator->getNode(dataIdxToPlotOnX)->getNameEx();
        x = astEvaluator->getNamedNodeValue(dataIdxToPlotOnX);
    }

    y = simulationLoop->simulationTimer.elapsedTime;
    if (dataIdxToPlotOnY > -1)
    {
        labelY = astEvaluator->getNode(dataIdxToPlotOnY)->getNameEx();
        y = astEvaluator->getNamedNodeValue(dataIdxToPlotOnY);
    }

    buffer.AddPoint(x, y);

    static float history = 10.0f;
    //buffer.Span = 10.0f;

    if (ImPlot::BeginPlot("Plot", ImVec2(530, 300), ImPlotFlags_NoLegend))
    {
        ImPlot::SetupAxes(labelX.c_str(), labelY.c_str());
        //ImPlot::SetupAxisLimits(ImAxis_X1, 0, history, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_X1,x - history, x, ImGuiCond_Always);

        //ImPlot::SetupAxisLimits(ImAxis_Y1, 0, history, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 100000);

        ImPlot::PlotLine("data", &buffer.Data[0].x, &buffer.Data[0].y, buffer.Data.size(), 0, 2 * sizeof(float));
        ImPlot::EndPlot();
    }
}

void ECellEngine::Editor::ModelStateVisualizationWidget::drawRawString()
{
    ImGui::Text("Legacy Raw string format");
    if (ImGui::Button("Print"))
    {
        engineCmdsManager->interpretCommand(displayCommandArray);
    }
}


void ECellEngine::Editor::ModelStateVisualizationWidget::drawTable()
{
    static ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable
        | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY;

    ImGui::Text("The model state has %1d species", nbSpecies);
    if (ImGui::BeginTable("Loaded SBML Documents", 2, flags))
    {
        ImGui::TableSetupColumn("Species Id");
        ImGui::TableSetupColumn("Quantity");
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        for (int i = 0; i < 2 * nbSpecies; i += 2)
        {
            ImGui::TableNextColumn();
            ImGui::Text(astEvaluator->getNode(i)->getNameEx().c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%2f", astEvaluator->getNamedNodeValue(i));
            ImGui::TableNextRow();
        }

        ImGui::EndTable();
    }
}