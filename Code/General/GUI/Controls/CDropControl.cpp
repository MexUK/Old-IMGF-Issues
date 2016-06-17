#include "CDropControl.h"
#include "Math/CMathUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Window/CWindow.h"
#include "Entries/CDropControlEntry.h"
#include "GUI/Styles/CGUIStyles.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

using namespace std;

auto pOnMouseUp_Drop		= [](void *pControl, void *pTriggerArg) { ((CDropControl*) pControl)->onMouseUp(*(CPoint2D*) pTriggerArg); };
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

// serialization
void				CDropControl::unserialize(bool bSkipControlId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CGUIControl::unserialize(bSkipControlId);
	uint32 uiItemCount = pDataReader->readUint32();
	uint32 uiActiveIndex = pDataReader->readUint32();
	setListWidth(pDataReader->readUint32());
	setSelectionListOpen(pDataReader->readUint8() ? true : false); // todo - readBool8()
	for (uint32 i = 0; i < uiItemCount; i++)
	{
		CDropControlEntry *pDropEntry = new CDropControlEntry;
		pDropEntry->setText(pDataReader->readStringWithLength());
		addEntry(pDropEntry);
	}
	setActiveItem(getEntryByIndex(uiActiveIndex));
}

void				CDropControl::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CGUIControl::serialize();
	pDataWriter->writeUint32(getEntryCount()); // item count
	pDataWriter->writeUint32(getActiveIndex()); // active index
	pDataWriter->writeUint32(getListWidth()); // list width
	pDataWriter->writeUint8(isSelectionListOpen() ? 1 : 0); // selection list open status - todo writeBool8(b)
	for (CDropControlEntry *pItem : getEntries())
	{
		pDataWriter->writeStringWithLengthRef(pItem->getText()); // item text
	}
}

// input
void				CDropControl::onMouseUp(CPoint2D& vecCursorPosition)
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
		bool bRecalculateListWidth = false;
		for (CDropControlEntry *pDropEntry : getEntries())
		{
			if (pDropEntry->checkToRecalculateStringSize(getStyles()))
			{
				bRecalculateListWidth = true;
			}
		}
		if (bRecalculateListWidth)
		{
			recalculateListWidth();
		}

		getStyles()->setItemComponent("drop-list");
		pGFX->drawRectangle(getSelectionListPosition(), getSelectionListSize(), getStyles());

		getStyles()->setItemComponent("drop-row");
		uint32 i = 0;
		for(CDropControlEntry *pDropEntry : getEntries())
		{
			pGFX->drawText(getSelectionListEntryPosition(i), getSelectionListEntrySize(), pDropEntry->getText(), getStyles());
			i++;
		}
	}

	getStyles()->resetItemComponent();
	pGFX->drawRectangle(getPosition(), getSize(), getStyles());
	if (getActiveItem())
	{
		pGFX->drawText(getPosition(), getSize(), getActiveItem()->getText(), getStyles());
	}

	getStyles()->setItemComponent("drop-arrow");
	pGFX->drawEquilateralTriangle(getDropTrianglePosition(), getDropTriangleSideLength(), (uint32) 4, getStyles());

	// reset
	getStyles()->restoreTemporaryStyleData();
}

// add/remove item
CDropControlEntry*	CDropControl::addItem(string strText, bool bIsActiveItem)
{
	CDropControlEntry *pDropEntry = new CDropControlEntry;
	pDropEntry->setText(strText);
	pDropEntry->setStringSizeNeedsRecalculating(true);
	addEntry(pDropEntry);
	if (bIsActiveItem)
	{
		setActiveItem(pDropEntry);
	}
	return pDropEntry;
}

void				CDropControl::removeItem(CDropControlEntry *pDropEntry)
{
	if (getActiveItem() == pDropEntry)
	{
		setActiveItem(nullptr);
	}
	removeEntry(pDropEntry);
}

// cursor
bool				CDropControl::isPointInControl(CPoint2D& vecPoint)
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

bool				CDropControl::isPointInSelectionList(CPoint2D& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getSelectionListPosition(), getSelectionListSize());
}

// selection list
CPoint2D			CDropControl::getSelectionListPosition(void)
{
	return CPoint2D(getPosition().m_x, getPosition().m_y + getSize().m_y);
}

CSize2D				CDropControl::getSelectionListSize(void)
{
	return CSize2D(getListWidth(), getEntryCount() * getListRowHeight());
}

// selection list entry
CPoint2D			CDropControl::getSelectionListEntryPosition(uint32 uiEntryIndex)
{
	CPoint2D vecPosition = getSelectionListPosition();
	return CPoint2D(vecPosition.m_x, vecPosition.m_y + (uiEntryIndex * getListRowHeight()));
}

CSize2D				CDropControl::getSelectionListEntrySize(void)
{
	return CSize2D(getSelectionListSize().m_x, getListRowHeight());
}

uint32				CDropControl::getSelectionListEntryFromPoint(CPoint2D& vecCursorPosition)
{
	return CMathUtility::getRowIndex(vecCursorPosition, getSelectionListPosition(), getListRowHeight(), getEntryCount());
}

// drop triangle
CPoint2D			CDropControl::getDropTrianglePosition(void)
{
	float32
		fTriangleHeight = getDropTriangleSideHeight(),
		fTriangleSideLength = getDropTriangleSideLength();
	CPoint2D
		vecTrianglePositionOffset((getSize().m_x - fTriangleSideLength) - 3, CMathUtility::divide(getSize().m_y - floor(fTriangleHeight), 2.0f));
	return CPoint2D(getPosition() + vecTrianglePositionOffset);
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
void				CDropControl::setSize(CSize2D& vecSize)
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

// active entry
uint32				CDropControl::getActiveIndex(void)
{
	if (getActiveItem() == nullptr)
	{
		return -1;
	}
	else
	{
		return getIndexByEntry(getActiveItem());
	}
}