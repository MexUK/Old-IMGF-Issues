#include "CWindowControl_List.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/CEventManager.h"

using namespace std;

// input
void					CWindowControl_List::onMouseDown(CVector2ui32& vecCursorPosition)
{
	if (isPointInControl(vecCursorPosition))
	{
		CEventManager::getInstance()->setEventHogged(true);
	}
}

// render
void					CWindowControl_List::render(void)
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
CVector2ui32					CWindowControl_List::getRowPosition(uint32 uiRowIndex)
{
	return getPosition() + CVector2ui32(0, uiRowIndex * getRowHeight());
}

CVector2ui32					CWindowControl_List::getRowSize(void)
{
	return CVector2ui32(getSize().m_x, getRowHeight());
}

// row text
CVector2ui32					CWindowControl_List::getCellTextPosition(uint32 uiRowIndex, uint32 uiTextRowIndex, uint32 uiColumnIndex)
{
	return getPosition() + CVector2ui32(uiColumnIndex * getColumnWidth(), (uiRowIndex * getRowHeight()) + (uiTextRowIndex * getRowTextHeight()));
}

CVector2ui32					CWindowControl_List::getCellTextSize(uint32 uiRowIndex, uint32 uiTextRowIndex, uint32 uiColumnIndex)
{
	return getSize() + CVector2ui32(uiColumnIndex * getColumnWidth(), (uiRowIndex * getRowHeight()) + ((uiTextRowIndex + 1) * getRowTextHeight()));
}