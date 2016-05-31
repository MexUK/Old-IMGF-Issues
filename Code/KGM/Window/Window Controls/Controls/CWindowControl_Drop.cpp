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
		uint32 uiEntryIndex = getSelectionListEntryFromPoint(vecCursorPosition);
		if (uiEntryIndex == -1)
		{
			// no entry is selected
			setSelectionListOpen(false);
			getWindow()->setMarkedToRedraw(true);
		}
		else
		{
			// entry is selected
			if (CEventManager::getInstance()->triggerEvent(EVENT_onHideDropList, this))
			{
				setSelectionListOpen(false);
				setSelectedIndex(uiEntryIndex);
				getWindow()->setMarkedToRedraw(true);
			}
		}
	}
	else if (CMathUtility::isPointInRectangle(vecCursorPosition, getPosition(), getSize()))
	{
		if (CEventManager::getInstance()->triggerEvent(EVENT_onShowDropList, this))
		{
			setSelectionListOpen(true);
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void		CWindowControl_Drop::render(void)
{
	if(isSelectionListOpen())
	{
		CGDIPlusUtility::drawRectangleFill(getSelectionListPosition(), getSelectionListSize(), getFillColour());
		CGDIPlusUtility::drawRectangleBorder(getSelectionListPosition(), getSelectionListSize(), getLineColour());
		uint32 i = 0;
		for(auto pDropEntry : getEntries())
		{
			CGDIPlusUtility::drawText(getSelectionListEntryPosition(i), getSelectionListEntrySize(), pDropEntry->getText(), getTextColour(), getFontSize(), isBold());
			i++;
		}
	}

	CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	CGDIPlusUtility::drawText(getPosition(), getSize(), getEntryByIndex(getSelectedIndex())->getText(), getTextColour(), getFontSize(), isBold());
	//CGDIPlusUtility::drawTriangle(aaaaaaaaaaaaaaaaaaaaaaaaa); // todo
}

// cursor
bool		CWindowControl_Drop::isPointInControl(CVector2ui32& vecPoint)
{
	if (isSelectionListOpen())
	{
		return isPointInSelectionList(vecPoint);
	}
	else
	{
		return CWindowControl::isPointInControl(vecPoint);
	}
}

bool		CWindowControl_Drop::isPointInSelectionList(CVector2ui32& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getSelectionListPosition(), getSelectionListSize());
}

// selection list
CVector2ui32		CWindowControl_Drop::getSelectionListPosition(void)
{
	return CVector2ui32(getPosition().m_x, getPosition().m_y + getSize().m_y);
}

CVector2ui32		CWindowControl_Drop::getSelectionListSize(void)
{
	return CVector2ui32(getListWidth(), getEntryCount() * getListRowHeight());
}

CVector2ui32		CWindowControl_Drop::getSelectionListEntryPosition(uint32 uiEntryIndex)
{
	CVector2ui32 vecPosition = getSelectionListPosition();
	return CVector2ui32(vecPosition.m_x, vecPosition.m_y + (uiEntryIndex * getListRowHeight()));
}

CVector2ui32		CWindowControl_Drop::getSelectionListEntrySize(void)
{
	return CVector2ui32(getSelectionListSize().m_x, getListRowHeight());
}

uint32				CWindowControl_Drop::getSelectionListEntryFromPoint(CVector2ui32& vecCursorPosition)
{
	return CMathUtility::getRowIndex(vecCursorPosition, getSelectionListPosition(), getListRowHeight(), getEntryCount());
}