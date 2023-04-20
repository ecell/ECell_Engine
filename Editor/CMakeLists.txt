# ~/Editor/CMakeLists.txt

cmake_minimum_required (VERSION 3.12)


find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(implot CONFIG REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

add_executable( ECellEngineEditor
"src/imgui_impl_glfw.cpp"
"src/imgui_impl_vulkan.cpp"

"src/imgui-node-editor/crude_json.cpp"
"src/imgui-node-editor/imgui_canvas.cpp"
"src/imgui-node-editor/imgui_node_editor.cpp"
"src/imgui-node-editor/imgui_node_editor_api.cpp"

"src/Main.cpp"
"src/BackendUtility.cpp"
"src/ConsoleWidget.cpp"
"src/Editor.cpp"
"src/EditorConsoleLoggerSink.cpp"
#"src/EngineDataVisualizationWidget.cpp"
#"src/FileIOWidget.cpp"
#"src/ModelStateVisualizationWidget.cpp"
"src/MainWindow.cpp"
"src/ModelExplorerWidget.cpp"
"src/ModelHierarchyWidget.cpp"
"src/ModelNodeBasedViewerContext.cpp"
"src/ModelNodeBasedViewerGlobal.cpp"
"src/ModelNodeBasedViewerWidget.cpp"
"src/NodeEditorData.cpp"
"src/NodeEditorDraw.cpp"
"src/OptionsWidget.cpp"
"src/SimulationFlowControlWidget.cpp"
)

target_link_libraries(ECellEngineEditor PRIVATE ECellEngine)
target_link_libraries(ECellEngineEditor PRIVATE imgui::imgui)
target_link_libraries(ECellEngineEditor PRIVATE glfw)
target_link_libraries(ECellEngineEditor PRIVATE Vulkan::Vulkan)
target_link_libraries(ECellEngineEditor PRIVATE implot::implot)

target_include_directories(ECellEngineEditor
							PRIVATE
								"./headers"
								"./headers/imgui-node-editor"
								"../ECell_Engine/engine_system_files"
								"../ECell_Engine/headers"
)

# Better font files for the Node Editor (but it will be applied everywhere).
# Originally available at: https://github.com/thedmd/imgui-node-editor/tree/master/examples/data
configure_file("./data/Play-Regular.ttf" "${CMAKE_BINARY_DIR}/bin/data/Play-Regular.ttf" COPYONLY)
configure_file("./data/Cuprum-Bold.ttf" "${CMAKE_BINARY_DIR}/bin/data/Cuprum-Bold.ttf" COPYONLY)