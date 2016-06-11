#include "CThemeDesigner.h"
#include "Math/CMathUtility.h"
#include "Event/eEvent.h"
#include "Event/CEventManager.h"
#include "GUI/CGUIManager.h"
#include "GUI/Layer/CGUILayer.h"
#include "GUI/Shape/CGUIShape.h"
#include "GUI/Styles/CGUIStyles.h"
#include "GUI/Controls/CTabControl.h"
#include "GUI/ThemeDesigner/Tabs/CThemeDesignerTab_AddItem.h"
#include <vector>

using namespace std;

CThemeDesigner::CThemeDesigner(void) :
	m_pWindow(nullptr),
	m_pTabControl(nullptr),
	m_pItemHoverRectangle(nullptr)
{
}

void					CThemeDesigner::init(void)
{
	initWindow();
	initWindowDesign();
	CEventManager::getInstance()->bindEvent(EVENT_onToolReady, [](void *pBoundArg) { ((CThemeDesigner*) pBoundArg)->bindEvents(); }, this);
}

void					CThemeDesigner::bindEvents(void)
{
	for (CThemeDesignerTab *pThemeDesignerTab : getEntries())
	{
		pThemeDesignerTab->bindEvents();
	}
}

void					CThemeDesigner::initWindow(void)
{
	CVector2i32 vecWindowPosition(10, 10);
	CVector2ui32 vecWindowSize(500, 500);
	CWindow *pWindow = CGUIManager::getInstance()->addWindow(vecWindowPosition, vecWindowSize);
	pWindow->getStyles()->setStyle("fill-colour", RGB(33, 78, 103));
	setWindow(pWindow);

	CGUIStyles *pStyles_TabControl = CGUIManager::createStyles();
	pStyles_TabControl->setStyle("fill-colour", RGB(19, 46, 60));
	pStyles_TabControl->setStyle("text-colour", RGB(121, 180, 199));
	pStyles_TabControl->setStyle("border-colour", RGB(121, 180, 199));
	pStyles_TabControl->setStyle<string>("text-align", "left center");
	pStyles_TabControl->setStyle<uint32>("inner-spacing-left", 15);
	//pStyles_TabControl->setStyle<bool>("border-state-left", false);
	//pStyles_TabControl->setStyle<bool>("border-state-top", false);

	CGUILayer *pLayer_ThemeDesignerWindow = pWindow->addLayer(pWindow, true);
	CTabControl *pTabControl = pLayer_ThemeDesignerWindow->addTabBar(CVector2i32(0, 0), CVector2ui32(vecWindowSize.m_x, 25), pStyles_TabControl);
	setTabControl(pTabControl);
	m_umapTabControlEntries["items"] = pTabControl->addTab("Items");
	m_umapTabControlEntries["add_item"] = pTabControl->addTab("Add Item", true);
	m_umapTabControlEntries["item"] = pTabControl->addTab("Item");
	m_umapTabControlEntries["themes"] = pTabControl->addTab("Themes");
	m_umapTabControlEntries["export"] = pTabControl->addTab("Export");
}

void					CThemeDesigner::initWindowDesign(void)
{
	addEntry(new CThemeDesignerTab_AddItem(this));

	for (CThemeDesignerTab *pThemeDesignerTab : getEntries())
	{
		pThemeDesignerTab->initDesign();
	}

	getWindow()->bindAllEvents();
}

// layers
CGUILayer*				CThemeDesigner::addLayer(string strLayerKey, bool bLayerIsEnabled)
{
	CGUILayer *pLayer = getWindow()->addLayer(getWindow(), bLayerIsEnabled); // todo - param same as object
	m_umapTabLayers[strLayerKey] = pLayer;
	getTabControl()->bindTabLayer(m_umapTabControlEntries[strLayerKey], pLayer);
	return pLayer;
}