#include "CTabControl.h"
#include "GUI/Window/CWindow.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Styles/CGUIStyles.h"
#include "Math/CMathUtility.h"

using namespace std;

auto pOnLeftMouseDown_Tab	= [](void *pControl, void *pTriggerArg) { ((CTabControl*) pControl)->onLeftMouseDown(*(CPoint2D*) pTriggerArg); };
auto pOnRender_Tab			= [](void *pControl) { ((CTabControl*) pControl)->render(); };

CTabControl::CTabControl(void) :
	CGUIControl(GUI_CONTROL_TAB),
	m_pActiveTab(nullptr),
	m_uiActiveTabHeightDifference(5)
{
}

// event binding
void					CTabControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseDown, pOnLeftMouseDown_Tab, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Tab, this));
}

// input
void					CTabControl::onLeftMouseDown(CPoint2D& vecCursorPosition)
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

	CPoint2D
		vecTabTopLeftBoundingPosition(getPosition()),
		vecTabTopLeftPosition,
		vecTabTopRightPosition,
		vecTabBottomLeftPosition,
		vecTabBottomRightPosition;
	uint32
		uiTabWidth,
		uiTabHeight,
		uiActiveTabHeightDifference = getActiveTabHeightDifference();
	int32
		iMaxX = getSize().m_x;
	for (CTabControlEntry *pTab : getEntries())
	{
		if (vecTabTopLeftBoundingPosition.m_x >= iMaxX)
		{
			break;
		}

		pTab->checkToRecalculateStringSize(getStyles());

		// calculate tab position and size
		uiTabWidth = pTab->getTextWidth() + getStyles()->getInnerSpacingTotalX();
		uiTabHeight = getSize().m_y;
		if (!pTab->isActiveTab())
		{
			uiTabHeight -= uiActiveTabHeightDifference;
		}
		CPoint2D
			vecTabTopLeftPosition = pTab->isActiveTab() ? vecTabTopLeftBoundingPosition : (vecTabTopLeftBoundingPosition + CPoint2D(0, uiActiveTabHeightDifference)),
			vecTabTopRightPosition = vecTabTopLeftPosition + CPoint2D(uiTabWidth, 0),
			vecTabBottomRightPosition = vecTabTopRightPosition + CPoint2D(0, uiTabHeight),
			vecTabBottomLeftPosition = vecTabTopLeftPosition + CPoint2D(0, uiTabHeight),
			vecTextTabPosition = vecTabTopLeftPosition;
		CSize2D
			vecTabSize(uiTabWidth, uiTabHeight);

		// draw tab fill
		pGFX->drawRectangleFill(vecTabTopLeftPosition, vecTabSize, getStyles());

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
CTabControlEntry*		CTabControl::getTabFromPosition(CPoint2D& vecPosition)
{
	CGraphicsLibrary
		*pGFX = CGUIManager::getInstance()->getGraphicsLibrary();
	CPoint2D
		vecTabTopLeftPosition(getPosition());
	CSize2D
		vecTabSize;
	uint32
		uiTabTextWidth,
		uiTabWidth,
		uiTabHeight,
		uiActiveTabHeightDifference = getActiveTabHeightDifference();
	int32
		iMaxX = getSize().m_x;
	for (CTabControlEntry *pTab : getEntries())
	{
		if (vecTabTopLeftPosition.m_x >= iMaxX)
		{
			break;
		}

		uiTabTextWidth = pTab->getTextWidth();
		uiTabWidth = uiTabTextWidth + getStyles()->getInnerSpacingTotalX();
		uiTabHeight = getSize().m_y;
		if (!pTab->isActiveTab())
		{
			uiTabHeight -= uiActiveTabHeightDifference;
		}
		vecTabSize = CSize2D(uiTabWidth, uiTabHeight);

		if (CMathUtility::isPointInRectangle(vecPosition, vecTabTopLeftPosition, vecTabSize))
		{
			return pTab;
		}

		vecTabTopLeftPosition.m_x += uiTabWidth;
	}
	return nullptr;
}

// tab layer
void					CTabControl::bindTabLayer(CTabControlEntry *pTab, CGUILayer *pLayer)
{
	m_umapTabLayers[pTab] = pLayer;
}

void					CTabControl::applyTabLayer(CTabControlEntry *pTab, CTabControlEntry *pPreviousTab)
{
	if (m_umapTabLayers.count(pPreviousTab) != 0)
	{
		CGUILayer *pOldLayer = m_umapTabLayers[pPreviousTab];
		pOldLayer->setEnabled(false);
	}

	if (m_umapTabLayers.count(pTab) != 0)
	{
		CGUILayer *pNewLayer = m_umapTabLayers[pTab];
		pNewLayer->setEnabled(true);
	}
}