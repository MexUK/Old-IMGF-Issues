#ifndef CGraphicsLibrary_H
#define CGraphicsLibrary_H

#include "Types.h"
#include "CVector2i32.h"
#include "CVector2ui32.h"
// todo #include "GUI/Font/CGUIFont.h"
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

	virtual void					drawLine(CVector2i32& vecPoint1, CVector2i32& vecPoint2, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawSquare(CVector2i32& vecPosition, uint32 uiSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawSquareBorder(CVector2i32& vecPosition, uint32 uiSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawSquareFill(CVector2i32& vecPosition, uint32 uiSize, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawRectangle(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawRectangleBorder(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawRectangleFill(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawCircle(CVector2i32& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawCircleBorder(CVector2i32& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawCircleFill(CVector2i32& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawEllipse(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawEllipseBorder(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawEllipseFill(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	
	virtual void					drawTriangle(CVector2i32& vecPoint1, CVector2i32& vecPoint2, CVector2i32& vecPoint3, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawEquilateralTriangle(CVector2i32& vecPosition, float32 fSideLength, uint32 uiPointingDirection, CGUIStyles *pStyles = nullptr) = 0; // [parameter uiPointingDirection] Left=1,Top=2,Right=3,Bottom=4,TopLeft=5,TopRight=6,BottomRight=7,BottomLeft=8
	virtual void					drawEquilateralTriangle(CVector2i32& vecBottomLeftPoint, CVector2i32& vecBottomRightPoint, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawEquilateralTriangle(CVector2i32& vecBottomLeftPoint, float32 fSideLength, float32 fBaseAngle = 0.0f, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawIsoscelesTriangle(CVector2i32& vecPosition, float32 fBaseLength, float32 fLegLength, uint32 uiPointingDirection, CGUIStyles *pStyles = nullptr) = 0; // [parameter uiPointingDirection] Left=1,Top=2,Right=3,Bottom=4,TopLeft=5,TopRight=6,BottomRight=7,BottomLeft=8
	virtual void					drawIsoscelesTriangle(CVector2i32& vecBaseCenterPoint, CVector2i32& vecTipPoint, uint32 uiBaseHalfWidth, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawIsoscelesTriangle(CVector2i32& vecBottomLeftPoint, float32 fBaseLength, float32 fTipAngle = 25.0f, float32 fBaseAngle = 0.0f, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawTriangleBorder(CVector2i32& vecPoint1, CVector2i32& vecPoint2, CVector2i32& vecPoint3, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawTriangleFill(CVector2i32& vecPoint1, CVector2i32& vecPoint2, CVector2i32& vecPoint3, CGUIStyles *pStyles = nullptr) = 0;
	
	virtual void					drawPolygon(std::vector<CVector2i32>& vecPoints, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawPolygonBorder(std::vector<CVector2i32>& vecPoints, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawPolygonFill(std::vector<CVector2i32>& vecPoints, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawText(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string& strText, CGUIStyles *pStyles = nullptr) = 0;
	virtual CVector2ui32			getTextSize(std::string& strText, CGUIStyles *pStyles = nullptr) = 0;
};

#endif