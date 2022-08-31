#pragma once

#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

//#define IMGUI_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

namespace ECellEngine::Editor::Utility
{
    static void checkVkResult(VkResult err)
    {
        if (err == 0)
            return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        if (err < 0)
            abort();
    }

#ifdef IMGUI_VULKAN_DEBUG_REPORT
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode,
        const char* pLayerPrefix, const char* pMessage, void* pUserData)
    {
        (void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
        fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
        return VK_FALSE;
    }
#endif // IMGUI_VULKAN_DEBUG_REPORT

    static void glfwErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }
}