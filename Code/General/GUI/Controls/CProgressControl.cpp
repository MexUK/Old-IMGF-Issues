#include "CProgressControl.h"
#include "GUI/Window/CWindow.h"
#include "String/CStringUtility.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

using namespace std;

auto pOnRender_Progress		= [](void *pControl) { ((CProgressControl*) pControl)->render(); };

// event binding
void			CProgressControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Progress, this));
}

// render
void			CProgressControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	CVector2i32 vecProgressPosition = getProgressPosition(); // todo
	pGFX->drawRectangle(getPosition(), getSize(), getStyles());
	pGFX->drawRectangle(getPosition(), CVector2ui32(vecProgressPosition.m_x, vecProgressPosition.m_y), getStyles());
	pGFX->drawText(getPosition(), getSize(), getProgressPercentText(), getStyles());
}

// ratio
float32			CProgressControl::getProgress(void)
{
	return ((float32) getCurrentTicks()) / ((float32) getMaxTicks());
}

// text
string			CProgressControl::getProgressPercentText(void)
{
	return CStringUtility::toString(getProgress() * 100.0f) + "%";
}

// position
CVector2i32		CProgressControl::getProgressPosition(void)
{
	return CVector2i32(getProgressPositionX(), getSize().m_y);
}

int32			CProgressControl::getProgressPositionX(void)
{
	return ((float32) getSize().m_x) * getProgress();
}