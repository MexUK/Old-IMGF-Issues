#include "CWindowControl_Drop.h"
#include "Math/CMathUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "Window/CWindow.h"
#include "GDIPlus/CGDIPlusUtility.h"

using namespace std;

// input
void		CWindowControl_Drop::onMouseUp(CVector2ui32& vecCursorPosition)
{
	if (isSelectionListOpen())
	{
		CVector2ui32 vecListPosition = CVector2ui32(getPosition().m_x, getPosition().m_y + getSize().m_y);
		uint32 uiRowIndex = CMathUtility::getRowIndex(vecCursorPosition, vecListPosition, getListRowHeight(), getEntryCount());
		if (uiRowIndex == -1)
		{
			m_bSelectionListOpen = false;
			getWindow()->setMarkedToRedraw(true);
		}
		else
		{
			if (CEventManager::getInstance()->triggerEvent(EVENT_onHideDropList, this))
			{
				m_bSelectionListOpen = false;
				m_uiSelectedIndex = uiRowIndex;
				getWindow()->setMarkedToRedraw(true);
			}
		}
	}
	else if (CMathUtility::isPointInRectangle(vecCursorPosition, getPosition(), getSize()))
	{
		if (CEventManager::getInstance()->triggerEvent(EVENT_onShowDropList, this))
		{
			m_bSelectionListOpen = true;
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void		CWindowControl_Drop::render(void)
{
	if(isSelectionListOpen())
	{
		CVector2ui32 vecListSize = CVector2ui32(getListWidth(), getEntryCount() * getListRowHeight());
		CVector2ui32 vecListMin = CVector2ui32(getPosition().m_x, getPosition().m_y + getSize().m_y);
		CVector2ui32 vecListMax = CVector2ui32(getPosition().m_x + vecListSize.m_x, getPosition().m_y + getSize().m_y + vecListSize.m_y);
		
		CGDIPlusUtility::drawRectangleFill(vecListMin, vecListSize, getFillColour());
		CGDIPlusUtility::drawRectangleBorder(vecListMin, vecListSize, getLineColour());
		uint32 i = 0;
		for(auto pDropEntry : getEntries())
		{
			CGDIPlusUtility::drawText(CVector2ui32(vecListMin.m_x, vecListMin.m_y + (i * getListRowHeight())), CVector2ui32(vecListSize.m_x, getListRowHeight()), pDropEntry->getText(), getTextColour(), getFontSize(), isBold());
			i++;
		}
	}

	CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	CGDIPlusUtility::drawText(getPosition(), getSize(), getEntryByIndex(getSelectedIndex())->getText(), getTextColour(), getFontSize(), isBold());
	//CGDIPlusUtility::drawTriangle(aaaaaaaaaaaaaaaaaaaaaaaaa); // todo
}

bool		CWindowControl_Drop::isPointInControl(CVector2ui32& vecPoint)
{
	if (isSelectionListOpen())
	{
		CVector2ui32 vecListSize = CVector2ui32(getListWidth(), getEntryCount() * getListRowHeight());
		CVector2ui32 vecListMin = CVector2ui32(getPosition().m_x, getPosition().m_y + getSize().m_y);
		return CMathUtility::isPointInRectangle(vecPoint, vecListMin, vecListSize);
	}
	else
	{
		return CWindowControl::isPointInControl(vecPoint);
	}
}