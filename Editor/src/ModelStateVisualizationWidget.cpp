#include "ModelStateVisualizationWidget.hpp"

void ECellEngine::Editor::ModelStateVisualizationWidget::draw()
{
    ImGui::SetNextWindowSize(ImVec2(530, 300), ImGuiCond_FirstUseEver);

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

void ECellEngine::Editor::ModelStateVisualizationWidget::drawLinePlot()
{
    ImGui::BulletText("Move your mouse to change the data!");
    ImGui::BulletText("This example assumes 60 FPS. Higher FPS requires larger buffer size.");
    static RollingBuffer   rdata1, rdata2;

    rdata1.AddPoint(simulationLoop->simulationTimer.elapsedTime, 0);
    rdata2.AddPoint(simulationLoop->simulationTimer.elapsedTime, 0);

    static float history = 10.0f;
    rdata1.Span = 10.0f;
    rdata2.Span = 10.0f;

    if (ImPlot::BeginPlot("##Rolling"))
    {
        ImPlot::SetupAxes("Time", "Quantity");
        ImPlot::SetupAxisLimits(ImAxis_X1, 0, history, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);

        ImPlot::PlotLine("Mouse X", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 2 * sizeof(float));
        ImPlot::PlotLine("Mouse Y", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 2 * sizeof(float));
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