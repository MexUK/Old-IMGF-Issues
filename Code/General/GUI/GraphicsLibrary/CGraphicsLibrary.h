#ifndef CGraphicsLibrary_H
#define CGraphicsLibrary_H

#include "Types.h"
#include "CPoint2D.h"
#include "CSize2D.h"
#include "GUI/String/CGUIString.h"
#include <string>
#include <vector>
#include <Windows.h>
#include <gdiplus.h>

class CGUIStyles;

class CGraphicsLibrary
{
public:
	void							init(void);
	void							uninit(void);

	virtual void					drawLine(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawSquare(CPoint2D& vecPosition, uint32 uiSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawSquareBorder(CPoint2D& vecPosition, uint32 uiSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawSquareFill(CPoint2D& vecPosition, uint32 uiSize, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawRectangle(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawRectangleBorder(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawRectangleFill(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawCircle(CPoint2D& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawCircleBorder(CPoint2D& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawCircleFill(CPoint2D& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawEllipse(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawEllipseBorder(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawEllipseFill(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	
	virtual void					drawTriangle(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CPoint2D& vecPoint3, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawEquilateralTriangle(CPoint2D& vecPosition, float32 fSideLength, uint32 uiPointingDirection, CGUIStyles *pStyles = nullptr) = 0; // [parameter uiPointingDirection] Left=1,Top=2,Right=3,Bottom=4,TopLeft=5,TopRight=6,BottomRight=7,BottomLeft=8
	virtual void					drawEquilateralTriangle(CPoint2D& vecBottomLeftPoint, CPoint2D& vecBottomRightPoint, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawEquilateralTriangle(CPoint2D& vecBottomLeftPoint, float32 fSideLength, float32 fBaseAngle = 0.0f, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawIsoscelesTriangle(CPoint2D& vecPosition, float32 fBaseLength, float32 fLegLength, uint32 uiPointingDirection, CGUIStyles *pStyles = nullptr) = 0; // [parameter uiPointingDirection] Left=1,Top=2,Right=3,Bottom=4,TopLeft=5,TopRight=6,BottomRight=7,BottomLeft=8
	virtual void					drawIsoscelesTriangle(CPoint2D& vecBaseCenterPoint, CPoint2D& vecTipPoint, uint32 uiBaseHalfWidth, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawIsoscelesTriangle(CPoint2D& vecBottomLeftPoint, float32 fBaseLength, float32 fTipAngle = 25.0f, float32 fBaseAngle = 0.0f, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawTriangleBorder(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CPoint2D& vecPoint3, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawTriangleFill(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CPoint2D& vecPoint3, CGUIStyles *pStyles = nullptr) = 0;
	
	virtual void					drawPolygon(std::vector<CPoint2D>& vecPoints, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawPolygonBorder(std::vector<CPoint2D>& vecPoints, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawPolygonFill(std::vector<CPoint2D>& vecPoints, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawText(CPoint2D& vecPosition, CSize2D& vecSize, CGUIString& gstrString, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawText(CPoint2D& vecPosition, CSize2D& vecSize, std::string& strText, CGUIStyles *pStyles = nullptr, CSize2D vecTextSize = CSize2D(0, 0)) = 0;
	virtual CSize2D					getTextSize(std::string& strText, CGUIStyles *pStyles = nullptr) = 0;
};

#endif