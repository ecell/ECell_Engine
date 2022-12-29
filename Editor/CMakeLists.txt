# ~/Editor/CMakeLists.txt

cmake_minimum_required (VERSION 3.12)


find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(implot CONFIG REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

add_executable( ECellEngineEditor
"src/Main.cpp"	
"src/imgui_impl_glfw.cpp"
"src/imgui_impl_vulkan.cpp"
"src/Editor.cpp"
"src/EditorLog.cpp"
#"src/EngineDataVisualizationWidget.cpp"
#"src/FileIOWidget.cpp"
#"src/ModelStateVisualizationWidget.cpp"
"src/LogWidget.cpp"
"src/OptionsWidget.cpp"
#"src/SimulationFlowControlWidget.cpp"
  )

target_link_libraries(ECellEngineEditor PRIVATE ECellEngine)
target_link_libraries(ECellEngineEditor PRIVATE imgui::imgui)
target_link_libraries(ECellEngineEditor PRIVATE glfw)
target_link_libraries(ECellEngineEditor PRIVATE Vulkan::Vulkan)
target_link_libraries(ECellEngineEditor PRIVATE implot::implot)

target_include_directories(ECellEngineEditor
							PRIVATE
								"./headers"
								"../ECell_Engine/engine_system_files"
								"../ECell_Engine/headers"
)