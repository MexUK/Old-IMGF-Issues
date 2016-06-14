#include "CDropControl.h"
#include "Math/CMathUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Window/CWindow.h"

using namespace std;

auto pOnMouseUp_Drop		= [](void *pControl, void *pTriggerArg) { ((CDropControl*) pControl)->onMouseUp(*(CVector2i32*) pTriggerArg); };
auto pOnRender_Drop			= [](void *pControl) { ((CDropControl*) pControl)->render(); };

CDropControl::CDropControl(void) :
	CGUIControl(GUI_CONTROL_DROP),
	m_uiSelectedIndex(-1),
	m_uiListWidth(100),
	m_uiListRowHeight(30),
	m_bSelectionListOpen(false)
{
}

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
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	if(isSelectionListOpen())
	{
		pGFX->drawRectangle(getSelectionListPosition(), getSelectionListSize(), getStyles());
		uint32 i = 0;
		for(auto pDropEntry : getEntries())
		{
			pGFX->drawText(getSelectionListEntryPosition(i), getSelectionListEntrySize(), pDropEntry->getText(), getStyles());
			i++;
		}
	}

	pGFX->drawRectangleFill(getPosition(), getSize(), getStyles());
	pGFX->drawRectangleBorder(getPosition(), getSize(), getStyles());
	if (getSelectedIndex() != -1)
	{
		pGFX->drawText(getPosition(), getSize(), getEntryByIndex(getSelectedIndex())->getText(), getStyles());
	}
	// todo pGFX->drawTriangle(getPosition(), 10, 0, getStyles());
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
		return CGUIControl::isPointInItem(vecPoint);
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