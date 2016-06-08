#include "CThemeDesigner.h"
#include "GUI/Group/CControlGroup.h"
#include "Math/CMathUtility.h"
#include "GUI/CGUIManager.h"
#include "GUI/Shape/CGUIShape.h"
#include "GUI/Styles/CGUIStyles.h"
#include <vector>

using namespace std;

CThemeDesigner::CThemeDesigner(void) :
	m_pWindow(nullptr)
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
}

void					CThemeDesigner::initWindowDesign(void)
{
	initTab_AddItem();

	getWindow()->bindAllEvents();
}

void					CThemeDesigner::initTab_AddItem(void)
{
	// add control group
	CControlGroup *pControlGroup_AddItem = getWindow()->addControlGroup(getWindow());
	getWindow()->addEntry(pControlGroup_AddItem);

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
		*pStyles_BlueText = CGUIManager::createStyles();
	pStyles_GoldBorder->setStyle("border-colour", RGB(230, 223, 12));
	pStyles_GoldText->setStyle("text-colour", RGB(230, 223, 12));
	pStyles_BlueBorder->setStyle("border-colour", RGB(121, 180, 199));
	pStyles_BlueText->setStyle("text-colour", RGB(121, 180, 199));

	// add 2 lines
	pShape = (CGUIShape*) pControlGroup_AddItem->addLine(CVector2i32(0, 80), CVector2i32(vecWindowSize.m_x, 80), pStyles_GoldBorder);
	pShape = (CGUIShape*) pControlGroup_AddItem->addLine(CVector2i32(uiCenterX, 0), CVector2i32(uiCenterX, vecWindowSize.m_y), pStyles_GoldBorder);

	// add headers
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(20, 50), CVector2ui32(150, 30), "Shapes", pStyles_GoldText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiCenterX + 20, 50), CVector2ui32(150, 30), "Controls", pStyles_GoldText);

	// add shape icons and text
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

	pShape = (CGUIShape*) pControlGroup_AddItem->addCircle(CVector2i32(uiShapeIconX, uiShapeIconY), 7, pStyles_BlueBorder); // todo - make addCircle take a float for radius coz diameter might be odd number
	pShape = (CGUIShape*) pControlGroup_AddItem->addEllipse(CVector2i32(uiShapeIconX + 3, uiShapeIconY + 30), CVector2ui32(8, 14), pStyles_BlueBorder);
	pShape = (CGUIShape*) pControlGroup_AddItem->addLine(CVector2i32(uiShapeIconX, uiShapeIconY + 60), CVector2i32(uiShapeIconX + 15, 120 + 60 + 15), pStyles_BlueBorder);
	pShape = (CGUIShape*) pControlGroup_AddItem->addPolygon(vecPolygonPoints, pStyles_BlueBorder);
	pShape = (CGUIShape*) pControlGroup_AddItem->addRectangle(CVector2i32(uiShapeIconX + 3, uiShapeIconY + 120), CVector2ui32(8, 15), pStyles_BlueBorder);
	pShape = (CGUIShape*) pControlGroup_AddItem->addSquare(CVector2i32(uiShapeIconX, uiShapeIconY + 150), 15, pStyles_BlueBorder);
	pShape = (CGUIShape*) pControlGroup_AddItem->addEquilateralTriangle(CVector2i32(uiShapeIconX, uiShapeIconY + 195), 15.0f, 0.0f, pStyles_BlueBorder);

	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY), CVector2ui32(150, 30), "Circle", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 30), CVector2ui32(150, 30), "Ellipse", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 60), CVector2ui32(150, 30), "Line", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 90), CVector2ui32(150, 30), "Polygon", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 120), CVector2ui32(150, 30), "Rectangle", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 150), CVector2ui32(150, 30), "Square", pStyles_BlueText);
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, uiShapeIconY + 180), CVector2ui32(150, 30), "Triangle", pStyles_BlueText);
}