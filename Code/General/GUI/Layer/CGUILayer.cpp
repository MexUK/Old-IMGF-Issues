#include "CGUILayer.h"
#include "GUI/Controls/CButtonControl.h"
#include "GUI/Controls/CCheckControl.h"
#include "GUI/Controls/CDropControl.h"
#include "GUI/Controls/CEditControl.h"
#include "GUI/Controls/CListControl.h"
#include "GUI/Controls/CMenuControl.h"
#include "GUI/Controls/CProgressControl.h"
#include "GUI/Controls/CRadioControl.h"
#include "GUI/Controls/CScrollControl.h"
#include "GUI/Controls/CTabControl.h"
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

CGUILayer::CGUILayer(void) :
	m_pWindow(nullptr),
	m_bEnabled(true)
{
}

// event binding
void									CGUILayer::bindAllEvents(void)
{
	for (CGUIShape *pGUIShape : getShapes().getEntries())
	{
		pGUIShape->bindEvents();
	}
	for (CGUIControl *pWindowControl : getControls().getEntries())
	{
		pWindowControl->bindEvents();
	}
}

void									CGUILayer::unbindAllEvents(void)
{
	for (CGUIControl *pWindowControl : getControls().getEntries())
	{
		pWindowControl->unbindEvents();
	}
	for (CGUIShape *pGUIShape : getShapes().getEntries())
	{
		pGUIShape->unbindEvents();
	}
}

// item fetching
CGUIItem*			CGUILayer::getItemByPoint(CPoint2D& vecPoint)
{
	uint32 uiOuterSpacing = 1;
	for (CGUIShape *pShape : getShapes().getEntries())
	{
		if (pShape->isPointInBoundingRectangle(vecPoint, uiOuterSpacing))
		{
			return pShape;
		}
	}
	for (CGUIControl *pControl : getControls().getEntries())
	{
		if (pControl->isPointInBoundingRectangle(vecPoint, uiOuterSpacing))
		{
			return pControl;
		}
	}
	return nullptr;
}

// add control - derived
CGUIControl*		CGUILayer::addControl(eGUIControl eControlId, CGUIStyles *pStyles)
{
	CGUIControl *pControl = createControlFromId(eControlId);
	_addControl(pControl, pStyles);
	return pControl;
}

CButtonControl*		CGUILayer::addButton(CPoint2D& vecPosition, CSize2D& vecSize, string strButtonText, CGUIStyles *pStyles)
{
	CButtonControl *pControl = new CButtonControl;
	pControl->setText(strButtonText);
	_addControl(pControl, vecPosition, vecSize, pStyles);
	return pControl;
}

CCheckControl*		CGUILayer::addCheck(CPoint2D& vecPosition, CSize2D& vecSize, string strCheckText, CGUIStyles *pStyles)
{
	CCheckControl *pControl = new CCheckControl;
	pControl->setText(strCheckText);
	_addControl(pControl, vecPosition, vecSize, pStyles);
	return pControl;
}

CDropControl*		CGUILayer::addDrop(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	CDropControl *pControl = new CDropControl;
	_addControl(pControl, vecPosition, vecSize, pStyles);
	return pControl;
}

CEditControl*		CGUILayer::addEdit(CPoint2D& vecPosition, CSize2D& vecSize, string strEditText, CGUIStyles *pStyles)
{
	CEditControl *pControl = new CEditControl;
	// todo pControl->setText(strEditText);
	_addControl(pControl, vecPosition, vecSize, pStyles);
	return pControl;
}

CListControl*		CGUILayer::addList(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	CListControl *pControl = new CListControl;
	_addControl(pControl, vecPosition, vecSize, pStyles);
	return pControl;
}

CMenuControl*		CGUILayer::addMenu(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	CMenuControl *pControl = new CMenuControl;
	_addControl(pControl, vecPosition, vecSize, pStyles);
	return pControl;
}

CProgressControl*	CGUILayer::addProgress(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	CProgressControl *pControl = new CProgressControl;
	_addControl(pControl, vecPosition, vecSize, pStyles);
	return pControl;
}

CRadioControl*		CGUILayer::addRadio(CPoint2D& vecPosition, CSize2D& vecSize, string strRadioText, CGUIStyles *pStyles)
{
	CRadioControl *pControl = new CRadioControl;
	pControl->setText(strRadioText);
	_addControl(pControl, vecPosition, vecSize, pStyles);
	return pControl;
}

CScrollControl*		CGUILayer::addScroll(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	CScrollControl *pControl = new CScrollControl;
	_addControl(pControl, vecPosition, vecSize, pStyles);
	return pControl;
}

CTabControl*		CGUILayer::addTabBar(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	CTabControl *pControl = new CTabControl;
	_addControl(pControl, vecPosition, vecSize, pStyles);
	return pControl;
}

CTextControl*		CGUILayer::addText(CPoint2D& vecPosition, CSize2D& vecSize, string strText, CGUIStyles *pStyles)
{
	CTextControl *pControl = new CTextControl;
	pControl->setText(strText);
	_addControl(pControl, vecPosition, vecSize, pStyles);
	return pControl;
}

// add control - base
void						CGUILayer::_addControl(CGUIControl *pControl, CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	pControl->setPosition(vecPosition);
	pControl->setSize(vecSize);
	_addControl(pControl, pStyles);
}

void						CGUILayer::_addControl(CGUIControl *pControl, CGUIStyles *pStyles)
{
	pControl->setLayer(this);
	if (pStyles)
	{
		pControl->setStyles(pStyles);
	}
	getControls().addEntry(pControl);
}

// create shape
CGUIShape*					CGUILayer::createShapeFromId(eGUIShape eShapeId)
{
	switch (eShapeId)
	{
	case GUI_SHAPE_CIRCLE:		return new CCircleShape;
	case GUI_SHAPE_ELLIPSE:		return new CEllipseShape;
	case GUI_SHAPE_LINE:		return new CLineShape;
	case GUI_SHAPE_POLYGON:		return new CPolygonShape;
	case GUI_SHAPE_RECTANGLE:	return new CRectangleShape;
	case GUI_SHAPE_SQUARE:		return new CSquareShape;
	case GUI_SHAPE_TRIANGLE:	return new CTriangleShape;
	}
	return nullptr;
}

// create control
CGUIControl*				CGUILayer::createControlFromId(eGUIControl eControlId)
{
	switch (eControlId)
	{
	case GUI_CONTROL_BUTTON:	return new CButtonControl;
	case GUI_CONTROL_CHECK:		return new CCheckControl;
	case GUI_CONTROL_DROP:		return new CDropControl;
	case GUI_CONTROL_EDIT:		return new CEditControl;
	case GUI_CONTROL_LIST:		return new CListControl;
	case GUI_CONTROL_MENU:		return new CMenuControl;
	case GUI_CONTROL_PROGRESS:	return new CProgressControl;
	case GUI_CONTROL_RADIO:		return new CRadioControl;
	case GUI_CONTROL_SCROLL:	return new CScrollControl;
	case GUI_CONTROL_TAB:		return new CTabControl;
	case GUI_CONTROL_TEXT:		return new CTextControl;
	}
	return nullptr;
}

// add shape - derived
CGUIShape*					CGUILayer::addShape(eGUIShape eShapeId, CGUIStyles *pStyles)
{
	CGUIShape *pShape = createShapeFromId(eShapeId);
	_addShape(pShape, pStyles);
	return pShape;
}

CLineShape*					CGUILayer::addLine(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CGUIStyles *pStyles)
{
	CLineShape *pLine = new CLineShape;
	vector<CPoint2D> vecPoints;
	vecPoints.resize(2);
	pLine->setPoint1(vecPoint1);
	pLine->setPoint2(vecPoint2);
	_addShape(pLine, pStyles);
	return pLine;
}

CRectangleShape*			CGUILayer::addRectangle(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	CRectangleShape *pRectangle = new CRectangleShape;
	pRectangle->setPosition(vecPosition);
	pRectangle->setSize(vecSize);
	_addShape(pRectangle, pStyles);
	return pRectangle;
}

CEllipseShape*				CGUILayer::addEllipse(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	CEllipseShape *pEllipse = new CEllipseShape;
	pEllipse->setPosition(vecPosition);
	pEllipse->setSize(vecSize);
	_addShape(pEllipse, pStyles);
	return pEllipse;
}

CPolygonShape*				CGUILayer::addPolygon(vector<CPoint2D>& vecPoints, CGUIStyles *pStyles)
{
	CPolygonShape *pPolygon = new CPolygonShape;
	pPolygon->setPoints(vecPoints);
	_addShape(pPolygon, pStyles);
	return pPolygon;
}

// add shape - derived - wrappers
CSquareShape*				CGUILayer::addSquare(CPoint2D& vecPosition, uint32 uiSideLength, CGUIStyles *pStyles)
{
	CSquareShape *pSquare = new CSquareShape;
	pSquare->setPosition(vecPosition);
	pSquare->setSize(uiSideLength);
	_addShape(pSquare, pStyles);
	return pSquare;
}

CCircleShape*				CGUILayer::addCircle(CPoint2D& vecPosition, uint32 uiRadius, CGUIStyles *pStyles)
{
	CCircleShape *pCircle = new CCircleShape;
	uint32 uiDiameter = uiRadius * 2;
	pCircle->setPosition(vecPosition);
	pCircle->setSize(uiRadius);
	_addShape(pCircle, pStyles);
	return pCircle;
}

CTriangleShape*				CGUILayer::addTriangle(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CPoint2D& vecPoint3, CGUIStyles *pStyles)
{
	CTriangleShape *pTriangle = new CTriangleShape;
	pTriangle->setPoint1(vecPoint1);
	pTriangle->setPoint2(vecPoint2);
	pTriangle->setPoint3(vecPoint3);
	_addShape(pTriangle, pStyles);
	return pTriangle;
}

CTriangleShape*				CGUILayer::addEquilateralTriangle(CPoint2D& vecPosition, float32 fSideLength, uint32 uiPointingDirection, CGUIStyles *pStyles)
{
	vector<CPoint2D> vecShapePoints = CMathUtility::getEquilateralTrianglePoints(vecPosition, fSideLength, uiPointingDirection);
	return addTriangle(vecShapePoints[0], vecShapePoints[1], vecShapePoints[2], pStyles);
}

CTriangleShape*				CGUILayer::addEquilateralTriangle(CPoint2D& vecBottomLeftPoint, CPoint2D& vecBottomRightPoint, CGUIStyles *pStyles)
{
	vector<CPoint2D> vecShapePoints = CMathUtility::getEquilateralTrianglePoints(vecBottomLeftPoint, vecBottomRightPoint);
	return addTriangle(vecShapePoints[0], vecShapePoints[1], vecShapePoints[2], pStyles);
}

CTriangleShape*				CGUILayer::addEquilateralTriangle(CPoint2D& vecBottomLeftPoint, float32 fSideLength, float32 fBaseAngle, CGUIStyles *pStyles)
{
	vector<CPoint2D> vecShapePoints = CMathUtility::getEquilateralTrianglePoints(vecBottomLeftPoint, fSideLength, fBaseAngle);
	return addTriangle(vecShapePoints[0], vecShapePoints[1], vecShapePoints[2], pStyles);
}

CTriangleShape*				CGUILayer::addIsoscelesTriangle(CPoint2D& vecPosition, float32 fBaseLength, float32 fLegLength, uint32 uiPointingDirection, CGUIStyles *pStyles)
{
	vector<CPoint2D> vecShapePoints = CMathUtility::getIsoscelesTrianglePoints(vecPosition, fBaseLength, fLegLength, uiPointingDirection);
	return addTriangle(vecShapePoints[0], vecShapePoints[1], vecShapePoints[2], pStyles);
}

CTriangleShape*				CGUILayer::addIsoscelesTriangle(CPoint2D& vecBaseCenterPoint, CPoint2D& vecTipPoint, uint32 uiBaseHalfWidth, CGUIStyles *pStyles)
{
	vector<CPoint2D> vecShapePoints = CMathUtility::getIsoscelesTrianglePoints(vecBaseCenterPoint, vecTipPoint, uiBaseHalfWidth);
	return addTriangle(vecShapePoints[0], vecShapePoints[1], vecShapePoints[2], pStyles);
}

CTriangleShape*				CGUILayer::addIsoscelesTriangle(CPoint2D& vecBottomLeftPoint, float32 fBaseLength, float32 fTipAngle, float32 fBaseAngle, CGUIStyles *pStyles)
{
	vector<CPoint2D> vecShapePoints = CMathUtility::getIsoscelesTrianglePoints(vecBottomLeftPoint, fBaseLength, fTipAngle, fBaseAngle);
	return addTriangle(vecShapePoints[0], vecShapePoints[1], vecShapePoints[2], pStyles);
}

// add shape - base
void						CGUILayer::_addShape(CGUIShape *pShape, CGUIStyles *pStyles)
{
	pShape->setLayer(this);
	if (pStyles)
	{
		pShape->setStyles(pStyles);
	}
	getShapes().addEntry(pShape);
}

// enabled state
void						CGUILayer::setEnabled(bool bEnabled)
{
	bool bPreviousEnabledState = m_bEnabled;
	m_bEnabled = bEnabled;
	if (bEnabled)
	{
		if (!bPreviousEnabledState)
		{
			bindAllEvents();
		}
	}
	else
	{
		if (bPreviousEnabledState)
		{
			unbindAllEvents();
		}
	}
}