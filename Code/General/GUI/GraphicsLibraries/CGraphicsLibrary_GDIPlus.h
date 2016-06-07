#ifndef CGraphicsLibrary_GDIPlus_H
#define CGraphicsLibrary_GDIPlus_H

#include "Types.h"
#include "CVector2i32.h"
#include "CVector2ui32.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include <vector>
#include <Windows.h>
#include <Gdiplus.h>

// Gdiplus::Pen is for lines
// Gdiplus::Brush is for backgrounds and text

class CGraphicsLibrary_GDIPlus : public CGraphicsLibrary
{
public:
	CGraphicsLibrary_GDIPlus(void);

	void					drawLine(CVector2i32& vecPoint1, CVector2i32& vecPoint2, CGUIStyles *pStyles = nullptr);

	void					drawSquare(CVector2i32& vecPosition, uint32 uiSize, CGUIStyles *pStyles = nullptr);
	void					drawSquareBorder(CVector2i32& vecPosition, uint32 uiSize, CGUIStyles *pStyles = nullptr);
	void					drawSquareFill(CVector2i32& vecPosition, uint32 uiSize, CGUIStyles *pStyles = nullptr);

	void					drawRectangle(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr);
	void					drawRectangleBorder(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr);
	void					drawRectangleFill(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr);

	void					drawCircle(CVector2i32& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr);
	void					drawCircleBorder(CVector2i32& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr);
	void					drawCircleFill(CVector2i32& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr);

	void					drawEllipse(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr);
	void					drawEllipseBorder(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr);
	void					drawEllipseFill(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr);

	void					drawTriangle(CVector2i32& vecPosition, uint32 uiSideLength, uint8 uiFacingDirection, CGUIStyles *pStyles = nullptr);
	void					drawTriangleBorder(CVector2i32& vecPosition, uint32 uiSideLength, uint8 uiFacingDirection, CGUIStyles *pStyles = nullptr);
	void					drawTriangleFill(CVector2i32& vecPosition, uint32 uiSideLength, uint8 uiFacingDirection, CGUIStyles *pStyles = nullptr);

	void					drawPolygon(std::vector<CVector2i32>& vecPoints, CGUIStyles *pStyles = nullptr);
	void					drawPolygonBorder(std::vector<CVector2i32>& vecPoints, CGUIStyles *pStyles = nullptr);
	void					drawPolygonFill(std::vector<CVector2i32>& vecPoints, CGUIStyles *pStyles = nullptr);

	void					drawText(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string& strText, CGUIStyles *pStyles = nullptr);
	CVector2ui32			getTextSize(std::string& strText, CGUIStyles *pStyles = nullptr);

	void					setGraphics(Gdiplus::Graphics* pGraphics) { m_pGraphics = pGraphics; }
	Gdiplus::Graphics*		getGraphics(void) { return m_pGraphics; }

private:
	Gdiplus::Pen*			createPenFromStyles(CGUIStyles *pStyles);
	Gdiplus::Brush*			createBackgroundBrushFromStyles(CGUIStyles *pStyles);
	Gdiplus::Brush*			createTextBrushFromStyles(CGUIStyles *pStyles);
	Gdiplus::Font*			createFontFromStyles(CGUIStyles *pStyles);

	Gdiplus::Point*			getGdiplusPointsFromVectorPoints(std::vector<CVector2i32>& vecPoints);
	Gdiplus::Rect			getGdiplusRect(CVector2i32& vecPosition, CVector2ui32& vecSize);
	Gdiplus::Color			getGdiplusColourFromRGBAUint32(uint32 uiColour);

private:
	Gdiplus::Graphics*		m_pGraphics;
};

#endif