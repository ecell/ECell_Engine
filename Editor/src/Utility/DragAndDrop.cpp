#include "Utility/DragAndDrop.hpp"

char ECellEngine::Editor::Utility::DragAndDrop::payloadID[32] = { 0 };
const void* ECellEngine::Editor::Utility::DragAndDrop::payloadData = nullptr;
bool ECellEngine::Editor::Utility::DragAndDrop::payloadCharged = false;