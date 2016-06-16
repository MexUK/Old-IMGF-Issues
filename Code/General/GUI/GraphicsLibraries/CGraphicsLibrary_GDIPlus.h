#ifndef CGraphicsLibrary_GDIPlus_H
#define CGraphicsLibrary_GDIPlus_H

#include "Types.h"
#include "CPoint2D.h"
#include "CSize2D.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/String/CGUIString.h"
#include <vector>
#include <Windows.h>
#include <Gdiplus.h>

// Gdiplus::Pen is for lines
// Gdiplus::Brush is for backgrounds and text

class CColour;

class CGraphicsLibrary_GDIPlus : public CGraphicsLibrary
{
public:
	CGraphicsLibrary_GDIPlus(void);

	void					drawLine(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CGUIStyles *pStyles = nullptr);

	void					drawSquare(CPoint2D& vecPosition, uint32 uiSize, CGUIStyles *pStyles = nullptr);
	void					drawSquareBorder(CPoint2D& vecPosition, uint32 uiSize, CGUIStyles *pStyles = nullptr);
	void					drawSquareFill(CPoint2D& vecPosition, uint32 uiSize, CGUIStyles *pStyles = nullptr);

	void					drawRectangle(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);
	void					drawRectangleBorder(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);
	void					drawRectangleFill(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);

	void					drawCircle(CPoint2D& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr);
	void					drawCircleBorder(CPoint2D& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr);
	void					drawCircleFill(CPoint2D& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr);

	void					drawEllipse(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);
	void					drawEllipseBorder(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);
	void					drawEllipseFill(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles = nullptr);

	void					drawTriangle(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CPoint2D& vecPoint3, CGUIStyles *pStyles = nullptr);
	void					drawEquilateralTriangle(CPoint2D& vecPosition, float32 fSideLength, uint32 uiPointingDirection, CGUIStyles *pStyles = nullptr); // [parameter uiPointingDirection] Left=1,Top=2,Right=3,Bottom=4,TopLeft=5,TopRight=6,BottomRight=7,BottomLeft=8
	void					drawEquilateralTriangle(CPoint2D& vecBottomLeftPoint, CPoint2D& vecBottomRightPoint, CGUIStyles *pStyles = nullptr);
	void					drawEquilateralTriangle(CPoint2D& vecBottomLeftPoint, float32 fSideLength, float32 fBaseAngle = 0.0f, CGUIStyles *pStyles = nullptr);
	void					drawIsoscelesTriangle(CPoint2D& vecPosition, float32 fBaseLength, float32 fLegLength, uint32 uiPointingDirection, CGUIStyles *pStyles = nullptr); // [parameter uiPointingDirection] Left=1,Top=2,Right=3,Bottom=4,TopLeft=5,TopRight=6,BottomRight=7,BottomLeft=8
	void					drawIsoscelesTriangle(CPoint2D& vecBaseCenterPoint, CPoint2D& vecTipPoint, uint32 uiBaseHalfWidth, CGUIStyles *pStyles = nullptr);
	void					drawIsoscelesTriangle(CPoint2D& vecBottomLeftPoint, float32 fBaseLength, float32 fTipAngle = 25.0f, float32 fBaseAngle = 0.0f, CGUIStyles *pStyles = nullptr);
	void					drawTriangleBorder(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CPoint2D& vecPoint3, CGUIStyles *pStyles = nullptr);
	void					drawTriangleFill(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CPoint2D& vecPoint3, CGUIStyles *pStyles = nullptr);

	void					drawPolygon(std::vector<CPoint2D>& vecPoints, CGUIStyles *pStyles = nullptr);
	void					drawPolygonBorder(std::vector<CPoint2D>& vecPoints, CGUIStyles *pStyles = nullptr);
	void					drawPolygonFill(std::vector<CPoint2D>& vecPoints, CGUIStyles *pStyles = nullptr);

	void					drawText(CPoint2D& vecPosition, CSize2D& vecSize, CGUIString& gstrString, CGUIStyles *pStyles = nullptr);
	void					drawText(CPoint2D& vecPosition, CSize2D& vecSize, std::string& strText, CGUIStyles *pStyles = nullptr, CSize2D vecTextSize = CSize2D(0, 0));
	CSize2D					getTextSize(std::string& strText, CGUIStyles *pStyles = nullptr);

	void					setGraphics(Gdiplus::Graphics* pGraphics) { m_pGraphics = pGraphics; }
	Gdiplus::Graphics*		getGraphics(void) { return m_pGraphics; }

private:
	Gdiplus::Pen*			createPenFromStyles(CGUIStyles *pStyles);
	Gdiplus::Brush*			createBackgroundBrushFromStyles(CGUIStyles *pStyles);
	Gdiplus::Brush*			createTextBrushFromStyles(CGUIStyles *pStyles);
	Gdiplus::Font*			createFontFromStyles(CGUIStyles *pStyles);

	CPoint2D				getTextPositionFromStyles(CPoint2D& vecPosition, CSize2D& vecSize, std::string& strText, CSize2D& vecTextSize, CGUIStyles *pStyles);

	Gdiplus::Point*			getGdiplusPointsFromVectorPoints(std::vector<CPoint2D>& vecPoints);
	Gdiplus::Rect			getGdiplusRect(CPoint2D& vecPosition, CSize2D& vecSize);
	Gdiplus::Color			getGdiplusColourFromColour(CColour *pColour);

private:
	Gdiplus::Graphics*		m_pGraphics;
};

#endif