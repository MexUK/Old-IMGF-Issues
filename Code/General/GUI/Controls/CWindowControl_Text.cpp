#include "CWindowControl_Text.h"
#include "GUI/Window/CWindow.h"
#include "Event/eEvent.h"
#include "GDIPlus/CGDIPlusUtility.h"

auto pOnRender_Text			= [](void *pControl) { ((CWindowControl_Text*) pControl)->render(); };

// event binding
void					CWindowControl_Text::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Text, this));
}

// render
void					CWindowControl_Text::render(void)
{
	CGDIPlusUtility::drawText(getPosition(), getSize(), getText(), getTextColour(), getFontSize(), isBold());
}