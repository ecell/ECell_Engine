#include "Editor.hpp"

void ECellEngine::Editor::Editor::InitializeImGui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    //Enable docking and multi view ports
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking by default
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    RecreateFontAtlas();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
}

void ECellEngine::Editor::Editor::RecreateFontAtlas()
{
    ImGuiIO& io = ImGui::GetIO();

    IM_DELETE(io.Fonts);

    io.Fonts = IM_NEW(ImFontAtlas);

    ImFontConfig config;
    config.OversampleH = 4;
    config.OversampleV = 4;
    config.PixelSnapH = false;

    //m_DefaultFont = io.Fonts->AddFontFromFileTTF("data/Play-Regular.ttf", 18.0f, &config);
    //m_HeaderFont = io.Fonts->AddFontFromFileTTF("data/Cuprum-Bold.ttf", 20.0f, &config);

    io.Fonts->AddFontFromFileTTF("../data/Play-Regular.ttf", 18.0f, &config);
    io.Fonts->AddFontFromFileTTF("../data/Cuprum-Bold.ttf", 20.0f, &config);

    io.Fonts->Build();
}

void ECellEngine::Editor::Editor::Start()
{
    //Start up style and create conxtexts for ImGui
    InitializeImGui();

    //Start glfw & Vulkan
    backend.InitializeEditorWindow();

    //Feed the fonts to Vulkan
    backend.VulkanFontUpload();

    //Start the engine
    engine.Start();
}

void ECellEngine::Editor::Editor::Stop()
{
    engine.Stop();

    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    // Cleanup
    backend.CleanUp();
}

void ECellEngine::Editor::Editor::Update()
{
    // Main loop
    while (!glfwWindowShouldClose(backend.window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        engine.Update(ImGui::GetIO().DeltaTime);

        // Resize swap chain?
        backend.ResizeSwapChain();

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // -- CUSTOM WINDOWS SPACE START --
        
        //edvWidget.draw();
        //fileIOWidget.draw();
        //consoleWidget.Draw();
        //optionsWidget.Draw();
        //simulationWidget.Draw();
        //sfcWidget.draw();
        for (std::vector<Widget*>::iterator it = widgets.begin(); it != widgets.end(); it++)
        {
            (*it)->Draw();
        }

        // Show the big demo window
        // Most of the sample code is in ImGui::ShowDemoWindow()!
        // You can browse its code to learn more about Dear ImGui.
        if (showDemoWindow)
        {
            ImGui::ShowDemoWindow(&showDemoWindow);
            ImPlot::ShowDemoWindow(&showDemoWindow);
        }

        // -- CUSTOM WINDOWS SPACE END --

        // Rendering
        ImGui::Render();
        ImDrawData* main_draw_data = ImGui::GetDrawData();
        const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
        
        if (!main_is_minimized)
        {
            backend.FrameRender(main_draw_data);
        }

        // Update and Render additional Platform Windows
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        // Present Main Platform Window
        if (!main_is_minimized)
        {
            backend.FramePresent();
        }
    }
}