#include "CControlGroup.h"
#include "GUI/Controls/CButtonControl.h"
#include "GUI/Controls/CCheckControl.h"
#include "GUI/Controls/CDropControl.h"
#include "GUI/Controls/CEditControl.h"
#include "GUI/Controls/CListControl.h"
#include "GUI/Controls/CMenuControl.h"
#include "GUI/Controls/CProgressControl.h"
#include "GUI/Controls/CRadioControl.h"
#include "GUI/Controls/CScrollControl.h"
// todo #include "GUI/Controls/CTabBarControl.h"
#include "GUI/Controls/CTextControl.h"
#include "GUI/Shapes/CCircleShape.h"
#include "GUI/Shapes/CEllipseShape.h"
#include "GUI/Shapes/CLineShape.h"
#include "GUI/Shapes/CPolygonShape.h"
#include "GUI/Shapes/CRectangleShape.h"
#include "GUI/Shapes/CSquareShape.h"
#include "GUI/Shapes/CTriangleShape.h"
#include "Math/CMathUtility.h"

using namespace std;

CControlGroup::CControlGroup(void) :
	m_pWindow(nullptr)
{
}

// event binding
void									CControlGroup::bindAllEvents(void)
{
	for (CGUIControl *pWindowControl : getEntries())
	{
		pWindowControl->bindEvents();
	}
	for (CGUIShape *pGUIShape : getShapes().getEntries())
	{
		pGUIShape->bindEvents();
	}
}

void									CControlGroup::unbindAllEvents(void)
{
	for (CGUIShape *pGUIShape : getShapes().getEntries())
	{
		pGUIShape->unbindEvents();
	}
	for (CGUIControl *pWindowControl : getEntries())
	{
		pWindowControl->unbindEvents();
	}
}

// add control - derived
CButtonControl*		CControlGroup::addButton(CVector2i32& vecPosition, CVector2ui32& vecSize, string strButtonText)
{
	CButtonControl *pControl = new CButtonControl;
	pControl->setText(strButtonText);
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CCheckControl*		CControlGroup::addCheck(CVector2i32& vecPosition, CVector2ui32& vecSize, string strCheckText)
{
	CCheckControl *pControl = new CCheckControl;
	pControl->setText(strCheckText);
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CDropControl*		CControlGroup::addDrop(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CDropControl *pControl = new CDropControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CEditControl*		CControlGroup::addEdit(CVector2i32& vecPosition, CVector2ui32& vecSize, string strEditText)
{
	CEditControl *pControl = new CEditControl;
	pControl->setText(strEditText);
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CListControl*		CControlGroup::addList(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CListControl *pControl = new CListControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CMenuControl*		CControlGroup::addMenu(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CMenuControl *pControl = new CMenuControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CProgressControl*	CControlGroup::addProgress(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CProgressControl *pControl = new CProgressControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CRadioControl*		CControlGroup::addRadio(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CRadioControl *pControl = new CRadioControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CScrollControl*		CControlGroup::addScroll(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CScrollControl *pControl = new CScrollControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CTabBarControl*		CControlGroup::addTabBar(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	/*
	todo
	CTabBarControl *pControl = new CTabBarControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
	*/
	return nullptr;
}

CTextControl*		CControlGroup::addText(CVector2i32& vecPosition, CVector2ui32& vecSize, string strText)
{
	CTextControl *pControl = new CTextControl;
	pControl->setText(strText);
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

// add control - base
void						CControlGroup::_addControl(CGUIControl *pControl, CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	pControl->setControlGroup(this);
	pControl->setPosition(vecPosition);
	pControl->setSize(vecSize);
	addEntry(pControl);
}

// add shape - derived
CLineShape*					CControlGroup::addLine(CVector2i32& vecPoint1, CVector2i32& vecPoint2)
{
	CLineShape *pLine = new CLineShape;
	vector<CVector2i32> vecPoints;
	vecPoints.resize(2);
	vecPoints[0] = vecPoint1;
	vecPoints[1] = vecPoint2;
	_addShapeWithPolygonPoints(pLine, vecPoints);
	return pLine;
}

CRectangleShape*			CControlGroup::addRectangle(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CRectangleShape *pRectangle = new CRectangleShape;
	_addShape(pRectangle, vecPosition, vecSize);
	return pRectangle;
}

CEllipseShape*				CControlGroup::addEllipse(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CEllipseShape *pEllipse = new CEllipseShape;
	_addShape(pEllipse, vecPosition, vecSize);
	return pEllipse;
}

CPolygonShape*				CControlGroup::addPolygon(vector<CVector2i32>& vecPoints)
{
	CPolygonShape *pPolygon = new CPolygonShape;
	_addShapeWithPolygonPoints(pPolygon, vecPoints);
	return pPolygon;
}

// add shape - derived - wrappers
CSquareShape*				CControlGroup::addSquare(CVector2i32& vecPosition, uint32 uiSideLength)
{
	CSquareShape *pSquare = new CSquareShape;
	_addShape(pSquare, vecPosition, CVector2ui32(uiSideLength, uiSideLength)); // todo - CVector2ui32(uiSideLength) to init both props
	return pSquare;
}

CCircleShape*				CControlGroup::addCircle(CVector2i32& vecPosition, uint32 uiRadius)
{
	CCircleShape *pCircle = new CCircleShape;
	uint32 uiDiameter = uiRadius * 2;
	_addShape(pCircle, vecPosition, CVector2ui32(uiDiameter, uiDiameter));  // todo - CVector2ui32(uiDiameter) to init both props
	return pCircle;
}

CTriangleShape*				CControlGroup::addTriangle(CVector2i32& vecPoint1, CVector2i32& vecPoint2, CVector2i32& vecPoint3)
{
	vector<CVector2i32> vecPoints;
	vecPoints.resize(3);
	vecPoints[0] = vecPoint1;
	vecPoints[1] = vecPoint2;
	vecPoints[2] = vecPoint3;
	CTriangleShape *pTriangle = new CTriangleShape;
	_addShapeWithPolygonPoints(pTriangle, vecPoints);
	return pTriangle;
}

CTriangleShape*				CControlGroup::addEquilateralTriangle(CVector2i32& vecBottomLeftPoint, CVector2i32& vecBottomRightPoint)
{
	const float32
		fSidesLength = CMathUtility::getDistanceBetweenPoints(vecBottomLeftPoint, vecBottomRightPoint),
		fBaseAngle = CMathUtility::getAngleBetweenPoints(vecBottomLeftPoint, vecBottomRightPoint),
		fAngle = CMathUtility::convertDegreesToRadians(60.0f);
	CVector2i32
		vecTipPoint = CMathUtility::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle - fAngle, fSidesLength);
	return addTriangle(vecBottomLeftPoint, vecBottomRightPoint, vecTipPoint);
}

CTriangleShape*				CControlGroup::addEquilateralTriangle(CVector2i32& vecBottomLeftPoint, float32 fSidesLength, float32 fBaseAngle)
{
	const float32
		fAngle = CMathUtility::convertDegreesToRadians(60.0f);
	fBaseAngle = CMathUtility::convertDegreesToRadians(fBaseAngle);
	CVector2i32
		vecBottomRightPoint = CMathUtility::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle, fSidesLength),
		vecTopPoint = CMathUtility::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle - fAngle, fSidesLength);
	return addTriangle(vecBottomLeftPoint, vecBottomRightPoint, vecTopPoint);
}

CTriangleShape*				CControlGroup::addIsoscelesTriangle(CVector2i32& vecBaseCenterPoint, CVector2i32& vecTipPoint, uint32 uiBaseHalfWidth)
{
	const float32
		fBaseToTipDistance = CMathUtility::getDistanceBetweenPoints(vecBaseCenterPoint, vecTipPoint),
		fBaseToTipAngle = CMathUtility::getAngleBetweenPoints(vecBaseCenterPoint, vecTipPoint),
		f90DegreeAngle = CMathUtility::convertDegreesToRadians(90.0f);
	CVector2i32
		vecBasePoint1 = CMathUtility::getPositionInFrontOfPosition(vecBaseCenterPoint, fBaseToTipAngle - f90DegreeAngle, uiBaseHalfWidth),
		vecBasePoint2 = CMathUtility::getPositionInFrontOfPosition(vecBaseCenterPoint, fBaseToTipAngle + f90DegreeAngle, uiBaseHalfWidth);
	return addTriangle(vecBasePoint1, vecBasePoint2, vecTipPoint);
}

CTriangleShape*				CControlGroup::addIsoscelesTriangle(CVector2i32& vecBottomLeftPoint, float32 fBaseLength, float32 fTipAngle, float32 fBaseAngle)
{
	float32
		fLinesAngle = (180.0f - fTipAngle) / 2.0f,
		fHeight = (fBaseLength / 2.0f) * tan(fLinesAngle),
		fSidesLength = sqrt(((fBaseLength * fBaseLength) / 4.0f) + (fHeight * fHeight));
	fTipAngle = CMathUtility::convertDegreesToRadians(fTipAngle);
	fBaseAngle = CMathUtility::convertDegreesToRadians(fBaseAngle);
	fLinesAngle = CMathUtility::convertDegreesToRadians(fLinesAngle);
	CVector2i32
		vecBottomRightPoint = CMathUtility::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle, fBaseLength),
		vecTipPoint = CMathUtility::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle - fLinesAngle, fSidesLength);
	return addTriangle(vecBottomLeftPoint, vecBottomRightPoint, vecTipPoint);
}

// add shape - base
void						CControlGroup::_addShape(CGUIShape *pShape, CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	pShape->setControlGroup(this);
	pShape->setPosition(vecPosition);
	pShape->setSize(vecSize);
	getShapes().addEntry(pShape);
}

void						CControlGroup::_addShapeWithPolygonPoints(CGUIShape *pShape, vector<CVector2i32>& vecPoints)
{
	pShape->setControlGroup(this);
	pShape->setPolygonPointPositions(vecPoints);
	getShapes().addEntry(pShape);
}