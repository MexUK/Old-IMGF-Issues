#include "CTextControl.h"
#include "GUI/Window/CWindow.h"
#include "Event/eEvent.h"
#include "GDIPlus/CGDIPlusUtility.h"

auto pOnRender_Text			= [](void *pControl) { ((CTextControl*) pControl)->render(); };

// event binding
void					CTextControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Text, this));
}

// render
void					CTextControl::render(void)
{
	CGDIPlusUtility::drawText(getPosition(), getSize(), getText(), getTextColour(), getFontSize(), isBold());
}