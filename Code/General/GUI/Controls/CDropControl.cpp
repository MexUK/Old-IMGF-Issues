#include "CDropControl.h"
#include "Math/CMathUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/Window/CWindow.h"
#include "GDIPlus/CGDIPlusUtility.h"

using namespace std;

auto pOnMouseUp_Drop		= [](void *pControl, void *pTriggerArg) { ((CDropControl*) pControl)->onMouseUp(*(CVector2i32*) pTriggerArg); };
auto pOnRender_Drop			= [](void *pControl) { ((CDropControl*) pControl)->render(); };

// event binding
void				CDropControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseUp, pOnMouseUp_Drop, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Drop, this));
}

// input
void				CDropControl::onMouseUp(CVector2i32& vecCursorPosition)
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
			if (getWindow()->triggerEvent(EVENT_onHideDropList, this))
			{
				setSelectionListOpen(false);
				setSelectedIndex(uiEntryIndex);
				getWindow()->setMarkedToRedraw(true);
			}
		}
	}
	else if (CMathUtility::isPointInRectangle(vecCursorPosition, getPosition(), getSize()))
	{
		if (getWindow()->triggerEvent(EVENT_onShowDropList, this))
		{
			setSelectionListOpen(true);
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void				CDropControl::render(void)
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
bool				CDropControl::isPointInControl(CVector2i32& vecPoint)
{
	if (isSelectionListOpen())
	{
		return isPointInSelectionList(vecPoint);
	}
	else
	{
		return CGUIControl::isPointInControl(vecPoint);
	}
}

bool				CDropControl::isPointInSelectionList(CVector2i32& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getSelectionListPosition(), getSelectionListSize());
}

// selection list
CVector2i32			CDropControl::getSelectionListPosition(void)
{
	return CVector2i32(getPosition().m_x, getPosition().m_y + getSize().m_y);
}

CVector2ui32		CDropControl::getSelectionListSize(void)
{
	return CVector2ui32(getListWidth(), getEntryCount() * getListRowHeight());
}

// selection list entry
CVector2i32			CDropControl::getSelectionListEntryPosition(uint32 uiEntryIndex)
{
	CVector2i32 vecPosition = getSelectionListPosition();
	return CVector2i32(vecPosition.m_x, vecPosition.m_y + (uiEntryIndex * getListRowHeight()));
}

CVector2ui32		CDropControl::getSelectionListEntrySize(void)
{
	return CVector2ui32(getSelectionListSize().m_x, getListRowHeight());
}

uint32				CDropControl::getSelectionListEntryFromPoint(CVector2i32& vecCursorPosition)
{
	return CMathUtility::getRowIndex(vecCursorPosition, getSelectionListPosition(), getListRowHeight(), getEntryCount());
}