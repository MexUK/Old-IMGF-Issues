#include "CWindowControl_Progress.h"
#include "GUI/Window/CWindow.h"
#include "String/CStringUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/eEvent.h"

using namespace std;

auto pOnRender_Progress		= [](void *pControl) { ((CWindowControl_Progress*) pControl)->render(); };

// event binding
void			CWindowControl_Progress::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Progress, this));
}

// render
void			CWindowControl_Progress::render(void)
{
	CVector2i32 vecProgressPosition = getProgressPosition(); // todo
	CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	CGDIPlusUtility::drawRectangleFill(getPosition(), CVector2ui32(vecProgressPosition.m_x, vecProgressPosition.m_y), getProgressFillColour());
	CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	CGDIPlusUtility::drawText(getPosition(), getSize(), getProgressPercentText(), getTextColour(), getFontSize(), isBold());
}

// ratio
float32			CWindowControl_Progress::getProgress(void)
{
	return ((float32) getCurrentTicks()) / ((float32) getMaxTicks());
}

// text
string			CWindowControl_Progress::getProgressPercentText(void)
{
	return CStringUtility::toString(getProgress() * 100.0f) + "%";
}

// position
CVector2i32		CWindowControl_Progress::getProgressPosition(void)
{
	return CVector2i32(getProgressPositionX(), getSize().m_y);
}

int32			CWindowControl_Progress::getProgressPositionX(void)
{
	return ((float32) getSize().m_x) * getProgress();
}