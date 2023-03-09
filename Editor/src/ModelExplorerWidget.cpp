#include "Editor.hpp"//forward declaration initiated in the  base class "Widget"

void ECellEngine::Editor::ModelExplorerWidget::Draw()
{
    //ImGui::SetNextWindowSize(ImVec2(900, 600), ImGuiCond_Once);
    static bool wasdocked = false;
    ImGui::Begin("Model Explorer");
    if (ImGui::IsWindowDocked())
    {
        if (!wasdocked)
        {
            ImGui::DockBuilderSetNodeSize(ImGui::GetWindowDockID(), ImVec2(ImGui::GetMainViewport()->Size.x, ImGui::GetMainViewport()->Size.y * 0.95));
            wasdocked = true;
        }
    }
    else
    {
        wasdocked = false;
    }

    DrawModelExplorer();
    ImGui::End();
}


void ECellEngine::Editor::ModelExplorerWidget::DrawModelExplorer()
{
    static bool firstUse = true;
    /*if (ImGui::Button("Add Module"))
    {
        addModuleCommandArray[1] = "C:/Users/eliot/Documents/Source/External/Papers/Hat-et-al_p53_BNG/S2_Code_BioNetGen/export/p53_L3V2_mod2.xml";
        editor.engine.GetCommandsManager()->interpretCommand(addModuleCommandArray);
    }*/
    
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ImGui::Separator();

    ax::NodeEditor::SetCurrentEditor(m_Context);

    // Start interaction with editor.
    ax::NodeEditor::Begin("Model Exploration Space", ImVec2(0.0, 0.0f));

    int uniqueId = 1;

    //
    // 1) Commit known data to editor
    //

    // Create Ids Node A
    ax::NodeEditor::NodeId nodeA_Id = uniqueId++;
    ax::NodeEditor::PinId  nodeA_InputPinId = uniqueId++;
    ax::NodeEditor::PinId  nodeA_OutputPinId = uniqueId++;

    // Create Ids Node B
    ax::NodeEditor::NodeId nodeB_Id = uniqueId++;
    ax::NodeEditor::PinId  nodeB_InputPinId1 = uniqueId++;
    ax::NodeEditor::PinId  nodeB_InputPinId2 = uniqueId++;
    ax::NodeEditor::PinId  nodeB_OutputPinId = uniqueId++;

    //ECellEngine::Logging::Logger::GetSingleton().LogDebug("Before if: " + std::to_string(firstUse));
    if (firstUse)
    {
        ECellEngine::Logging::Logger::GetSingleton().LogDebug("After if: " + std::to_string(firstUse));
        ax::NodeEditor::SetNodePosition(nodeA_Id, ImVec2(10, 10));
        ax::NodeEditor::SetNodePosition(nodeB_Id, ImVec2(210, 60));
        ax::NodeEditor::NavigateToContent(0.0f);
        firstUse = false;
    }

    ax::NodeEditor::BeginNode(nodeA_Id);
    ImGui::Text("Node A");
    ax::NodeEditor::BeginPin(nodeA_InputPinId, ax::NodeEditor::PinKind::Input);
    ImGui::Text("-> In");
    ax::NodeEditor::EndPin();
    ImGui::SameLine();
    ax::NodeEditor::BeginPin(nodeA_OutputPinId, ax::NodeEditor::PinKind::Output);
    ImGui::Text("Out ->");
    ax::NodeEditor::EndPin();
    ax::NodeEditor::EndNode();

    ax::NodeEditor::BeginNode(nodeB_Id);
    ImGui::Text("Node B");
    ImGuiEx_BeginColumn();
    ax::NodeEditor::BeginPin(nodeB_InputPinId1, ax::NodeEditor::PinKind::Input);
    ImGui::Text("-> In1");
    ax::NodeEditor::EndPin();
    ax::NodeEditor::BeginPin(nodeB_InputPinId2, ax::NodeEditor::PinKind::Input);
    ImGui::Text("-> In2");
    ax::NodeEditor::EndPin();
    ImGuiEx_NextColumn();
    ax::NodeEditor::BeginPin(nodeB_OutputPinId, ax::NodeEditor::PinKind::Output);
    ImGui::Text("Out ->");
    ax::NodeEditor::EndPin();
    ImGuiEx_EndColumn();
    ax::NodeEditor::EndNode();

    // Submit Links
    for (auto& linkInfo : m_Links)
        ax::NodeEditor::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

    //
    // 2) Handle interactions
    //

    // Handle creation action, returns true if editor want to create new object (node or link)
    if (ax::NodeEditor::BeginCreate())
    {
        ax::NodeEditor::PinId inputPinId, outputPinId;
        if (ax::NodeEditor::QueryNewLink(&inputPinId, &outputPinId))
        {
            // QueryNewLink returns true if editor want to create new link between pins.
            //
            // Link can be created only for two valid pins, it is up to you to
            // validate if connection make sense. Editor is happy to make any.
            //
            // Link always goes from input to output. User may choose to drag
            // link from output pin or input pin. This determine which pin ids
            // are valid and which are not:
            //   * input valid, output invalid - user started to drag new ling from input pin
            //   * input invalid, output valid - user started to drag new ling from output pin
            //   * input valid, output valid   - user dragged link over other pin, can be validated

            if (inputPinId && outputPinId) // both are valid, let's accept link
            {
                // ax::NodeEditor::AcceptNewItem() return true when user release mouse button.
                if (ax::NodeEditor::AcceptNewItem())
                {
                    // Since we accepted new link, lets add one to our list of links.
                    m_Links.push_back({ ax::NodeEditor::LinkId(m_NextLinkId++), inputPinId, outputPinId });

                    // Draw new link.
                    ax::NodeEditor::Link(m_Links.back().Id, m_Links.back().InputId, m_Links.back().OutputId);
                }

                // You may choose to reject connection between these nodes
                // by calling ax::NodeEditor::RejectNewItem(). This will allow editor to give
                // visual feedback by changing link thickness and color.
            }
        }
    }
    ax::NodeEditor::EndCreate(); // Wraps up object creation action handling.


    // Handle deletion action
    if (ax::NodeEditor::BeginDelete())
    {
        // There may be many links marked for deletion, let's loop over them.
        ax::NodeEditor::LinkId deletedLinkId;
        while (ax::NodeEditor::QueryDeletedLink(&deletedLinkId))
        {
            // If you agree that link can be deleted, accept deletion.
            if (ax::NodeEditor::AcceptDeletedItem())
            {
                // Then remove link from your data.
                for (auto& link : m_Links)
                {
                    if (link.Id == deletedLinkId)
                    {
                        m_Links.erase(&link);
                        break;
                    }
                }
            }

            // You may reject link deletion by calling:
            // ax::NodeEditor::RejectDeletedItem();
        }
    }
    ax::NodeEditor::EndDelete(); // Wrap up deletion action

    // End of interaction with editor.
    ax::NodeEditor::End();
    ax::NodeEditor::SetCurrentEditor(nullptr);
}

//
//void ECellEngine::Editor::ModelExplorerWidget::DrawSimulationFlowControls()
//{
//    ImGui::Text("This is Simulation Workflow Control.");
//}