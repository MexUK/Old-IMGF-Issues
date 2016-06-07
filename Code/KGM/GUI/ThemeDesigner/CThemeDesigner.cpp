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

	// add 2 lines
	pShape = (CGUIShape*) pControlGroup_AddItem->addLine(CVector2i32(0, 80), CVector2i32(vecWindowSize.m_x, 80));
	pShape->getStyles()->setStyle("border-colour", RGB(230, 223, 12));

	pShape = (CGUIShape*) pControlGroup_AddItem->addLine(CVector2i32(uiCenterX, 0), CVector2i32(uiCenterX, vecWindowSize.m_y));
	pShape->getStyles()->setStyle("border-colour", RGB(230, 223, 12));

	// add headers
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(20, 50), CVector2ui32(150, 30), "Shapes");
	pControl->getStyles()->setStyle("text-colour", RGB(230, 223, 12));

	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiCenterX + 20, 50), CVector2ui32(150, 30), "Controls");
	pControl->getStyles()->setStyle("text-colour", RGB(230, 223, 12));

	// add shape icons and text
	uint32
		uiShapeIconX = 30,
		uiShapeTextX = 60;

	pShape = (CGUIShape*) pControlGroup_AddItem->addCircle(CVector2i32(uiShapeIconX, 120), 7); // todo - make addRicle take a float for radius coz diameter might be odd number
	pShape->getStyles()->setStyle("border-colour", RGB(121, 180, 199));
	pShape = (CGUIShape*) pControlGroup_AddItem->addEllipse(CVector2i32(uiShapeIconX + 3, 120 + 30), CVector2ui32(8, 14));
	pShape->getStyles()->setStyle("border-colour", RGB(121, 180, 199));
	pShape = (CGUIShape*) pControlGroup_AddItem->addLine(CVector2i32(uiShapeIconX, 120 + 60), CVector2i32(uiShapeIconX + 15, 120 + 60 + 15));
	pShape->getStyles()->setStyle("border-colour", RGB(121, 180, 199));
	vector<CVector2i32> vecPoints;
	vecPoints.resize(5);
	vecPoints[0] = CVector2i32(uiShapeIconX, 120 + 90);
	vecPoints[1] = CVector2i32(uiShapeIconX + 15, 120 + 90 + 3);
	vecPoints[2] = CVector2i32(uiShapeIconX + 12, 120 + 90 + 10);
	vecPoints[3] = CVector2i32(uiShapeIconX + 7, 120 + 90 + 5);
	vecPoints[4] = CVector2i32(uiShapeIconX + 4, 120 + 90 + 15);
	pShape = (CGUIShape*) pControlGroup_AddItem->addPolygon(vecPoints);
	pShape->getStyles()->setStyle("border-colour", RGB(121, 180, 199));
	pShape = (CGUIShape*) pControlGroup_AddItem->addRectangle(CVector2i32(uiShapeIconX + 3, 120 + 120), CVector2ui32(8, 15));
	pShape->getStyles()->setStyle("border-colour", RGB(121, 180, 199));
	pShape = (CGUIShape*) pControlGroup_AddItem->addSquare(CVector2i32(uiShapeIconX, 120 + 150), 15);
	pShape->getStyles()->setStyle("border-colour", RGB(121, 180, 199));
	pShape = (CGUIShape*) pControlGroup_AddItem->addEquilateralTriangle(CVector2i32(uiShapeIconX, 120 + 195), 15);
	pShape->getStyles()->setStyle("border-colour", RGB(121, 180, 199));

	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, 120), CVector2ui32(150, 30), "Circle");
	pControl->getStyles()->setStyle("text-colour", RGB(121, 180, 199));
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, 120 + 30), CVector2ui32(150, 30), "Ellipse");
	pControl->getStyles()->setStyle("text-colour", RGB(121, 180, 199));
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, 120 + 60), CVector2ui32(150, 30), "Line");
	pControl->getStyles()->setStyle("text-colour", RGB(121, 180, 199));
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, 120 + 90), CVector2ui32(150, 30), "Polygon");
	pControl->getStyles()->setStyle("text-colour", RGB(121, 180, 199));
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, 120 + 120), CVector2ui32(150, 30), "Rectangle");
	pControl->getStyles()->setStyle("text-colour", RGB(121, 180, 199));
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, 120 + 150), CVector2ui32(150, 30), "Square");
	pControl->getStyles()->setStyle("text-colour", RGB(121, 180, 199));
	pControl = (CGUIControl*) pControlGroup_AddItem->addText(CVector2i32(uiShapeTextX, 120 + 180), CVector2ui32(150, 30), "Triangle");
	pControl->getStyles()->setStyle("text-colour", RGB(121, 180, 199));
}