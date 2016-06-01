#include "CWindowControl_Check.h"
#include "Math/CMathUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/Window/CWindow.h"

// input
void		CWindowControl_Check::onMouseUp(CVector2ui32& vecCursorPosition)
{
	if (CMathUtility::isPointInRectangle(vecCursorPosition, getPosition(), getSize() + CVector2ui32(getTextWidth(), 0)))
	{
		if (CEventManager::getInstance()->triggerEvent(EVENT_onCheckCheck, this))
		{
			setChecked(!isChecked());
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void		CWindowControl_Check::render(void)
{
	CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	if(isChecked())
	{
		CGDIPlusUtility::drawLine(CVector2ui32(getPosition().m_x, getPosition().m_y + getSize().m_y), CVector2ui32(getPosition().m_x + getSize().m_x, getPosition().m_y), getLineColour());
	}
	CGDIPlusUtility::drawText(CVector2ui32(getPosition().m_x + getSize().m_x + getIconRightMargin(), getPosition().m_y), getSize(), getText(), getTextColour(), getFontSize(), isBold());
}

// cursor
bool		CWindowControl_Check::isPointInControl(CVector2ui32& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getPosition(), getSizeWithText());
}

// size
CVector2ui32	CWindowControl_Check::getSizeWithText(void)
{
	return getSize() + CVector2ui32(getTextWidth(), 0);
}