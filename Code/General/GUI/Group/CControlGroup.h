#ifndef CControlGroup_H
#define CControlGroup_H

#include "Types.h"
#include "Pool/CVectorPool.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Shape/CGUIShape.h"
#include "CVector2i32.h"
#include "CVector2ui32.h"
#include <string>
#include <vector>

class CButtonControl;
class CCheckControl;
class CDropControl;
class CEditControl;
class CListControl;
class CMenuControl;
class CProgressControl;
class CRadioControl;
class CScrollControl;
class CTabBarControl;
class CTextControl;
class CLineShape;
class CTriangleShape;
class CRectangleShape;
class CEllipseShape;
class CPolygonShape;
class CSquareShape;
class CCircleShape;

class CControlGroup : public CVectorPool<CGUIControl*> // todo - move to property like shapes vector pool is
{
public:
	CControlGroup(void);

	void					unload(void) {}

	void					bindAllEvents(void);
	void					unbindAllEvents(void);

	void					setWindow(CWindow *pWindow) { m_pWindow = pWindow; }
	CWindow*				getWindow(void) { return m_pWindow; }

	// controls
	CButtonControl*			addButton(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string strButtonText);
	CCheckControl*			addCheck(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string strCheckText);
	CDropControl*			addDrop(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CEditControl*			addEdit(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string strEditText = "");
	CListControl*			addList(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CMenuControl*			addMenu(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CProgressControl*		addProgress(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CRadioControl*			addRadio(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CScrollControl*			addScroll(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CTabBarControl*			addTabBar(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CTextControl*			addText(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string strText);

	// shapes
	CLineShape*				addLine(CVector2i32& vecPoint1, CVector2i32& vecPoint2);
	CRectangleShape*		addRectangle(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CEllipseShape*			addEllipse(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CPolygonShape*			addPolygon(std::vector<CVector2i32>& vecPoints);

	CSquareShape*			addSquare(CVector2i32& vecPosition, uint32 uiSideLength);
	CCircleShape*			addCircle(CVector2i32& vecPosition, uint32 uiRadius);
	CTriangleShape*			addTriangle(CVector2i32& vecPoint1, CVector2i32& vecPoint2, CVector2i32& vecPoint3);	// scalene or any triangle type
	CTriangleShape*			addEquilateralTriangle(CVector2i32& vecBottomLeftPoint, CVector2i32& vecBottomRightPoint);
	CTriangleShape*			addEquilateralTriangle(CVector2i32& vecBottomLeftPoint, float32 fSidesLength, float32 fBaseAngle = 0.0f);	// base angle parameter is in degrees
	CTriangleShape*			addIsoscelesTriangle(CVector2i32& vecBaseCenterPoint, CVector2i32& vecTipPoint, uint32 uiBaseHalfWidth);
	CTriangleShape*			addIsoscelesTriangle(CVector2i32& vecBottomLeftPoint, float32 fBaseLength, float32 fTipAngle = 25.0f, float32 fBaseAngle = 0.0f);		// tip angle parameter is in degrees

	// CControlGroup continued.
	CVectorPool<CGUIShape*>&	getShapes(void) { return m_vecShapes; }

private:
	void						_addControl(CGUIControl *pWindowControl, CVector2i32& vecPosition, CVector2ui32& vecSize);
	void						_addShape(CGUIShape *pShape, CVector2i32& vecPosition, CVector2ui32& vecSize);
	void						_addShapeWithPolygonPoints(CGUIShape *pShape, std::vector<CVector2i32>& vecPoints);

private:
	CWindow*					m_pWindow;
	CVectorPool<CGUIShape*>		m_vecShapes;
};

#endif