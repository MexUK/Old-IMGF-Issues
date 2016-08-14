#ifndef CGUILayer_H
#define CGUILayer_H

#include "Types.h"
#include "Pool/CVectorPool.h"
#include "GUI/Shape/CGUIShape.h"
#include "GUI/Control/CGUIControl.h"
#include "CPoint2D.h"
#include "CSize2D.h"
#include <string>
#include <vector>

class CCircleShape;
class CEllipseShape;
class CLineShape;
class CPolygonShape;
class CRectangleShape;
class CSquareShape;
class CTriangleShape;

class CButtonControl;
class CCheckControl;
class CDropControl;
class CEditControl;
class CImageControl;
class CListControl;
class CMenuControl;
class CProgressControl;
class CRadioControl;
class CScrollControl;
class CTabControl;
class CTextControl;

class CGUIStyles;
class CGUIItem;

class CGUILayer
{
public:
	CGUILayer(void);

	void					unload(void) {}

	void					bindAllEvents(void);
	void					unbindAllEvents(void);

	void					unserialize(void);
	void					serialize(void);

	void					setWindow(CWindow *pWindow) { m_pWindow = pWindow; }
	CWindow*				getWindow(void) { return m_pWindow; }

	void					setEnabled(bool bEnabled);
	bool					isEnabled(void) { return m_bEnabled; }

	CGUIItem*				getItemByPoint(CPoint2D& vecPoint);

	// controls
	CGUIControl*			addControl(eGUIControl eControlId, CGUIStyles *pStyles = nullptr);

	CButtonControl*			addButton(CPoint2D& vecPosition, CSize2D& vecSize, std::string strButtonText, CGUIStyles *pStyles = nullptr);
	CCheckControl*			addCheck(CPoint2D& vecPosition, CSize2D& vecSize, std::string strCheckText, CGUIStyles *pStyles = nullptr);
	CDropControl*			addDrop(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);
	CEditControl*			addEdit(CPoint2D& vecPosition, CSize2D& vecSize, std::string strEditText = "", bool bMultiLine = false, CGUIStyles *pStyles = nullptr);
	CImageControl*			addImage(CPoint2D& vecPosition, std::string& strImagePath, CSize2D vecSize = CSize2D(0, 0), CGUIStyles *pStyles = nullptr);
	// todo CImageControl*			addImageMem(CPoint2D& vecPosition, std::string& strImageData, CSize2D vecSize = CSize2D(0, 0), CGUIStyles *pStyles = nullptr);
	CListControl*			addList(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);
	CMenuControl*			addMenu(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);
	CProgressControl*		addProgress(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);
	CRadioControl*			addRadio(CPoint2D& vecPosition, CSize2D& vecSize, std::string strRadioText, CGUIStyles *pStyles = nullptr);
	CScrollControl*			addScroll(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);
	CTabControl*			addTabBar(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);
	CTextControl*			addText(CPoint2D& vecPosition, CSize2D& vecSize, std::string strText, CGUIStyles *pStyles = nullptr);

	// shapes
	CGUIShape*				addShape(eGUIShape eShapeId, CGUIStyles *pStyles = nullptr);

	CLineShape*				addLine(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CGUIStyles *pStyles = nullptr);
	CRectangleShape*		addRectangle(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);
	CEllipseShape*			addEllipse(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);
	CPolygonShape*			addPolygon(std::vector<CPoint2D>& vecPoints, CGUIStyles *pStyles = nullptr);

	CSquareShape*			addSquare(CPoint2D& vecPosition, uint32 uiSideLength, CGUIStyles *pStyles = nullptr);
	CCircleShape*			addCircle(CPoint2D& vecPosition, uint32 uiRadius, CGUIStyles *pStyles = nullptr);		// position is top left of circle
	CTriangleShape*			addTriangle(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CPoint2D& vecPoint3, CGUIStyles *pStyles = nullptr);		// scalene or any triangle type
	CTriangleShape*			addEquilateralTriangle(CPoint2D& vecPosition, float32 fSideLength, uint32 uiPointingDirection, CGUIStyles *pStyles = nullptr); // [parameter uiPointingDirection] Left=1,Top=2,Right=3,Bottom=4,TopLeft=5,TopRight=6,BottomRight=7,BottomLeft=8
	CTriangleShape*			addEquilateralTriangle(CPoint2D& vecBottomLeftPoint, CPoint2D& vecBottomRightPoint, CGUIStyles *pStyles = nullptr);
	CTriangleShape*			addEquilateralTriangle(CPoint2D& vecBottomLeftPoint, float32 fSideLength, float32 fBaseAngle = 0.0f, CGUIStyles *pStyles = nullptr);	// base angle parameter is in degrees
	CTriangleShape*			addIsoscelesTriangle(CPoint2D& vecPosition, float32 fBaseLength, float32 fLegLength, uint32 uiPointingDirection, CGUIStyles *pStyles = nullptr); // [parameter uiPointingDirection] Left=1,Top=2,Right=3,Bottom=4,TopLeft=5,TopRight=6,BottomRight=7,BottomLeft=8
	CTriangleShape*			addIsoscelesTriangle(CPoint2D& vecBaseCenterPoint, CPoint2D& vecTipPoint, uint32 uiBaseHalfWidth, CGUIStyles *pStyles = nullptr);
	CTriangleShape*			addIsoscelesTriangle(CPoint2D& vecBottomLeftPoint, float32 fBaseLength, float32 fTipAngle = 25.0f, float32 fBaseAngle = 0.0f, CGUIStyles *pStyles = nullptr);	// tip angle parameter is in degrees

	// CGUILayer continued.
	CVectorPool<CGUIShape*>&	getShapes(void) { return m_vecShapes; }
	CVectorPool<CGUIControl*>&	getControls(void) { return m_vecControls; }

	CGUIControl*				getControlById(uint32 uiControlId);
	CGUIShape*					getShapeById(uint32 uiShapeId);

private:
	void						_addControl(CGUIControl *pWindowControl, CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles);
	void						_addControl(CGUIControl *pWindowControl, CGUIStyles *pStyles);
	void						_addShape(CGUIShape *pShape, CGUIStyles *pStyles);

	CGUIShape*					createShapeFromId(eGUIShape eShapeId);
	CGUIControl*				createControlFromId(eGUIControl eControlId);

private:
	CWindow*					m_pWindow;
	bool						m_bEnabled;
	CVectorPool<CGUIShape*>		m_vecShapes;
	CVectorPool<CGUIControl*>	m_vecControls;
};

#endif