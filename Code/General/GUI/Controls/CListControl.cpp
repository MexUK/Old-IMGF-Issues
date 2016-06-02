#include "CListControl.h"
#include "GUI/Window/CWindow.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"

using namespace std;

auto pOnMouseDown_List		= [](void *pControl, void *pTriggerArg) { ((CListControl*) pControl)->onMouseDown(*(CVector2i32*) pTriggerArg); };
auto pOnRender_List			= [](void *pControl) { ((CListControl*) pControl)->render(); };

// event binding
void					CListControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseDown, pOnMouseDown_List, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_List, this));
}

// input
void					CListControl::onMouseDown(CVector2i32& vecCursorPosition)
{
	if (isPointInControl(vecCursorPosition))
	{
		CEventManager::getInstance()->setEventHogged(true);
	}
}

// render
void					CListControl::render(void)
{
	if (doesHaveFill())
	{
		CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	}

	uint32
		uiRowIndex = 0,
		uiTextRowIndex,
		uiColumnIndex;
	for(auto pListEntry : getEntries())
	{
		uint32 uiRowBackgroundColour = (uiRowIndex % 2) == 0 ? getRowBackgroundColour1() : getRowBackgroundColour2();
		CGDIPlusUtility::drawRectangleFill(getRowPosition(uiRowIndex), getRowSize(), uiRowBackgroundColour);

		uiTextRowIndex = 0;
		for(vector<string>& vecText : pListEntry->getText())
		{
			uiColumnIndex = 0;
			for(string& strText : vecText)
			{
				CGDIPlusUtility::drawText(getCellTextPosition(uiRowIndex, uiTextRowIndex, uiColumnIndex), getCellTextSize(uiRowIndex, uiTextRowIndex, uiColumnIndex), strText, getTextColour(), getFontSize(), isBold());
				uiColumnIndex++;
			}

			uiTextRowIndex++;
		}
		
		if (uiRowIndex == 10) break; // todo - temp

		uiRowIndex++;
	}

	if (doesHaveBorder())
	{
		CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	}
}

// row
CVector2i32						CListControl::getRowPosition(uint32 uiRowIndex)
{
	return getPosition() + CVector2i32(0, uiRowIndex * getRowHeight());
}

CVector2ui32					CListControl::getRowSize(void)
{
	return CVector2ui32(getSize().m_x, getRowHeight());
}

// row text
CVector2i32						CListControl::getCellTextPosition(uint32 uiRowIndex, uint32 uiTextRowIndex, uint32 uiColumnIndex)
{
	return getPosition() + CVector2i32(uiColumnIndex * getColumnWidth(), (uiRowIndex * getRowHeight()) + (uiTextRowIndex * getRowTextHeight()));
}

CVector2ui32					CListControl::getCellTextSize(uint32 uiRowIndex, uint32 uiTextRowIndex, uint32 uiColumnIndex)
{
	return getSize() + CVector2ui32(uiColumnIndex * getColumnWidth(), (uiRowIndex * getRowHeight()) + ((uiTextRowIndex + 1) * getRowTextHeight()));
}