#include "Widget/MNBV/ModelNodeBasedViewerContext.hpp"

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::ConserveLinkDataIntegrity()
{
	for (std::vector<Utility::MNBV::LinkData>::iterator it = dynamicLinks.begin(); it != dynamicLinks.end(); it++)
	{
		it->Refresh();
	}
}