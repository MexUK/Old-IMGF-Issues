#include "CDropControl.h"
#include "Math/CMathUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Window/CWindow.h"
#include "Entries/CDropControlEntry.h"
#include "GUI/Styles/CGUIStyles.h"

using namespace std;

auto pOnMouseUp_Drop		= [](void *pControl, void *pTriggerArg) { ((CDropControl*) pControl)->onMouseUp(*(CVector2i32*) pTriggerArg); };
auto pOnRender_Drop			= [](void *pControl) { ((CDropControl*) pControl)->render(); };

CDropControl::CDropControl(void) :
	CGUIControl(GUI_CONTROL_DROP),
	m_pActiveItem(nullptr),
	m_uiListWidth(0),
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
				setActiveItem(getEntryByIndex(uiEntryIndex));
				getWindow()->setMarkedToRedraw(true);
			}
		}
	}
	else if (isPointInControl(vecCursorPosition))
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
		getStyles()->setItemComponent("list");
		pGFX->drawRectangle(getSelectionListPosition(), getSelectionListSize(), getStyles());

		getStyles()->setItemComponent("list-row");
		uint32 i = 0;
		bool bRecalculateListWidth = false;
		for(auto pDropEntry : getEntries())
		{
			if (pDropEntry->checkToRecalculateStringSize(getStyles()))
			{
				bRecalculateListWidth = true;
			}

			pGFX->drawText(getSelectionListEntryPosition(i), getSelectionListEntrySize(), pDropEntry->getText(), getStyles());
			i++;
		}

		if (bRecalculateListWidth)
		{
			recalculateListWidth();
		}
	}

	getStyles()->resetItemComponent(); // todo getStyles()->setItemComponent("");
	pGFX->drawRectangle(getPosition(), getSize(), getStyles());
	if (getActiveItem())
	{
		pGFX->drawText(getPosition(), getSize(), getActiveItem()->getText(), getStyles());
	}

	getStyles()->setItemComponent("arrow");
	pGFX->drawEquilateralTriangle(getDropTrianglePosition(), getDropTriangleSideLength(), (uint32) 4, getStyles());

	// reset
	getStyles()->restoreTemporaryStyleData();
}

// add/remove item
CDropControlEntry*	CDropControl::addItem(string strText)
{
	CDropControlEntry *pDropEntry = new CDropControlEntry;
	pDropEntry->setText(strText);
	pDropEntry->setStringSizeNeedsRecalculating(true);
	addEntry(pDropEntry);
	return pDropEntry;
}

void				CDropControl::removeItem(CDropControlEntry *pDropEntry)
{
	if (getActiveItem() == pDropEntry)
	{
		setActiveItem(nullptr);
	}
	CVectorPool::removeEntry(pDropEntry);
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

// drop triangle
CVector2i32			CDropControl::getDropTrianglePosition(void)
{
	float32
		fTriangleHeight = getDropTriangleSideHeight(),
		fTriangleSideLength = getDropTriangleSideLength();
	CVector2i32
		vecTrianglePositionOffset((getSize().m_x - fTriangleSideLength) - 3, CMathUtility::divide(getSize().m_y - floor(fTriangleHeight), 2.0f));
	return CVector2i32(getPosition() + vecTrianglePositionOffset);
}

float32				CDropControl::getDropTriangleSideLength(void)
{
	return CMathUtility::getEquilateralTriangleSideLengthFromHeight(getDropTriangleSideHeight());
}

float32				CDropControl::getDropTriangleSideHeight(void)
{
	return CMathUtility::divide(getSize().m_y, 2.0f);
}

// set size
void				CDropControl::setSize(CVector2ui32& vecSize)
{
	CGUIControl::setSize(vecSize);
	recalculateListWidth();
}

// list width
void				CDropControl::recalculateListWidth(void)
{
	uint32
		uiMaxWidth = getSize().m_x,
		uiDropEntryTextWidth;
	for (CDropControlEntry *pDropControlEntry : getEntries())
	{
		uiDropEntryTextWidth = pDropControlEntry->getGUIString().getSize().m_x;
		if (uiDropEntryTextWidth > uiMaxWidth)
		{
			uiMaxWidth = uiDropEntryTextWidth;
		}
	}
	setListWidth(uiMaxWidth);
}