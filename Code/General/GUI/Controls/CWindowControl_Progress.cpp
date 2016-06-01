#include "CWindowControl_Progress.h"
#include "GUI/Window/CWindow.h"
#include "String/CStringUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/eEvent.h"

using namespace std;

auto pOnRender_Progress		= [](void *pControl) { ((CWindowControl_Progress*) pControl)->render(); };

// event binding
void					CWindowControl_Progress::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Progress, this));
}

// render
void			CWindowControl_Progress::render(void)
{
	CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	CGDIPlusUtility::drawRectangleFill(getPosition(), getProgressPosition(), getProgressFillColour());
	CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	CGDIPlusUtility::drawText(getPosition(), getSize(), getProgressPercentText(), getTextColour(), getFontSize(), isBold());
}

// ratio
float32			CWindowControl_Progress::getProgress(void)
{
	return ((float32) getCurrentTicks()) / ((float32) getMaxTicks());
}

// text
string		CWindowControl_Progress::getProgressPercentText(void)
{
	return CStringUtility::toString(getProgress() * 100.0f) + "%";
}

// position
CVector2ui32	CWindowControl_Progress::getProgressPosition(void)
{
	return CVector2ui32(getProgressPositionX(), getSize().m_y);
}

uint32			CWindowControl_Progress::getProgressPositionX(void)
{
	return ((float32) getSize().m_x) * getProgress();
}