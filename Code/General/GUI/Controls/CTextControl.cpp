#include "CTextControl.h"
#include "GUI/Window/CWindow.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

auto pOnRender_Text			= [](void *pControl) { ((CTextControl*) pControl)->render(); };

// event binding
void					CTextControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Text, this));
}

// render
void					CTextControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	pGFX->drawText(getPosition(), getSize(), getText(), getStyles());
}