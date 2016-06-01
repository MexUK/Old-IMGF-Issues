#include "CWindowControl_Menu.h"
#include "GUI/Window/CWindow.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/eEvent.h"

auto pOnRender_Menu		= [](void *pControl) { ((CWindowControl_Menu*) pControl)->render(); };

// event binding
void					CWindowControl_Menu::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Menu, this));
}

// render
void					CWindowControl_Menu::render(void)
{
}