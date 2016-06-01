#include "CWindowControl_Radio.h"
#include "Math/CMathUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/Window/CWindow.h"

// input
void		CWindowControl_Radio::onMouseUp(CVector2ui32& vecCursorPosition)
{
	if (isPointInControl(vecCursorPosition))
	{
		if (CEventManager::getInstance()->triggerEvent(EVENT_onCheckRadio, this))
		{
			getWindow()->uncheckRadios(this);
			setChecked(true);
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void		CWindowControl_Radio::render(void)
{
	float32 fRadius = getIconRadius();
	CVector2ui32 vecCircleCenterPosition = getIconCenterPosition();
	if (isChecked())
	{
		CGDIPlusUtility::drawCircleFill(vecCircleCenterPosition, fRadius, 0x0000FFFF);
	}
	else
	{
		CGDIPlusUtility::drawCircleFill(vecCircleCenterPosition, fRadius, getFillColour());
	}
	CGDIPlusUtility::drawCircleBorder(vecCircleCenterPosition, fRadius, getLineColour());
	CGDIPlusUtility::drawText(getTextPosition(), getTextSize(), getText(), getTextColour(), getFontSize(), isBold());
}

// cursor
bool		CWindowControl_Radio::isPointInControl(CVector2ui32& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getPosition(), getSizeWithText());
}

// position
CVector2ui32	CWindowControl_Radio::getIconCenterPosition(void)
{
	return CVector2ui32(CMathUtility::getEllipseFromRectangle(getPosition(), getSize()));
}

// size
CVector2ui32	CWindowControl_Radio::getSizeWithText(void)
{
	return getSize() + CVector2ui32(getTextWidth(), 0);
}

uint32			CWindowControl_Radio::getIconRadius(void)
{
	return CMathUtility::convertDiameterToRadius(getSize().m_x);
}

// text
CVector2ui32	CWindowControl_Radio::getTextPosition(void)
{
	return CVector2ui32(getPosition().m_x + getSize().m_x + getIconRightMargin(), getPosition().m_y);
}

CVector2ui32	CWindowControl_Radio::getTextSize(void)
{
	return CVector2ui32(500, getSize().m_y); // todo - change 500 to fetch the displayed width of just the text
}