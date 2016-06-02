#include "CMenuControl.h"
#include "GUI/Window/CWindow.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/eEvent.h"

auto pOnRender_Menu		= [](void *pControl) { ((CMenuControl*) pControl)->render(); };

// event binding
void					CMenuControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Menu, this));
}

// render
void					CMenuControl::render(void)
{
}