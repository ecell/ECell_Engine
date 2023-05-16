# ~/Editor/CMakeLists.txt

cmake_minimum_required (VERSION 3.12)


find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(implot CONFIG REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

add_executable( ECellEngineEditor
"src/imgui/imgui_impl_glfw.cpp"
"src/imgui/imgui_impl_vulkan.cpp"

"src/imgui-node-editor/crude_json.cpp"
"src/imgui-node-editor/imgui_canvas.cpp"
"src/imgui-node-editor/imgui_node_editor.cpp"
"src/imgui-node-editor/imgui_node_editor_api.cpp"

"src/Logging/EditorConsoleLoggerSink.cpp"

"src/Main.cpp"
"src/Editor.cpp"

#"src/Widget/EngineDataVisualizationWidget.cpp"
#"src/Widget/FileIOWidget.cpp"
#"src/Widget/ModelStateVisualizationWidget.cpp"
"src/Widget/ConsoleWidget.cpp"
"src/Widget/MainWindow.cpp"
"src/Widget/ModelExplorerWidget.cpp"
"src/Widget/ModelHierarchyWidget.cpp"
"src/Widget/OptionsWidget.cpp"
"src/Widget/SimulationFlowControlWidget.cpp"

"src/Utility/BackendUtility.cpp"
"src/Utility/NodeEditorData.cpp"
"src/Utility/NodeEditorDraw.cpp"

"src/Widget/MNBV/ModelNodeBasedViewerContext.cpp"
"src/Widget/MNBV/ModelNodeBasedViewerGlobal.cpp"
"src/Widget/MNBV/ModelNodeBasedViewerWidget.cpp"
)

if (MSVC)

    target_compile_options(ECellEngineEditor PRIVATE /MP)	# Multithread compilation

endif()

target_link_libraries(ECellEngineEditor PRIVATE ECellEngine)
target_link_libraries(ECellEngineEditor PRIVATE imgui::imgui)
target_link_libraries(ECellEngineEditor PRIVATE glfw)
target_link_libraries(ECellEngineEditor PRIVATE Vulkan::Vulkan)
target_link_libraries(ECellEngineEditor PRIVATE implot::implot)

target_include_directories(ECellEngineEditor
							PRIVATE
								"./headers"
								"./headers/imgui-node-editor"
								"./headers/imgui"
								"../ECell_Engine/engine_system_files"
								"../ECell_Engine/headers"
)

# Better font files for the Node Editor (but it will be applied everywhere).
# Originally available at: https://github.com/thedmd/imgui-node-editor/tree/master/examples/data
configure_file("./data/Play-Regular.ttf" "${CMAKE_BINARY_DIR}/bin/data/Play-Regular.ttf" COPYONLY)
configure_file("./data/Cuprum-Bold.ttf" "${CMAKE_BINARY_DIR}/bin/data/Cuprum-Bold.ttf" COPYONLY)