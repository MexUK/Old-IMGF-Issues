#include "CMenuControl.h"
#include "GUI/Window/CWindow.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

auto pOnRender_Menu		= [](void *pControl) { ((CMenuControl*) pControl)->render(); };

CMenuControl::CMenuControl(void) :
	CGUIControl(GUI_CONTROL_MENU)
{
}

// event binding
void					CMenuControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Menu, this));
}

// render
void					CMenuControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();


}