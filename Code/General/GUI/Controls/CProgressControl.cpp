#include "CProgressControl.h"
#include "GUI/Window/CWindow.h"
#include "String/CStringUtility.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

using namespace std;

auto pOnRender_Progress		= [](void *pControl) { ((CProgressControl*) pControl)->render(); };

CProgressControl::CProgressControl(void) :
	CGUIControl(GUI_CONTROL_PROGRESS),
	m_uiMaxTicks(0),
	m_uiCurrentTicks(0),
	m_bCompletionPercentageShown(false),
	m_bTextShown(false)
{
}

// event binding
void			CProgressControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Progress, this)); // todo - change to like CGUIControl::bindEvent(eEventId, func) which stores it
}

// render
void			CProgressControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	// todo - getStyles()->setComponentType

	//getStyles()->setComponentType("default");
	pGFX->drawRectangle(getPosition(), getSize(), getStyles());

	//getStyles()->setComponentType("seek");
	CPoint2D vecProgressPosition = getProgressPosition(); // todo
	// todo getStyles()->setStyleNameOverwrite("fill-colour", "progress-bar-fill-colour");
	pGFX->drawRectangle(getPosition(), CSize2D(vecProgressPosition.m_x, vecProgressPosition.m_y), getStyles());
	// todo getStyles()->restoreStyleNameOverwrites();

	if (isTextShown())
	{
		checkToRecalculateStringSize(getStyles());

		//getStyles()->setComponentType("text");
		pGFX->drawText(getPosition(), getSize(), getProgressPercentText(), getStyles());
	}

	//getStyles()->resetComponentType();
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
CPoint2D		CProgressControl::getProgressPosition(void)
{
	return CPoint2D(getProgressPositionX(), getSize().m_y);
}

int32			CProgressControl::getProgressPositionX(void)
{
	return ((float32) getSize().m_x) * getProgress();
}