#include "CThemeDesigner.h"
#include "GUI/Group/CControlGroup.h"
#include "Math/CMathUtility.h"
#include "GUI/CGUIManager.h"
#include "GUI/Shape/CGUIShape.h"
#include "GUI/Styles/CGUIStyles.h"
#include "GUI/Controls/CCheckControl.h"
#include "GUI/Controls/CProgressControl.h"
#include "GUI/Controls/CRadioControl.h"
#include "GUI/Controls/CTabControl.h"
#include <vector>

using namespace std;

CThemeDesigner::CThemeDesigner(void) :
	m_pWindow(nullptr),
	m_pTabControl(nullptr)
{
}

void					CThemeDesigner::init(void)
{
	initWindow();
	initWindowDesign();
}

void					CThemeDesigner::initWindow(void)
{
	CVector2i32 vecWindowPosition(10, 10);
	CVector2ui32 vecWindowSize(500, 500);
	CWindow *pWindow = CGUIManager::getInstance()->addWindow(vecWindowPosition, vecWindowSize);
	pWindow->getStyles()->setStyle("background-colour", RGB(33, 78, 103));
	setWindow(pWindow);

	CGUIStyles *pStyles_TabControl = CGUIManager::createStyles();
	pStyles_TabControl->setStyle("background-colour", RGB(19, 46, 60));
	pStyles_TabControl->setStyle("text-colour", RGB(121, 180, 199));
	pStyles_TabControl->setStyle("border-colour", RGB(121, 180, 199));
	pStyles_TabControl->setStyle<string>("text-align", "left center");
	pStyles_TabControl->setStyle<uint32>("inner-spacing-left", 15);
	//pStyles_TabControl->setStyle<bool>("border-state-left", false);
	//pStyles_TabControl->setStyle<bool>("border-state-top", false);

	CControlGroup *pControlGroup_ThemeDesignerWindow = pWindow->addControlGroup(pWindow, true);
	CTabControl *pTabControl = pControlGroup_ThemeDesignerWindow->addTabBar(CVector2i32(0, 0), CVector2ui32(vecWindowSize.m_x, 25), pStyles_TabControl);
	setTabControl(pTabControl);
	m_umapTabControlEntries["items"] = pTabControl->addTab("Items");
	m_umapTabControlEntries["add_item"] = pTabControl->addTab("Add Item", true);
	m_umapTabControlEntries["item"] = pTabControl->addTab("Item");
	m_umapTabControlEntries["themes"] = pTabControl->addTab("Themes");
	m_umapTabControlEntries["export"] = pTabControl->addTab("Export");
}

void					CThemeDesigner::initWindowDesign(void)
{
	initTab_AddItem();
	initTab_Items();

	getWindow()->bindAllEvents();
}

void					CThemeDesigner::initTab_AddItem(void)
{
	// add control group
	CControlGroup *pControlGroup_AddItem = getWindow()->addControlGroup(getWindow(), true);

	// bind layer to tab
	getTabControl()->bindTabLayer(m_umapTabControlEntries["add_item"], pControlGroup_AddItem);

	// prepare variables
	CGUIShape *pShape = nullptr;
	CGUIControl *pControl = nullptr;

	// fetch window positions and sizes
	CVector2ui32 vecWindowSize = getWindow()->getSize();
	CVector2i32 vecWindowCenterPosition = CVector2i32(vecWindowSize.m_x, vecWindowSize.m_y) / 2;
	uint32 uiCenterX = CMathUtility::getCenterX(vecWindowSize.m_x);

	// create styles
	CGUIStyles
		*pStyles_GoldBorder = CGUIManager::createStyles(),
		*pStyles_GoldText = CGUIManager::createStyles(),
		*pStyles_BlueBorder = CGUIManager::createStyles(),
		*pStyles_BlueText = CGUIManager::createStyles(),
		*pStyles_Edit = CGUIManager::createStyles(),
		*pStyles_BlueBorderAndText = CGUIManager::createStyles(),
		*pStyles_Progress = CGUIManager::createStyles(),
		*pStyles_Radio = CGUIManager::createStyles();

	pStyles_GoldBorder->setStyle("border-colour", RGB(230, 223, 12));
	pStyles_GoldText->setStyle("text-colour", RGB(230, 223, 12));
	pStyles_BlueBorder->setStyle("border-colour", RGB(121, 180, 199));
	pStyles_BlueText->setStyle("text-colour", RGB(121, 180, 199));
	pStyles_Edit->setStyle("border-colour", RGB(121, 180, 199));
	pStyles_Edit->setStyle("background-colour", RGB(0xFF, 0xFF, 0xFF));
	pStyles_BlueBorderAndText->setStyle("border-colour", RGB(121, 180, 199));
	pStyles_BlueBorderAndText->setStyle("text-colour", RGB(121, 180, 199));
	pStyles_Progress->setStyle("border-colour", RGB(121, 180, 199));
	pStyles_Progress->setStyle("progress-bar-background-colour", RGB(85, 33, 33));
	pStyles_Radio->setStyle("border-colour", RGB(121, 180, 199));
	pStyles_Radio->setStyle("background-colour-marked", RGB(85, 33, 33));

	// add 2 lines
	pShape = (CGUIShape*) pControlGroup_AddItem->addLine(CVector2i32(0, 80), CVector2i32(vecWindowSize.m_x, 80), pStyles_GoldBorder); // horizontal
	pShape = (CGUIShape*) pControlGroup_AddItem->addLine(CVector2i32(uiCenterX, 25), CVector2i32(uiCenterX, vecWindowSize.m_y), pStyles_GoldBorder); // vertical

	// add headers
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(20, 50), CVector2ui32(150, 30), "Shapes", pStyles_GoldText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiCenterX + 20, 50), CVector2ui32(150, 30), "Controls", pStyles_GoldText);

	// prepare shape icons and text
	uint32
		uiShapeIconX = 30,
		uiShapeTextX = 60,
		uiShapeIconY = 120;

	vector<CVector2i32> vecPolygonPoints;
	vecPolygonPoints.resize(5);
	vecPolygonPoints[0] = CVector2i32(uiShapeIconX, uiShapeIconY + 90);
	vecPolygonPoints[1] = CVector2i32(uiShapeIconX + 15, uiShapeIconY + 90 + 3);
	vecPolygonPoints[2] = CVector2i32(uiShapeIconX + 12, uiShapeIconY + 90 + 10);
	vecPolygonPoints[3] = CVector2i32(uiShapeIconX + 7, uiShapeIconY + 90 + 5);
	vecPolygonPoints[4] = CVector2i32(uiShapeIconX + 4, uiShapeIconY + 90 + 15);

	// add shape icons
	pShape = (CGUIShape*) pControlGroup_AddItem->addCircle(CVector2i32(uiShapeIconX, uiShapeIconY), 7, pStyles_BlueBorder); // todo - make addCircle take a float for radius coz diameter might be odd number
	pShape = (CGUIShape*) pControlGroup_AddItem->addEllipse(CVector2i32(uiShapeIconX + 3, uiShapeIconY + 30), CVector2ui32(8, 14), pStyles_BlueBorder);
	pShape = (CGUIShape*) pControlGroup_AddItem->addLine(CVector2i32(uiShapeIconX, uiShapeIconY + 60), CVector2i32(uiShapeIconX + 15, 120 + 60 + 15), pStyles_BlueBorder);
	pShape = (CGUIShape*) pControlGroup_AddItem->addPolygon(vecPolygonPoints, pStyles_BlueBorder);
	pShape = (CGUIShape*) pControlGroup_AddItem->addRectangle(CVector2i32(uiShapeIconX + 3, uiShapeIconY + 120), CVector2ui32(8, 15), pStyles_BlueBorder);
	pShape = (CGUIShape*) pControlGroup_AddItem->addSquare(CVector2i32(uiShapeIconX, uiShapeIconY + 150), 15, pStyles_BlueBorder);
	pShape = (CGUIShape*) pControlGroup_AddItem->addEquilateralTriangle(CVector2i32(uiShapeIconX, uiShapeIconY + 195), 15.0f, 0.0f, pStyles_BlueBorder);

	// add shape text
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY), CVector2ui32(150, 30), "Circle", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 30), CVector2ui32(150, 30), "Ellipse", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 60), CVector2ui32(150, 30), "Line", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 90), CVector2ui32(150, 30), "Polygon", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 120), CVector2ui32(150, 30), "Rectangle", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 150), CVector2ui32(150, 30), "Square", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 180), CVector2ui32(150, 30), "Triangle", pStyles_BlueText);

	// prepare control icons and text
	uint32
		uiControlIconX = uiCenterX + 30,
		uiControlTextX = uiCenterX + 60,
		uiControlIconY = 120;

	// add control icons
	pControl = (CGUIControl*) pControlGroup_AddItem->addButton(CVector2i32(uiControlIconX, uiControlIconY), CVector2ui32(15, 9), "", pStyles_BlueBorder);

	pControl = (CGUIControl*) pControlGroup_AddItem->addCheck(CVector2i32(uiControlIconX, uiControlIconY + 30), CVector2ui32(15, 15), "", pStyles_BlueBorder);
	((CCheckControl*) pControl)->setMarked(true);

	pControl = (CGUIControl*) pControlGroup_AddItem->addDrop(CVector2i32(uiControlIconX, uiControlIconY + 60), CVector2ui32(15, 10), pStyles_BlueBorder);
	pControl = (CGUIControl*) pControlGroup_AddItem->addEdit(CVector2i32(uiControlIconX, uiControlIconY + 90), CVector2ui32(15, 15), "", pStyles_Edit);
	pControl = (CGUIControl*) pControlGroup_AddItem->addList(CVector2i32(uiControlIconX, uiControlIconY + 120), CVector2ui32(15, 15), pStyles_BlueBorder);
	// todo pControl = (CGUIControl*) pControlGroup_AddItem->addMenu(CVector2i32(uiControlIconX, uiControlIconY + 150), CVector2ui32(15, 15), pStyles_BlueBorder);

	pControl = (CGUIControl*) pControlGroup_AddItem->addProgress(CVector2i32(uiControlIconX, uiControlIconY + 180), CVector2ui32(15, 12), pStyles_Progress);
	((CProgressControl*) pControl)->setMaxTicks(100);
	((CProgressControl*) pControl)->setCurrentTicks(75);
	((CProgressControl*) pControl)->setTextShown(false);

	pControl = (CGUIControl*) pControlGroup_AddItem->addRadio(CVector2i32(uiControlIconX, uiControlIconY + 210), CVector2ui32(15, 15), "", pStyles_Radio);
	((CRadioControl*) pControl)->setMarked(true);

	pControl = (CGUIControl*) pControlGroup_AddItem->addScroll(CVector2i32(uiControlIconX, uiControlIconY + 240), CVector2ui32(10, 15), pStyles_BlueBorder);
	// todo pControl = (CGUIControl*) pControlGroup_AddItem->addTabs(CVector2i32(uiControlIconX, uiControlIconY + 270), CVector2ui32(15, 15), "Tabs", pStyles_BlueBorder);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiControlIconX, uiControlIconY + 300), CVector2ui32(15, 15), "abc", pStyles_BlueBorderAndText);

	// add control text
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiControlTextX, uiControlIconY), CVector2ui32(150, 30), "Button", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiControlTextX, uiControlIconY + 30), CVector2ui32(150, 30), "Check", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiControlTextX, uiControlIconY + 60), CVector2ui32(150, 30), "Drop", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiControlTextX, uiControlIconY + 90), CVector2ui32(150, 30), "Edit", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiControlTextX, uiControlIconY + 120), CVector2ui32(150, 30), "List", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiControlTextX, uiControlIconY + 150), CVector2ui32(150, 30), "Menu", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiControlTextX, uiControlIconY + 180), CVector2ui32(150, 30), "Progress", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiControlTextX, uiControlIconY + 210), CVector2ui32(150, 30), "Radio", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiControlTextX, uiControlIconY + 240), CVector2ui32(150, 30), "Scroll", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiControlTextX, uiControlIconY + 270), CVector2ui32(150, 30), "Tabs", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiControlTextX, uiControlIconY + 300), CVector2ui32(150, 30), "Text", pStyles_BlueText);
}

void					CThemeDesigner::initTab_Items(void)
{
	// add control group
	CControlGroup *pControlGroup_AddItem = getWindow()->addControlGroup(getWindow(), false);

	// prepare variables
	CGUIShape *pShape = nullptr;
	CGUIControl *pControl = nullptr;

	// fetch window positions and sizes
	CVector2ui32 vecWindowSize = getWindow()->getSize();
	CVector2i32 vecWindowCenterPosition = CVector2i32(vecWindowSize.m_x, vecWindowSize.m_y) / 2;
	uint32 uiCenterX = CMathUtility::getCenterX(vecWindowSize.m_x);

	// create styles
	CGUIStyles
		*pStyles_Radio = CGUIManager::createStyles();

	pStyles_Radio->setStyle("border-colour", RGB(230, 223, 12));

	// add 2 lines
	pShape = (CGUIShape*) pControlGroup_AddItem->addLine(CVector2i32(0, 80), CVector2i32(vecWindowSize.m_x, 80), pStyles_Radio);
	pShape = (CGUIShape*) pControlGroup_AddItem->addLine(CVector2i32(uiCenterX, 0), CVector2i32(uiCenterX, vecWindowSize.m_y), pStyles_Radio);
}