#include "CThemeDesignerTab_AddItem.h"
#include "Math/CMathUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/Styles/CGUIStyles.h"
#include "GUI/Shapes/CRectangleShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_1xPoint_1x1DSize.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_1xPoint_1x2DSize.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_2xPoints.h" // todo - rename Point to Points
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_3xPoints.h" // todo - rename Point to Points
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_NxPoints.h" // todo - rename Point to Points
#include "GUI/Controls/CCheckControl.h"
#include "GUI/Controls/CProgressControl.h"
#include "GUI/Controls/CRadioControl.h"
#include "GUI/Controls/CDropControl.h"
#include "GUI/ThemeDesigner/CThemeDesigner.h"

using namespace std;

auto pOnLeftMouseDown_ThemeDesigner				= [](void *pThemeDesigner, void *pTriggerArg) { ((CThemeDesignerTab_AddItem*) pThemeDesigner)->onLeftMouseDown(*(CVector2i32*) pTriggerArg); };
auto pOnMouseMove_ThemeDesigner					= [](void *pThemeDesigner, void *pTriggerArg) { ((CThemeDesignerTab_AddItem*) pThemeDesigner)->onMouseMove(*(CVector2i32*) pTriggerArg); };
auto pOnLeftMouseDown_ThemeDesigner_MainWindow	= [](void *pThemeDesigner, void *pTriggerArg) { ((CThemeDesignerTab_AddItem*) pThemeDesigner)->onLeftMouseDown_MainWindow(*(CVector2i32*) pTriggerArg); };
auto pOnLeftMouseDown_ThemeDesigner_MainWindow_WindowEventType	= [](void *pThemeDesigner, void *pTriggerArg) { ((CThemeDesignerTab_AddItem*) pThemeDesigner)->onLeftMouseDown_MainWindow_WindowEventType(*(CVector2i32*) pTriggerArg); };

CThemeDesignerTab_AddItem::CThemeDesignerTab_AddItem(CThemeDesigner *pThemeDesigner) :
	CThemeDesignerTab(pThemeDesigner),
	m_uiHoveredItemType(0),
	m_uiActiveItemType(0),
	m_eHoveredShapeId(GUI_SHAPE_UNKNOWN),
	m_eHoveredControlId(GUI_CONTROL_UNKNOWN),
	m_eActiveShapeId(GUI_SHAPE_UNKNOWN),
	m_eActiveControlId(GUI_CONTROL_UNKNOWN),
	m_uiItemRowHeight(0)
{
}

// events
void									CThemeDesignerTab_AddItem::bindEvents(void)
{
	getThemeDesigner()->getWindow()->bindEvent(EVENT_onLeftMouseDown, pOnLeftMouseDown_ThemeDesigner, this);
	getThemeDesigner()->getWindow()->bindEvent(EVENT_onMouseMove, pOnMouseMove_ThemeDesigner, this);

	CEventManager *pEventManager = CEventManager::getInstance();
	CWindow *pMainWindow = CGUIManager::getInstance()->getEntryByIndex(1);
	pEventManager->bindEvent(EVENT_TYPE_THEME_DESIGNER, (uint32) pMainWindow->getWindowHandle(), EVENT_onLeftMouseDown, pOnLeftMouseDown_ThemeDesigner_MainWindow, this);

	pMainWindow->bindEvent(EVENT_onLeftMouseDown, pOnLeftMouseDown_ThemeDesigner_MainWindow_WindowEventType, this);
}

// design
void									CThemeDesignerTab_AddItem::initDesign(void)
{
	setShapeIconStartPosition(CVector2i32(30, 120));
	setShapeTextStartPosition(CVector2i32(60, 120));
	setControlIconStartPosition(CVector2i32(250 + 30, 120));
	setControlTextStartPosition(CVector2i32(250 + 60, 120));
	setItemRowSize(CVector2ui32(210, 30));
	setItemSize(CVector2ui32(210, 16));

	CThemeDesigner *pThemeDesigner = getThemeDesigner();

	// add layer
	CGUILayer *pLayer = pThemeDesigner->addLayer("add_item", true);

	// prepare variables
	CGUIShape *pShape = nullptr;
	CGUIControl *pControl = nullptr;

	// fetch window positions and sizes
	CVector2ui32 vecWindowSize = pThemeDesigner->getWindow()->getSize();
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
	pStyles_Edit->setStyle("fill-colour", RGB(0xFF, 0xFF, 0xFF));
	pStyles_BlueBorderAndText->setStyle("border-colour", RGB(121, 180, 199));
	pStyles_BlueBorderAndText->setStyle("text-colour", RGB(121, 180, 199));
	pStyles_Progress->setStyle("border-colour", RGB(121, 180, 199));
	pStyles_Progress->setStyle("progress-bar-fill-colour", RGB(85, 33, 33));
	pStyles_Radio->setStyle<uint32>("border-colour", RGB(121, 180, 199));
	pStyles_Radio->setStyle<uint32>("fill-colour:marked", RGB(85, 33, 33));

	// add 2 lines
	pShape = (CGUIShape*) pLayer->addLine(CVector2i32(0, 80), CVector2i32(vecWindowSize.m_x, 80), pStyles_GoldBorder);					// horizontal line
	pShape = (CGUIShape*) pLayer->addLine(CVector2i32(uiCenterX, 25), CVector2i32(uiCenterX, vecWindowSize.m_y), pStyles_GoldBorder);	// vertical line
	
	// add headers
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(20, 50), CVector2ui32(150, 30), "Shapes", pStyles_GoldText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiCenterX + 20, 50), CVector2ui32(150, 30), "Controls", pStyles_GoldText);

	// prepare shape icons and text
	uint32
		uiShapeIconX = getShapeIconStartPosition().m_x,
		uiShapeTextX = getShapeTextStartPosition().m_x,
		uiShapeIconY = getShapeIconStartPosition().m_y;

	vector<CVector2i32> vecPolygonPoints;
	vecPolygonPoints.resize(5);
	vecPolygonPoints[0] = CVector2i32(uiShapeIconX, uiShapeIconY + 90);
	vecPolygonPoints[1] = CVector2i32(uiShapeIconX + 15, uiShapeIconY + 90 + 3);
	vecPolygonPoints[2] = CVector2i32(uiShapeIconX + 12, uiShapeIconY + 90 + 10);
	vecPolygonPoints[3] = CVector2i32(uiShapeIconX + 7, uiShapeIconY + 90 + 5);
	vecPolygonPoints[4] = CVector2i32(uiShapeIconX + 4, uiShapeIconY + 90 + 15);

	// add shape icons
	pShape = (CGUIShape*) pLayer->addCircle(				CVector2i32(uiShapeIconX, uiShapeIconY),			7, pStyles_BlueBorder); // todo - make addCircle take a float for radius coz diameter might be odd number
	pShape = (CGUIShape*) pLayer->addEllipse(				CVector2i32(uiShapeIconX + 3, uiShapeIconY + 30),	CVector2ui32(8, 14), pStyles_BlueBorder);
	pShape = (CGUIShape*) pLayer->addLine(					CVector2i32(uiShapeIconX, uiShapeIconY + 60),		CVector2i32(uiShapeIconX + 15, 120 + 60 + 15), pStyles_BlueBorder);
	pShape = (CGUIShape*) pLayer->addPolygon(vecPolygonPoints, pStyles_BlueBorder);
	pShape = (CGUIShape*) pLayer->addRectangle(				CVector2i32(uiShapeIconX + 3, uiShapeIconY + 120),	CVector2ui32(8, 15), pStyles_BlueBorder);
	pShape = (CGUIShape*) pLayer->addSquare(				CVector2i32(uiShapeIconX, uiShapeIconY + 150),		15, pStyles_BlueBorder);
	pShape = (CGUIShape*) pLayer->addEquilateralTriangle(	CVector2i32(uiShapeIconX, uiShapeIconY + 195),		15.0f, 0.0f, pStyles_BlueBorder);

	// add shape text
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiShapeTextX, uiShapeIconY), CVector2ui32(150, 30), "Circle", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 30), CVector2ui32(150, 30), "Ellipse", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 60), CVector2ui32(150, 30), "Line", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 90), CVector2ui32(150, 30), "Polygon", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 120), CVector2ui32(150, 30), "Rectangle", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 150), CVector2ui32(150, 30), "Square", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 180), CVector2ui32(150, 30), "Triangle", pStyles_BlueText);

	// prepare control icons and text
	uint32
		uiControlIconX = uiCenterX + 30,
		uiControlTextX = uiCenterX + 60,
		uiControlIconY = 120;

	// add control icons
	pControl = (CGUIControl*) pLayer->addButton(CVector2i32(uiControlIconX, uiControlIconY), CVector2ui32(15, 9), "", pStyles_BlueBorder);

	pControl = (CGUIControl*) pLayer->addCheck(CVector2i32(uiControlIconX, uiControlIconY + 30), CVector2ui32(15, 15), "", pStyles_BlueBorder);
	((CCheckControl*) pControl)->setMarked(true);

	pControl = (CGUIControl*) pLayer->addDrop(CVector2i32(uiControlIconX, uiControlIconY + 60), CVector2ui32(15, 10), pStyles_BlueBorder);
	/*
	((CDropControl*) pControl)->addItem("aaaaa");
	((CDropControl*) pControl)->addItem("bbbbbbbbbbbbbb");
	((CDropControl*) pControl)->addItem("c");
	*/

	pControl = (CGUIControl*) pLayer->addEdit(CVector2i32(uiControlIconX, uiControlIconY + 90), CVector2ui32(15, 15), "", pStyles_Edit);
	pControl = (CGUIControl*) pLayer->addList(CVector2i32(uiControlIconX, uiControlIconY + 120), CVector2ui32(15, 15), pStyles_BlueBorder);
	// todo pControl = (CGUIControl*) pLayer->addMenu(CVector2i32(uiControlIconX, uiControlIconY + 150), CVector2ui32(15, 15), pStyles_BlueBorder);

	pControl = (CGUIControl*) pLayer->addProgress(CVector2i32(uiControlIconX, uiControlIconY + 180), CVector2ui32(15, 12), pStyles_Progress);
	((CProgressControl*) pControl)->setMaxTicks(100);
	((CProgressControl*) pControl)->setCurrentTicks(75);
	((CProgressControl*) pControl)->setTextShown(false);

	pControl = (CGUIControl*) pLayer->addRadio(CVector2i32(uiControlIconX, uiControlIconY + 210), CVector2ui32(15, 15), "", pStyles_Radio);
	((CRadioControl*) pControl)->setMarked(true);

	pControl = (CGUIControl*) pLayer->addScroll(CVector2i32(uiControlIconX, uiControlIconY + 240), CVector2ui32(10, 15), pStyles_BlueBorder);
	// todo pControl = (CGUIControl*) pLayer->addTabs(CVector2i32(uiControlIconX, uiControlIconY + 270), CVector2ui32(15, 15), "Tabs", pStyles_BlueBorder);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiControlIconX, uiControlIconY + 300), CVector2ui32(15, 15), "abc", pStyles_BlueBorderAndText);

	// add control text
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiControlTextX, uiControlIconY), CVector2ui32(150, 30), "Button", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiControlTextX, uiControlIconY + 30), CVector2ui32(150, 30), "Check", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiControlTextX, uiControlIconY + 60), CVector2ui32(150, 30), "Drop", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiControlTextX, uiControlIconY + 90), CVector2ui32(150, 30), "Edit", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiControlTextX, uiControlIconY + 120), CVector2ui32(150, 30), "List", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiControlTextX, uiControlIconY + 150), CVector2ui32(150, 30), "Menu", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiControlTextX, uiControlIconY + 180), CVector2ui32(150, 30), "Progress", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiControlTextX, uiControlIconY + 210), CVector2ui32(150, 30), "Radio", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiControlTextX, uiControlIconY + 240), CVector2ui32(150, 30), "Scroll", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiControlTextX, uiControlIconY + 270), CVector2ui32(150, 30), "Tabs", pStyles_BlueText);
	pControl = (CGUIControl*) pLayer->addText(CVector2i32(uiControlTextX, uiControlIconY + 300), CVector2ui32(150, 30), "Text", pStyles_BlueText);
}

// input - theme designer window
void									CThemeDesignerTab_AddItem::onLeftMouseDown(CVector2i32& vecCursorPosition)
{
	CGUIManager::getInstance()->setThemeDesignerModeEnabled(true);
	CGUIManager::getInstance()->getEntryByIndex(1)->setMarkedToRedraw(true); // redraw main window title bar

	uint32
		uiShapeIndex,
		uiControlIndex;
	CVector2i32
		vecRectanglePosition;
	CVector2ui32
		vecRectangleSize(210, 30); // todo - move to object property
	bool
		bShowRectangle = false;

	uiShapeIndex = getTabShapeIndexFromPoint(vecCursorPosition);
	if (uiShapeIndex == -1)
	{
		uiControlIndex = getTabControlIndexFromPoint(vecCursorPosition);

		if (uiControlIndex != -1)
		{
			// mouse is over a control row
			vecRectanglePosition = getControlRowPoint(uiControlIndex); // todo getControlRowSize(ui)
			setActiveItemType(2); // control
			setActiveControlId(getControlIdFromIndex(uiControlIndex));
			bShowRectangle = true;
		}
	}
	else
	{
		// mouse over a shape row
		vecRectanglePosition = getShapeRowPoint(uiShapeIndex); // todo getShapeRowSize(ui)
		setActiveItemType(1); // shape
		setActiveShapeId(getShapeIdFromIndex(uiShapeIndex));
		bShowRectangle = true;
	}

	if (bShowRectangle)
	{
		if (getThemeDesigner()->getActiveItemRectangle() != nullptr)
		{
			getThemeDesigner()->getActiveItemRectangle()->unbindEvents();
			getThemeDesigner()->getActiveItemRectangle()->getLayer()->getShapes().removeEntry(getThemeDesigner()->getActiveItemRectangle());
			getThemeDesigner()->setActiveItemRectangle(nullptr);
			getThemeDesigner()->getWindow()->setMarkedToRedraw(true);
		}

		CGUIStyles *pStyles = new CGUIStyles;
		pStyles->setStyle("fill-colour", RGBA(215, 0, 0, 50));
		pStyles->setStyle("border-colour", RGBA(255, 0, 0, 230));
		getThemeDesigner()->setActiveItemRectangle(getThemeDesigner()->m_umapTabLayers["add_item"]->addRectangle(vecRectanglePosition, vecRectangleSize, pStyles));
		getThemeDesigner()->getActiveItemRectangle()->bindEvents();
		getThemeDesigner()->getWindow()->setMarkedToRedraw(true);
	}
	else
	{
		if (getThemeDesigner()->getActiveItemRectangle() != nullptr)
		{
			getThemeDesigner()->getActiveItemRectangle()->unbindEvents();
			getThemeDesigner()->getActiveItemRectangle()->getLayer()->getShapes().removeEntry(getThemeDesigner()->getActiveItemRectangle());
			getThemeDesigner()->setActiveItemRectangle(nullptr);
			getThemeDesigner()->getWindow()->setMarkedToRedraw(true);
		}

		CGUIManager::getInstance()->setThemeDesignerModeEnabled(false);
	}
}

void									CThemeDesignerTab_AddItem::onMouseMove(CVector2i32& vecCursorPosition)
{
	uint32
		uiShapeIndex,
		uiControlIndex;
	CVector2i32
		vecRectanglePosition;
	CVector2ui32
		vecRectangleSize(210, 30); // todo - move to object property
	bool
		bShowRectangle = false;

	uiShapeIndex = getTabShapeIndexFromPoint(vecCursorPosition);
	if (uiShapeIndex == -1)
	{
		uiControlIndex = getTabControlIndexFromPoint(vecCursorPosition);

		if (uiControlIndex != -1)
		{
			// mouse is over a control row
			vecRectanglePosition = getControlRowPoint(uiControlIndex); // todo getControlRowSize(ui)
			setHoveredItemType(2); // control
			setHoveredControlId(getControlIdFromIndex(uiControlIndex));
			bShowRectangle = true;
		}
	}
	else
	{
		// mouse over a shape row
		vecRectanglePosition = getShapeRowPoint(uiShapeIndex); // todo getShapeRowSize(ui)
		setHoveredItemType(1); // shape
		setHoveredShapeId(getShapeIdFromIndex(uiShapeIndex));
		bShowRectangle = true;
	}

	if (bShowRectangle)
	{
		if (getThemeDesigner()->getItemHoverRectangle() != nullptr)
		{
			getThemeDesigner()->getItemHoverRectangle()->unbindEvents();
			getThemeDesigner()->getItemHoverRectangle()->getLayer()->getShapes().removeEntry(getThemeDesigner()->getItemHoverRectangle());
			getThemeDesigner()->setItemHoverRectangle(nullptr);
			getThemeDesigner()->getWindow()->setMarkedToRedraw(true);
		}

		CGUIStyles *pStyles = new CGUIStyles;
		pStyles->setStyle("fill-colour", RGBA(215, 0, 0, 50));
		getThemeDesigner()->setItemHoverRectangle(getThemeDesigner()->m_umapTabLayers["add_item"]->addRectangle(vecRectanglePosition, vecRectangleSize, pStyles));
		getThemeDesigner()->getItemHoverRectangle()->bindEvents();
		getThemeDesigner()->getWindow()->setMarkedToRedraw(true);
	}
	else
	{
		if (getThemeDesigner()->getItemHoverRectangle() != nullptr)
		{
			getThemeDesigner()->getItemHoverRectangle()->unbindEvents();
			getThemeDesigner()->getItemHoverRectangle()->getLayer()->getShapes().removeEntry(getThemeDesigner()->getItemHoverRectangle());
			getThemeDesigner()->setItemHoverRectangle(nullptr);
			getThemeDesigner()->getWindow()->setMarkedToRedraw(true);
		}
	}
}

// input - main window
void									CThemeDesignerTab_AddItem::onLeftMouseDown_MainWindow(CVector2i32& vecCursorPosition)
{
	CWindow *pMainWindow = CGUIManager::getInstance()->getEntryByIndex(1);

	uint32 uiAddItemType = getActiveItemType();
	if (uiAddItemType != 0)
	{
		// add shape or control
		CGUILayer *pLayer = pMainWindow->getEntryByIndex(0);

		CGUIStyles *pDefaultItemStyles = new CGUIStyles;
		pDefaultItemStyles->setStyle("border-colour", RGB(0, 0, 0));

		if (uiAddItemType == 1)
		{
			// shape
			CGUIShape *pShape = pLayer->addShape(getActiveShapeId(), pDefaultItemStyles);
			switch (pShape->getShapeGeometry())
			{
			case GUI_SHAPE_GEOMETRY_1_POINT_1_X_1D_SIZE: // circle, square
				((CGUIShapeGeometry_1xPoint_1x1DSize*) pShape)->setPosition(vecCursorPosition);
				((CGUIShapeGeometry_1xPoint_1x1DSize*) pShape)->setSize(7.5);
				break;
			case GUI_SHAPE_GEOMETRY_1_POINT_1_X_2D_SIZE: // ellipse, rectangle
				((CGUIShapeGeometry_1xPoint_1x2DSize*) pShape)->setPosition(vecCursorPosition);
				((CGUIShapeGeometry_1xPoint_1x2DSize*) pShape)->setSize(CVector2ui32(10, 15));
				break;
			case GUI_SHAPE_GEOMETRY_2_POINTS: // line
				((CGUIShapeGeometry_2xPoints*) pShape)->setPoint1(vecCursorPosition);
				((CGUIShapeGeometry_2xPoints*) pShape)->setPoint2(vecCursorPosition + CVector2i32(15, 15));
				break;
			case GUI_SHAPE_GEOMETRY_3_POINTS: // triangle
				((CGUIShapeGeometry_3xPoints*) pShape)->setPoint1(vecCursorPosition);
				((CGUIShapeGeometry_3xPoints*) pShape)->setPoint2(vecCursorPosition + CVector2i32(0, 10));
				((CGUIShapeGeometry_3xPoints*) pShape)->setPoint3(vecCursorPosition + CVector2i32(15, 5));
				break;
			case GUI_SHAPE_GEOMETRY_N_POINTS: // polygon
			{
				vector<CVector2i32> vecPolygonPoints;
				vecPolygonPoints.resize(5);
				vecPolygonPoints[0] = CVector2i32(vecCursorPosition.m_x, vecCursorPosition.m_y);
				vecPolygonPoints[1] = CVector2i32(vecCursorPosition.m_x + 15, vecCursorPosition.m_y + 3);
				vecPolygonPoints[2] = CVector2i32(vecCursorPosition.m_x + 12, vecCursorPosition.m_y + 10);
				vecPolygonPoints[3] = CVector2i32(vecCursorPosition.m_x + 7, vecCursorPosition.m_y + 5);
				vecPolygonPoints[4] = CVector2i32(vecCursorPosition.m_x + 4, vecCursorPosition.m_y + 15);
				((CGUIShapeGeometry_NxPoints*) pShape)->setPoints(vecPolygonPoints);
				break;
			}
			}
			pShape->bindEvents();
		}
		else if (uiAddItemType == 2)
		{
			// control
			eGUIControl eControlId = getActiveControlId();
			CGUIControl *pControl = pLayer->addControl(eControlId, pDefaultItemStyles);
			pControl->setPosition(vecCursorPosition);
			pControl->setSize(getControlDefaultSize(eControlId));
			pControl->bindEvents();
		}

		pMainWindow->setMarkedToRedraw(true);
	}
}

void									CThemeDesignerTab_AddItem::onLeftMouseDown_MainWindow_WindowEventType(CVector2i32& vecCursorPosition)
{
	// check to set item active
	CWindow *pMainWindow = CGUIManager::getInstance()->getEntryByIndex(1);
	CGUILayer *pLayer = pMainWindow->getEntryByIndex(0);
	CGUIItem *pItem = pLayer->getItemByPoint(vecCursorPosition);
	if (pItem)
	{
		if (pMainWindow->doesActiveItemExist())
		{
			pMainWindow->clearActiveItem();
		}
		pItem->setActiveItem();
		pMainWindow->setMarkedToRedraw(true);
	}
	else
	{
		if (pMainWindow->doesActiveItemExist())
		{
			pMainWindow->clearActiveItem();
			pMainWindow->setMarkedToRedraw(true);
		}
	}

	/*
	todo
	// check to move/resize control
	uint32 uiEdges = CMathUtility::getRectangleResizeEdges(vecCursorPosition, pItem->getBoundingRectanglePosition(), pItem->getBoundingRectangleSize(), 10);
	if (uiEdges == 0)
	{
		// move item
		setMovingItem(pItem);
	}
	else
	{
		// resize item
		setResizingItem(pItem);
	}
	setResizingItemEdges(uiEdges);
	*/
}

// other
uint32									CThemeDesignerTab_AddItem::getTabShapeIndexFromPoint(CVector2i32& vecCursorPosition) // todo - rename method? // todo - repeated code f7h8e5t
{
	return CMathUtility::getRowIndexInRectangle(vecCursorPosition, getShapeIconStartPosition() - CVector2i32(10, getItemSize().m_y / 2), getItemSize().m_x, getItemRowSize().m_y, 7);
}

uint32									CThemeDesignerTab_AddItem::getTabControlIndexFromPoint(CVector2i32& vecCursorPosition) // todo - rename method? // todo - repeated code f7h8e5t
{
	return CMathUtility::getRowIndexInRectangle(vecCursorPosition, getControlIconStartPosition() - CVector2i32(10, getItemSize().m_y / 2), getItemSize().m_x, getItemRowSize().m_y, 11);
}

CVector2i32								CThemeDesignerTab_AddItem::getShapeRowPoint(uint32 uiShapeRowIndex) // todo - repeated code f7h8e5t
{
	return getShapeIconStartPosition() + CVector2i32(-10, ((int32)(uiShapeRowIndex * getItemRowSize().m_y)) - ((int32) (getItemSize().m_y / 2))); // todo - 10 repeated 4 times - move to object property
}

CVector2i32								CThemeDesignerTab_AddItem::getControlRowPoint(uint32 uiControlRowIndex) // todo - repeated code f7h8e5t
{
	return getControlIconStartPosition() + CVector2i32(-10, ((int32) (uiControlRowIndex * getItemRowSize().m_y)) - ((int32) (getItemSize().m_y / 2)));
}

eGUIShape								CThemeDesignerTab_AddItem::getShapeIdFromIndex(uint32 uiShapeIndex)
{
	switch (uiShapeIndex)
	{
	case 0:		return GUI_SHAPE_CIRCLE;
	case 1:		return GUI_SHAPE_ELLIPSE;
	case 2:		return GUI_SHAPE_LINE;
	case 3:		return GUI_SHAPE_POLYGON;
	case 4:		return GUI_SHAPE_RECTANGLE;
	case 5:		return GUI_SHAPE_SQUARE;
	case 6:		return GUI_SHAPE_TRIANGLE;
	}
	return GUI_SHAPE_UNKNOWN;
}

eGUIControl								CThemeDesignerTab_AddItem::getControlIdFromIndex(uint32 uiControlIndex)
{
	switch (uiControlIndex)
	{
	case 0:		return GUI_CONTROL_BUTTON;
	case 1:		return GUI_CONTROL_CHECK;
	case 2:		return GUI_CONTROL_DROP;
	case 3:		return GUI_CONTROL_EDIT;
	case 4:		return GUI_CONTROL_LIST;
	case 5:		return GUI_CONTROL_MENU;
	case 6:		return GUI_CONTROL_PROGRESS;
	case 7:		return GUI_CONTROL_RADIO;
	case 8:		return GUI_CONTROL_SCROLL;
	case 9:		return GUI_CONTROL_TAB;
	case 10:	return GUI_CONTROL_TEXT;
	}
	return GUI_CONTROL_UNKNOWN;
}

CVector2ui32							CThemeDesignerTab_AddItem::getControlDefaultSize(eGUIControl eControlId)
{
	switch (eControlId)
	{
	case GUI_CONTROL_BUTTON:	return CVector2ui32(60, 20);
	case GUI_CONTROL_CHECK:		return CVector2ui32(20, 20);
	case GUI_CONTROL_DROP:		return CVector2ui32(80, 25);
	case GUI_CONTROL_EDIT:		return CVector2ui32(100, 20);
	case GUI_CONTROL_LIST:		return CVector2ui32(250, 250);
	case GUI_CONTROL_MENU:		return CVector2ui32(250, 20);
	case GUI_CONTROL_PROGRESS:	return CVector2ui32(100, 25);
	case GUI_CONTROL_RADIO:		return CVector2ui32(20, 20);
	case GUI_CONTROL_SCROLL:	return CVector2ui32(250, 15);
	case GUI_CONTROL_TAB:		return CVector2ui32(250, 25);
	case GUI_CONTROL_TEXT:		return CVector2ui32(250, 50);
	}
	return CVector2ui32(50, 50);
}