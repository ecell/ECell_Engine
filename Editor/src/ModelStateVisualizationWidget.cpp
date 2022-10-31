#include "ModelStateVisualizationWidget.hpp"

void ECellEngine::Editor::ModelStateVisualizationWidget::draw()
{
    ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);

    ImGui::Begin(widgetNameID);

    //Draw Tab Generation Combo
    static int visualOptionIdx = 0;
    ImGui::Text("Visualization Tabs");
    ImGui::SameLine();
    if (ImGui::BeginCombo("##VisualizationTabs", visualizationOptions[visualOptionIdx]))
    {
        for (int i = 0; i < 2; i++)
        {
            const bool is_selected = (visualOptionIdx == i);
            if (ImGui::Selectable(visualizationOptions[i], is_selected))
            {
                visualOptionIdx = i;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (simulationLoop->getSimulationEnvironment()->isInitialized())
    {
        //We can start adding tabs only if a simulation environement was initialized.
        ImGui::SameLine();

        if (ImGui::Button("Add Tab"))
        {
            switch (visualOptionIdx)
            {
            case (0):
                lpTabsData.push_back(LinePlotTabData((short)lpTabsData.size()));
                break;
            case (1):
                qtTabsData.push_back(TabData((short)qtTabsData.size()));
                break;
            }
        }

        //Whenever the number of species in the model state changes
        if (nbSpecies != simulationLoop->getSimulationEnvironment()->getQuantities()->size())
        {
            nbSpecies = simulationLoop->getSimulationEnvironment()->getQuantities()->size();
            astEvaluator = simulationLoop->getSimulationEnvironment()->astEvaluator.get();
            
            //Copy the names in the dedicated buffer
            speciesNamesBuffer.clear();
            speciesNamesBuffer.reserve(nbSpecies);
            for (int i = 0; i < 2 * nbSpecies; i += 2)
            {
                speciesNamesBuffer.push_back(astEvaluator->getNode(i)->getNameEx());
            }
        }

        ImGui::Separator();
    }
    else
    {
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "No simulation environment has been set yet.");
    }

    static ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton 
        | ImGuiTabBarFlags_FittingPolicyScroll;
    if (ImGui::BeginTabBar("##TabBar", tabBarFlags))
    {
        for(std::vector<LinePlotTabData>::iterator it = lpTabsData.begin(); it != lpTabsData.end(); it++)
        {
            drawLinePlotTab(&(*it));
        }

        for (std::vector<TabData>::iterator it = qtTabsData.begin(); it != qtTabsData.end(); it++)
        {
            drawQuantityTableTab(&(*it));
        }
        ImGui::EndTabBar();
    }
    
    ImGui::End();
}

void ECellEngine::Editor::ModelStateVisualizationWidget::drawLinePlotTab(LinePlotTabData* _lpTabData)
{
    if (ImGui::BeginTabItem(_lpTabData->name))
    {
        ImGui::BulletText("This example assumes 60 FPS. Higher FPS requires larger buffer size.");
        ImGui::Text("dataIdxToPlotOnX: %1d; dataIdxToPlotOnY: %1d;", _lpTabData->dataIdxToPlotOnX, _lpTabData->dataIdxToPlotOnY);
        ImGui::Text("elapsedTime: %3f; deltaTime: %f;", simulationLoop->simulationTimer.elapsedTime, simulationLoop->simulationTimer.deltaTime);
        ImGui::Text("x: %3f; y: %f;", _lpTabData->x, _lpTabData->y);

        if (ImGui::CollapsingHeader("X axis"))
        {
            ImGui::Checkbox("Plot time on X", &_lpTabData->timeOnX);
            
            if (_lpTabData->timeOnX)
            {
                _lpTabData->xAxisGraphicsData.label = "Time (sec)";

                //Disable the Combo to chose species if we decide to map time
                ImGui::BeginDisabled();
            }

            // Combos to chose what species to plot on X
            if (ImGui::BeginCombo("Species to map on X axis", speciesNamesBuffer[_lpTabData->dataIdxToPlotOnX].c_str()))
            {
                for (int i = 0; i < nbSpecies; i++)
                {
                    const bool is_selected = (_lpTabData->dataIdxToPlotOnX == i);
                    if (ImGui::Selectable(speciesNamesBuffer[i].c_str(), is_selected))
                    {
                        _lpTabData->dataIdxToPlotOnX = i;
                        _lpTabData->xAxisGraphicsData.label = speciesNamesBuffer[i].c_str();
                        _lpTabData->buffer.Reset();
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            if (_lpTabData->timeOnX)
            {
                ImGui::EndDisabled();
            }

            if (ImGui::TreeNode("Parameters##X"))
            {
                ImGui::SliderFloat("Low tolerance##X", &_lpTabData->xAxisGraphicsData.lowTolerance, 0.f, 1.f);
                ImGui::SliderFloat("Up tolerance##X", &_lpTabData->xAxisGraphicsData.upTolerance, 0.f, 1.f);
                ImGui::SliderFloat("Low rescale threshold##X", &_lpTabData->xAxisGraphicsData.axisRescaleLowThreshold, 0.f, _lpTabData->xAxisGraphicsData.axisRescaleUpThreshold - 0.001f);
                ImGui::SliderFloat("Up rescale threshold##X", &_lpTabData->xAxisGraphicsData.axisRescaleUpThreshold, _lpTabData->xAxisGraphicsData.axisRescaleLowThreshold + 0.001f, 1.f);

                ImGui::TreePop();
            }
        }

        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Y axis"))
        {
            ImGui::Checkbox("Plot time on Y", &_lpTabData->timeOnY);

            if (_lpTabData->timeOnY)
            {
                _lpTabData->yAxisGraphicsData.label = "Time (sec)";

                //Disable the Combo to chose species if we decide to map time
                ImGui::BeginDisabled();
            }

            // Combos to chose what species to plot on Y
            if (ImGui::BeginCombo("Species to map on Y axis", speciesNamesBuffer[_lpTabData->dataIdxToPlotOnY].c_str()))
            {
                for (int i = 0; i < nbSpecies; i++)
                {
                    const bool is_selected = (_lpTabData->dataIdxToPlotOnY == i);
                    if (ImGui::Selectable(speciesNamesBuffer[i].c_str(), is_selected))
                    {
                        _lpTabData->dataIdxToPlotOnY = i;
                        _lpTabData->yAxisGraphicsData.label = speciesNamesBuffer[i].c_str();
                        _lpTabData->buffer.Reset();
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            if (_lpTabData->timeOnY)
            {
                ImGui::EndDisabled();
            }

            if (ImGui::TreeNode("Parameters##Y"))
            {
                ImGui::SliderFloat("Low tolerance##Y", &_lpTabData->yAxisGraphicsData.lowTolerance, 0.f, 1.f);
                ImGui::SliderFloat("Up tolerance##Y", &_lpTabData->yAxisGraphicsData.upTolerance, 0.f, 1.f);
                ImGui::SliderFloat("Low rescale threshold##Y", &_lpTabData->yAxisGraphicsData.axisRescaleLowThreshold, 0.f, _lpTabData->yAxisGraphicsData.axisRescaleUpThreshold - 0.001f);
                ImGui::SliderFloat("Up rescale threshold##Y", &_lpTabData->yAxisGraphicsData.axisRescaleUpThreshold, _lpTabData->yAxisGraphicsData.axisRescaleLowThreshold + 0.001f, 1.f);

                ImGui::TreePop();
            }
        }        

        _lpTabData->x = simulationLoop->simulationTimer.elapsedTime;
        if (!_lpTabData->timeOnX)
        {
            _lpTabData->x = astEvaluator->getNamedNodeValue(2*_lpTabData->dataIdxToPlotOnX);
        }

        _lpTabData->y = simulationLoop->simulationTimer.elapsedTime;
        if (!_lpTabData->timeOnY)
        {
            _lpTabData->y = astEvaluator->getNamedNodeValue(2*_lpTabData->dataIdxToPlotOnY);
        }

        _lpTabData->bufferAddPoint();

        if (ImPlot::BeginPlot("Plot", ImVec2(530, 530), ImPlotFlags_NoLegend))
        {
            ImPlot::SetupAxes(_lpTabData->xAxisGraphicsData.label, _lpTabData->yAxisGraphicsData.label);

            if (_lpTabData->rescaleXAxis())
            {
                ImPlot::SetupAxisLimits(ImAxis_X1,
                    _lpTabData->getXAxisLowLimit(),
                    _lpTabData->getXAxisUpLimit(), ImGuiCond_Always);
            }

            if (_lpTabData->rescaleYAxis())
            {
                ImPlot::SetupAxisLimits(ImAxis_Y1,
                    _lpTabData->getYAxisLowLimit(),
                    _lpTabData->getYAxisUpLimit(), ImGuiCond_Always);
            }       

            ImPlot::TagX(_lpTabData->xAxisGraphicsData.getLowThresholdedValue(), ImVec4(1, 1, 0, 1));
            ImPlot::TagX(_lpTabData->xAxisGraphicsData.getUpThresholdedValue(), ImVec4(1, 1, 0, 1));
            ImPlot::TagY(_lpTabData->yAxisGraphicsData.getLowThresholdedValue(), ImVec4(1, 1, 0, 1));
            ImPlot::TagY(_lpTabData->yAxisGraphicsData.getUpThresholdedValue(), ImVec4(1, 1, 0, 1));

            ImPlot::PlotLine("data", &_lpTabData->buffer.Data[0].x, &_lpTabData->buffer.Data[0].y, _lpTabData->buffer.Data.size(), 0, 2 * sizeof(float));
            ImPlot::EndPlot();
        }
        ImGui::EndTabItem();
    }
}


void ECellEngine::Editor::ModelStateVisualizationWidget::drawQuantityTableTab(TabData* _tabData)
{
    if (ImGui::BeginTabItem(_tabData->name))
    {
        const ImGuiTableFlags flags =
            ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable
            | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY;

        if (ImGui::BeginTable("Loaded SBML Documents", 2, flags))
        {
            ImGui::TableSetupColumn("Species Id");
            ImGui::TableSetupColumn("Quantity");
            ImGui::TableHeadersRow();
            ImGui::TableNextRow();
            for (int i = 0; i < nbSpecies; i++)
            {
                ImGui::TableNextColumn();
                ImGui::Text(speciesNamesBuffer[i].c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%2f", astEvaluator->getNamedNodeValue(2 * i));
                ImGui::TableNextRow();
            }

            ImGui::EndTable();
        }
        ImGui::EndTabItem();
    }
}