#include "Editor.hpp"//forward declaration initiated in the  base class "Widget"

void ECellEngine::Editor::ModelExplorerWidget::Draw()
{
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
    /*if (ImGui::Button("Add Module"))
    {
        addModuleCommandArray[1] = "C:/Users/eliot/Documents/Source/External/Papers/Hat-et-al_p53_BNG/S2_Code_BioNetGen/export/p53_L3V2_mod2.xml";
        editor.engine.GetCommandsManager()->interpretCommand(addModuleCommandArray);
    }*/
    
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ImGui::Separator();

    ax::NodeEditor::SetCurrentEditor(nodeEditorCtxt);

    // Start interaction with editor.
    ax::NodeEditor::Begin("Model Exploration Space", ImVec2(0.0, 0.0f));

    int uniqueId = 1;

    //
    // 1) Commit known data to editor
    //
    const char* inPinNames[1] {"In 1"};
    const char* outPinNames[1] {"Out 1"};
    ECellEngine::Editor::Utility::NodeEditorUtility::DrawNode(uniqueId, "Node A", 1, inPinNames, 1, outPinNames);

    const char* inPinNames2[2] {"In 1", "In 2"};
    ECellEngine::Editor::Utility::NodeEditorUtility::DrawNode(uniqueId, "Node B", 2, inPinNames2, 1, outPinNames);


    // Submit Links
    for (ECellEngine::Editor::Utility::LinkInfo& linkInfo : links)
    {
        ax::NodeEditor::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);
    }

    //
    // 2) Handle interactions
    //

    // Handle creation action, returns true if editor want to create new object (node or link)
    ECellEngine::Editor::Utility::NodeEditorUtility::LinkCreation(nextLinkId, links);


    // Handle deletion action
    ECellEngine::Editor::Utility::NodeEditorUtility::LinkDestruction(links);


    // End of interaction with editor.
    ax::NodeEditor::End();
    ax::NodeEditor::SetCurrentEditor(nullptr);
}