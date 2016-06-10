#include "CTabControl.h"
#include "GUI/Window/CWindow.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Styles/CGUIStyles.h"
#include "Math/CMathUtility.h"

using namespace std;

auto pOnLeftMouseDown_Tab	= [](void *pControl, void *pTriggerArg) { ((CTabControl*) pControl)->onLeftMouseDown(*(CVector2i32*) pTriggerArg); };
auto pOnRender_Tab			= [](void *pControl) { ((CTabControl*) pControl)->render(); };

CTabControl::CTabControl(void) :
	CGUIControl(GUI_CONTROL_TAB),
	m_pActiveTab(nullptr)
{
}

// event binding
void					CTabControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseDown, pOnLeftMouseDown_Tab, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Tab, this));
}

// input
void					CTabControl::onLeftMouseDown(CVector2i32& vecCursorPosition)
{
	CTabControlEntry *pTab = getTabFromPosition(vecCursorPosition);
	if (pTab != nullptr)
	{
		if (getWindow()->triggerEvent(EVENT_onPressTab, this))
		{
			CTabControlEntry *pPreviousActiveTab = getActiveTab();
			setActiveTab(pTab);
			applyTabLayer(pTab, pPreviousActiveTab);
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void					CTabControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	CVector2i32
		vecTabTopLeftBoundingPosition(getPosition()),
		vecTabTopLeftPosition,
		vecTabTopRightPosition,
		vecTabBottomLeftPosition,
		vecTabBottomRightPosition;
	uint32
		uiTabTextWidth,
		uiTabWidth,
		uiTabHeight,
		uiActiveTabHeightDifference = 5;
	for (CTabControlEntry *pTab : getEntries())
	{
		// calculate tab position and size
		uiTabTextWidth = pGFX->getTextSize(pTab->getText(), getStyles()).m_x;
		pTab->setTextWidth(uiTabTextWidth);
		uiTabWidth = uiTabTextWidth + 30;
		if (pTab->isActiveTab())
		{
			uiTabHeight = getSize().m_y;
		}
		else
		{
			uiTabHeight = getSize().m_y - uiActiveTabHeightDifference;
		}
		CVector2i32
			vecTabTopLeftPosition = pTab->isActiveTab() ? vecTabTopLeftBoundingPosition : (vecTabTopLeftBoundingPosition + CVector2i32(0, uiActiveTabHeightDifference)),
			vecTabTopRightPosition = vecTabTopLeftPosition + CVector2i32(uiTabWidth, 0),
			vecTabBottomRightPosition = vecTabTopRightPosition + CVector2i32(0, uiTabHeight),
			vecTabBottomLeftPosition = vecTabTopLeftPosition + CVector2i32(0, uiTabHeight),
			vecTextTabPosition = vecTabTopLeftPosition;
		CVector2ui32
			vecTabSize(uiTabWidth, uiTabHeight);

		// draw tab background
		pGFX->drawRectangleFill(vecTabTopLeftPosition, vecTabSize, getStyles()); // top horizontal line

		// draw tab border
		pGFX->drawLine(vecTabTopLeftPosition, vecTabTopRightPosition, getStyles()); // top horizontal line
		if (!pTab->isActiveTab())
		{
			pGFX->drawLine(vecTabBottomLeftPosition, vecTabBottomRightPosition, getStyles()); // bottom horizontal line
		}
		pGFX->drawLine(vecTabTopLeftPosition, vecTabBottomLeftPosition, getStyles()); // left veritcal line
		pGFX->drawLine(vecTabTopRightPosition, vecTabBottomRightPosition, getStyles()); // right vertical line
		
		// draw tab text
		pGFX->drawText(vecTextTabPosition, vecTabSize, pTab->getText(), getStyles()); // tab text

		// calculate next tab bounding position
		vecTabTopLeftBoundingPosition.m_x += uiTabWidth;
	}
}

// add/remove tab
CTabControlEntry*		CTabControl::addTab(string strTabText, bool bSetActiveTab)
{
	CTabControlEntry *pTab = new CTabControlEntry;
	pTab->setTabControl(this);
	pTab->setText(strTabText);
	if (bSetActiveTab)
	{
		setActiveTab(pTab);
	}
	addEntry(pTab);
	return pTab;
}

void					CTabControl::removeTab(CTabControlEntry *pTab)
{
	removeEntry(pTab);
	if (getActiveTab() == pTab)
	{
		setActiveTab(nullptr);
	}
}

// tab retrieval
CTabControlEntry*		CTabControl::getTabFromPosition(CVector2i32& vecPosition)
{
	CGraphicsLibrary
		*pGFX = CGUIManager::getInstance()->getGraphicsLibrary();
	CVector2i32
		vecTabTopLeftPosition(getPosition());
	CVector2ui32
		vecTabSize;
	uint32
		uiTabTextWidth,
		uiTabWidth,
		uiTabHeight,
		uiActiveTabHeightDifference = 5;
	for (CTabControlEntry *pTab : getEntries())
	{
		uiTabTextWidth = pTab->getTextWidth();
		uiTabWidth = uiTabTextWidth + 30;
		if (pTab->isActiveTab())
		{
			uiTabHeight = getSize().m_y;
		}
		else
		{
			uiTabHeight = getSize().m_y - uiActiveTabHeightDifference;
		}
		vecTabSize = CVector2ui32(uiTabWidth, uiTabHeight);

		if (CMathUtility::isPointInRectangle(vecPosition, vecTabTopLeftPosition, vecTabSize))
		{
			return pTab;
		}

		vecTabTopLeftPosition.m_x += uiTabWidth;
	}
	return nullptr;
}

// tab layer
void					CTabControl::bindTabLayer(CTabControlEntry *pTab, CControlGroup *pControlGroup)
{
	m_umapTabLayers[pTab] = pControlGroup;
}

void					CTabControl::applyTabLayer(CTabControlEntry *pTab, CTabControlEntry *pPreviousTab)
{
	if (m_umapTabLayers.count(pPreviousTab) != 0)
	{
		CControlGroup *pOldControlGroup = m_umapTabLayers[pPreviousTab];
		pOldControlGroup->setEnabled(false);
	}

	if (m_umapTabLayers.count(pTab) != 0)
	{
		CControlGroup *pNewControlGroup = m_umapTabLayers[pTab];
		pNewControlGroup->setEnabled(true);
	}
}