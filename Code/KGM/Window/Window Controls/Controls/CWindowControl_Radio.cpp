#include "CWindowControl_Radio.h"
#include "Math/CMathUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/CEventManager.h"
#include "Window/CWindow.h"

// input
void		CWindowControl_Radio::onMouseUp(CVector2ui32& vecCursorPosition)
{
	if (CMathUtility::isPointInRectangle(vecCursorPosition, getPosition() - CVector2ui32(getSize().m_x / 2, getSize().m_y / 2), getSize() + CVector2ui32(getTextWidth(), 0)))
	{
		if (CEventManager::getInstance()->triggerEvent("onCheckRadio", this))
		{
			getWindow()->uncheckRadios(this);
			m_bChecked = true;
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void		CWindowControl_Radio::render(void)
{
	float32 fRadius = getSize().m_x / 2;
	CVector2ui32 vecCircleCenterPosition = getPosition() + ((uint32) fRadius); // todo - use CVector2ui32 vecCircleCenterPosition = CMathUtility::getCircleFromRectangle()
	if (isChecked())
	{
		CGDIPlusUtility::drawCircleFill(vecCircleCenterPosition, fRadius, 0x0000FFFF);
	}
	else
	{
		CGDIPlusUtility::drawCircleFill(vecCircleCenterPosition, fRadius, getFillColour());
	}
	CGDIPlusUtility::drawCircleBorder(vecCircleCenterPosition, fRadius, getLineColour());
	CGDIPlusUtility::drawText(CVector2ui32(getPosition().m_x + getSize().m_x + getIconRightMargin(), getPosition().m_y), CVector2ui32(getPosition().m_x + getSize().m_x + getIconRightMargin() + 500, getSize().m_y), getText(), getTextColour(), getFontSize(), isBold());
}

// input procressing
bool		CWindowControl_Radio::isPointInControl(CVector2ui32& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getPosition(), getSize() + CVector2ui32(getTextWidth(), 0));
}