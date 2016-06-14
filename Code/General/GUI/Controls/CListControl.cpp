#include "CListControl.h"
#include "GUI/Window/CWindow.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

using namespace std;

auto pOnMouseDown_List		= [](void *pControl, void *pTriggerArg) { ((CListControl*) pControl)->onMouseDown(*(CVector2i32*) pTriggerArg); };
auto pOnRender_List			= [](void *pControl) { ((CListControl*) pControl)->render(); };

CListControl::CListControl(void) :
	CGUIControl(GUI_CONTROL_LIST),
	CGUIControlComponent_Text(),
	m_uiRowHeight(50),
	m_uiColumnWidth(100),
	m_uiRowTextHeight(10),
	m_bHasVerticalScrollBar(true)
{
}

// event binding
void					CListControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseDown, pOnMouseDown_List, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_List, this));
}

// input
void					CListControl::onMouseDown(CVector2i32& vecCursorPosition)
{
	if (isPointInItem(vecCursorPosition))
	{
		CEventManager::getInstance()->setEventHogged(true);
	}
}

// render
void					CListControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	if (getStyles()->doesHaveFill())
	{
		pGFX->drawRectangleFill(getPosition(), getSize(), getStyles());
	}

	uint32
		uiRowIndex = 0,
		uiTextRowIndex,
		uiColumnIndex;
	for(auto pListEntry : getEntries())
	{
		uint32 uiRowFillColour = (uiRowIndex % 2) == 0 ? getRowFillColour1() : getRowFillColour2(); // todo - used?
		pGFX->drawRectangleFill(getRowPosition(uiRowIndex), getRowSize(), getStyles());

		uiTextRowIndex = 0;
		for(vector<string>& vecText : pListEntry->getText())
		{
			uiColumnIndex = 0;
			for(string& strText : vecText)
			{
				pGFX->drawText(getCellTextPosition(uiRowIndex, uiTextRowIndex, uiColumnIndex), getCellTextSize(uiRowIndex, uiTextRowIndex, uiColumnIndex), strText, getStyles());
				uiColumnIndex++;
			}

			uiTextRowIndex++;
		}
		
		if (uiRowIndex == 10) break; // todo - temp

		uiRowIndex++;
	}

	if (getStyles()->doesHaveBorder())
	{
		pGFX->drawRectangleBorder(getPosition(), getSize(), getStyles());
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