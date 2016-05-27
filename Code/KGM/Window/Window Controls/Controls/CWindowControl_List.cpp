#include "CWindowControl_List.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/CEventManager.h"

using namespace std;

// input
void		CWindowControl_List::onMouseDown(CVector2ui32& vecCursorPosition)
{
	if (isPointInControl(vecCursorPosition))
	{
		CEventManager::getInstance()->setEventHogged(true);
	}
}

// render
void		CWindowControl_List::render(void)
{
	if (doesHaveFill())
	{
		CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	}

	uint32 i = 0;
	for(auto pListEntry : getEntries())
	{
		uint32 uiRowBackgroundColour = (i % 2) == 0 ? getRowBackgroundColour1() : getRowBackgroundColour2();
		CGDIPlusUtility::drawRectangleFill(getPosition() + CVector2ui32(0, i*getRowHeight()), CVector2ui32(getSize().m_x, getRowHeight()), uiRowBackgroundColour);

		uint32 i3 = 0;
		for(vector<string>& vecText : pListEntry->getText())
		{
			uint32 i2 = 0;
			for(string& strText : vecText)
			{
				CGDIPlusUtility::drawText(getPosition() + CVector2ui32(i2 * getColumnWidth(), (i*getRowHeight()) + (i3*getRowTextHeight())), getSize() + CVector2ui32(i2 * getColumnWidth(), (i*getRowHeight()) + ((i3 + 1)*getRowTextHeight())), strText, getTextColour(), getFontSize(), isBold());
				i2++;
			}
			i3++;
		}
		
		if (i == 10) break; // temp

		i++;
	}

	if (doesHaveBorder())
	{
		CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	}
}