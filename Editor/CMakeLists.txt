# ~/Editor/CMakeLists.txt

cmake_minimum_required (VERSION 3.12)


find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

add_executable( ECellEngineEditor
"Main.cpp"
"Main.hpp"
"CommandsManager.cpp"				
"CommandsManager.hpp"				
"imgui_impl_glfw.cpp"
"imgui_impl_glfw.h"
"imgui_impl_vulkan.cpp"
"imgui_impl_vulkan.h"
"Editor.cpp"
"Editor.hpp"
"EditorUtility.hpp"
"Windows.cpp"
"Windows.hpp")

target_link_libraries(ECellEngineEditor PRIVATE ECellEngine)
target_link_libraries(ECellEngineEditor PRIVATE imgui::imgui)
target_link_libraries(ECellEngineEditor PRIVATE glfw)
target_link_libraries(ECellEngineEditor PRIVATE Vulkan::Vulkan)

target_include_directories(ECellEngineEditor
							PRIVATE
								"./"
								"../ECell_Engine/engine_system_files"
								"../ECell_Engine/headers"
)