#include "CGraphicsLibrary_GDIPlus.h"
#include "Math/CMathUtility.h"
#include "String/CStringUtility.h"
#include "GUI/Styles/CGUIStyles.h"

using namespace std;
using namespace Gdiplus;

CGraphicsLibrary_GDIPlus::CGraphicsLibrary_GDIPlus(void) :
	m_pGraphics(nullptr)
{
}

// draw line
void					CGraphicsLibrary_GDIPlus::drawLine(CVector2i32& vecPoint1, CVector2i32& vecPoint2, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	m_pGraphics->DrawLine(createPenFromStyles(pStyles), (INT) vecPoint1.m_x, (INT) vecPoint1.m_y, (INT) vecPoint2.m_x, (INT) vecPoint2.m_y);
}

// draw rectangle
void					CGraphicsLibrary_GDIPlus::drawRectangle(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles)
{
	if (pStyles->doesHaveFill()) // todo - use Fill or Background for naming not both
	{
		drawRectangleFill(vecPosition, vecSize, pStyles);
	}
	if (pStyles->doesHaveBorder())
	{
		drawRectangleBorder(vecPosition, vecSize, pStyles);
	}
}

void					CGraphicsLibrary_GDIPlus::drawRectangleBorder(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	uint32 uiBorderIntersectionRadius = (uint32) pStyles->getStyle<float32>("border-intersection-radius");
	if (uiBorderIntersectionRadius == 0)
	{
		// border-intersection-radius is not set
		CVector2i32 vecMaxPoint = CVector2i32(vecPosition.m_x + vecSize.m_x, vecPosition.m_y + vecSize.m_y);
		if (pStyles->doesHaveLeftBorder())
		{
			drawLine(vecPosition, CVector2i32(vecPosition.m_x, vecMaxPoint.m_y), pStyles);
		}
		if (pStyles->doesHaveRightBorder())
		{
			drawLine(vecMaxPoint, CVector2i32(vecMaxPoint.m_x, vecPosition.m_y), pStyles);
		}
		if (pStyles->doesHaveTopBorder())
		{
			drawLine(vecPosition, CVector2i32(vecMaxPoint.m_x, vecPosition.m_y), pStyles);
		}
		if (pStyles->doesHaveBottomBorder())
		{
			drawLine(vecMaxPoint, CVector2i32(vecPosition.m_x, vecMaxPoint.m_y), pStyles);
		}
	}
	else
	{
		// border-intersection-radius is set
		
		// http://stackoverflow.com/questions/628261/how-to-draw-rounded-rectangle-with-variable-width-border-inside-of-specific-boun
		// todo DrawPen.EndCap = DrawPen.StartCap = LineCap.Round;
		GraphicsPath *pGFXPath = new GraphicsPath;
		pGFXPath->AddArc(vecPosition.m_x, vecPosition.m_y, uiBorderIntersectionRadius, uiBorderIntersectionRadius, 180, 90);
		pGFXPath->AddArc(vecPosition.m_x + vecSize.m_x - uiBorderIntersectionRadius, vecPosition.m_y, uiBorderIntersectionRadius, uiBorderIntersectionRadius, 270, 90);
		pGFXPath->AddArc((INT)(vecPosition.m_x + vecSize.m_x - uiBorderIntersectionRadius), vecPosition.m_y + vecSize.m_y - uiBorderIntersectionRadius, uiBorderIntersectionRadius, uiBorderIntersectionRadius, 0, 90);
		pGFXPath->AddArc(vecPosition.m_x, vecPosition.m_y + vecSize.m_y - uiBorderIntersectionRadius, uiBorderIntersectionRadius, uiBorderIntersectionRadius, 90, 90);
		pGFXPath->CloseAllFigures();
		m_pGraphics->DrawPath(createPenFromStyles(pStyles), pGFXPath);
		delete pGFXPath;
	}
}

void					CGraphicsLibrary_GDIPlus::drawRectangleFill(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveFill())
	{
		return;
	}

	uint32 uiBorderIntersectionRadius = (uint32) pStyles->getStyle<float32>("border-intersection-radius");
	if (uiBorderIntersectionRadius == 0)
	{
		// border-intersection-radius is not set
		m_pGraphics->FillRectangle(createBackgroundBrushFromStyles(pStyles), getGdiplusRect(vecPosition, vecSize));
	}
	else
	{
		// border-intersection-radius is set

		// http://stackoverflow.com/questions/628261/how-to-draw-rounded-rectangle-with-variable-width-border-inside-of-specific-boun
		// todo DrawPen.EndCap = DrawPen.StartCap = LineCap.Round;
		GraphicsPath *pGFXPath = new GraphicsPath;
		pGFXPath->AddArc(vecPosition.m_x, vecPosition.m_y, uiBorderIntersectionRadius, uiBorderIntersectionRadius, 180, 90);
		pGFXPath->AddArc(vecPosition.m_x + vecSize.m_x - uiBorderIntersectionRadius, vecPosition.m_y, uiBorderIntersectionRadius, uiBorderIntersectionRadius, 270, 90);
		pGFXPath->AddArc((INT) (vecPosition.m_x + vecSize.m_x - uiBorderIntersectionRadius), vecPosition.m_y + vecSize.m_y - uiBorderIntersectionRadius, uiBorderIntersectionRadius, uiBorderIntersectionRadius, 0, 90);
		pGFXPath->AddArc(vecPosition.m_x, vecPosition.m_y + vecSize.m_y - uiBorderIntersectionRadius, uiBorderIntersectionRadius, uiBorderIntersectionRadius, 90, 90);
		pGFXPath->CloseAllFigures();
		m_pGraphics->FillPath(createBackgroundBrushFromStyles(pStyles), pGFXPath);
		delete pGFXPath;
	}
}

// draw square
void					CGraphicsLibrary_GDIPlus::drawSquare(CVector2i32& vecPosition, uint32 uiSize, CGUIStyles *pStyles)
{
	if (pStyles->doesHaveFill()) // todo - use Fill or Background for naming not both
	{
		drawSquareFill(vecPosition, uiSize, pStyles);
	}
	if (pStyles->doesHaveBorder())
	{
		drawSquareBorder(vecPosition, uiSize, pStyles);
	}
}

void					CGraphicsLibrary_GDIPlus::drawSquareBorder(CVector2i32& vecPosition, uint32 uiSize, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	drawRectangleBorder(vecPosition, CVector2ui32(uiSize, uiSize), pStyles);
}

void					CGraphicsLibrary_GDIPlus::drawSquareFill(CVector2i32& vecPosition, uint32 uiSize, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveFill())
	{
		return;
	}

	drawRectangleFill(vecPosition, CVector2ui32(uiSize, uiSize), pStyles);
}

// draw circle
void					CGraphicsLibrary_GDIPlus::drawCircle(CVector2i32& vecPosition, float32 fRadius, CGUIStyles *pStyles)
{
	if (pStyles->doesHaveFill())
	{
		drawCircleFill(vecPosition, fRadius, pStyles);
	}
	if (pStyles->doesHaveBorder())
	{
		drawCircleBorder(vecPosition, fRadius, pStyles);
	}
}

void					CGraphicsLibrary_GDIPlus::drawCircleBorder(CVector2i32& vecPosition, float32 fRadius, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	drawEllipseBorder(vecPosition, CMathUtility::get2DSizeFromCircle(vecPosition, fRadius), pStyles);
}

void					CGraphicsLibrary_GDIPlus::drawCircleFill(CVector2i32& vecPosition, float32 fRadius, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveFill())
	{
		return;
	}

	drawEllipseFill(vecPosition, CMathUtility::get2DSizeFromCircle(vecPosition, fRadius), pStyles);
}

// draw ellipse
void					CGraphicsLibrary_GDIPlus::drawEllipse(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles)
{
	if (pStyles->doesHaveFill())
	{
		drawEllipseFill(vecPosition, vecSize, pStyles);
	}
	if (pStyles->doesHaveBorder())
	{
		drawEllipseBorder(vecPosition, vecSize, pStyles);
	}
}

void					CGraphicsLibrary_GDIPlus::drawEllipseBorder(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	m_pGraphics->DrawEllipse(createPenFromStyles(pStyles), getGdiplusRect(vecPosition, vecSize));
}

void					CGraphicsLibrary_GDIPlus::drawEllipseFill(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveFill())
	{
		return;
	}

	m_pGraphics->FillEllipse(createBackgroundBrushFromStyles(pStyles), getGdiplusRect(vecPosition, vecSize));
}

// draw triangle
void					CGraphicsLibrary_GDIPlus::drawTriangle(CVector2i32& vecPoint1, CVector2i32& vecPoint2, CVector2i32& vecPoint3, CGUIStyles *pStyles)
{
	if (pStyles->doesHaveFill())
	{
		drawTriangleFill(vecPoint1, vecPoint2, vecPoint3, pStyles);
	}
	if (pStyles->doesHaveBorder())
	{
		drawTriangleBorder(vecPoint1, vecPoint2, vecPoint3, pStyles);
	}
}

void					CGraphicsLibrary_GDIPlus::drawTriangleBorder(CVector2i32& vecPoint1, CVector2i32& vecPoint2, CVector2i32& vecPoint3, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	vector<CVector2i32> vecPoints;
	vecPoints.resize(3);
	vecPoints[0] = vecPoint1;
	vecPoints[1] = vecPoint2;
	vecPoints[2] = vecPoint3;

	m_pGraphics->DrawPolygon(createPenFromStyles(pStyles), getGdiplusPointsFromVectorPoints(vecPoints), 3);
}

void					CGraphicsLibrary_GDIPlus::drawTriangleFill(CVector2i32& vecPoint1, CVector2i32& vecPoint2, CVector2i32& vecPoint3, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveFill())
	{
		return;
	}

	vector<CVector2i32> vecPoints;
	vecPoints.resize(3);
	vecPoints[0] = vecPoint1;
	vecPoints[1] = vecPoint2;
	vecPoints[2] = vecPoint3;

	m_pGraphics->FillPolygon(createBackgroundBrushFromStyles(pStyles), getGdiplusPointsFromVectorPoints(vecPoints), 3);
}

// draw polygon
void					CGraphicsLibrary_GDIPlus::drawPolygon(vector<CVector2i32>& vecPoints, CGUIStyles *pStyles)
{
	if (pStyles->doesHaveFill())
	{
		drawPolygonFill(vecPoints, pStyles);
	}
	if (pStyles->doesHaveBorder())
	{
		drawPolygonBorder(vecPoints, pStyles);
	}
}

void					CGraphicsLibrary_GDIPlus::drawPolygonBorder(vector<CVector2i32>& vecPoints, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	Point* pPoints = getGdiplusPointsFromVectorPoints(vecPoints);
	m_pGraphics->DrawPolygon(createPenFromStyles(pStyles), pPoints, vecPoints.size());
}

void					CGraphicsLibrary_GDIPlus::drawPolygonFill(vector<CVector2i32>& vecPoints, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveFill())
	{
		return;
	}

	Point* pPoints = getGdiplusPointsFromVectorPoints(vecPoints);
	m_pGraphics->FillPolygon(createBackgroundBrushFromStyles(pStyles), pPoints, vecPoints.size());
}

// draw text
void					CGraphicsLibrary_GDIPlus::drawText(CVector2i32& vecPosition, CVector2ui32& vecSize, string& strText, CGUIStyles *pStyles)
{
	CVector2i32 vecPosition2 = getTextPositionFromStyles(vecPosition, vecSize, strText, pStyles);

	wstring wstrText = CStringUtility::convertStdStringToStdWString(strText);
	RectF layoutRect(vecPosition2.m_x, vecPosition2.m_y, vecPosition2.m_x + vecSize.m_x, vecPosition2.m_y + vecSize.m_y);
	StringFormat stringFormat;

	m_pGraphics->DrawString(wstrText.c_str(), wstrText.length(), createFontFromStyles(pStyles), layoutRect, &stringFormat, createTextBrushFromStyles(pStyles));
}

// text size
CVector2ui32			CGraphicsLibrary_GDIPlus::getTextSize(string& strText, CGUIStyles *pStyles)
{
	wstring wstrText = CStringUtility::convertStdStringToStdWString(strText);
	const RectF layoutRect(0, 0, 30000, 30000);
	RectF boundingBox(0, 0, 0, 0);
	CVector2ui32 vecTextSize;

	m_pGraphics->MeasureString(wstrText.c_str(), wstrText.length(), createFontFromStyles(pStyles), layoutRect, &boundingBox);
	vecTextSize.m_x = boundingBox.Width;
	vecTextSize.m_y = boundingBox.Height;
	return vecTextSize;
}

// pen/brush creation
Pen*					CGraphicsLibrary_GDIPlus::createPenFromStyles(CGUIStyles *pStyles)
{
	Color colour = getGdiplusColourFromRGBAUint32(pStyles->getStyle<uint32>("border-colour"));
	uint32 uiBorderThickness = pStyles->getStyle<uint32>("border-thickness");

	Pen *pPen = new Gdiplus::Pen(colour, uiBorderThickness);

	// todo
	// border-style
	// pPen->SetDashStyle()

	return pPen;
}

Brush*					CGraphicsLibrary_GDIPlus::createBackgroundBrushFromStyles(CGUIStyles *pStyles)
{
	uint32 uiBrushType = 1;
	if (pStyles->doesStyleExist("fill-colour-start"))
	{
		uiBrushType = 2;
	}

	switch (uiBrushType)
	{
	case 1:
		{
			Color colour = getGdiplusColourFromRGBAUint32(pStyles->getStyle<uint32>("fill-colour"));
			return new SolidBrush(colour);
		}
	case 2:
		{
			PointF point1, point2;
			Color colour1, colour2;
			point1 = PointF(0, 0);
			point2 = PointF(200, 200);
			colour1 = getGdiplusColourFromRGBAUint32(pStyles->getStyle<uint32>("fill-colour-start"));
			colour2 = getGdiplusColourFromRGBAUint32(pStyles->getStyle<uint32>("fill-colour-stop"));
			return new LinearGradientBrush(point1, point2, colour1, colour2);
		}
	}
	
	return nullptr;
}

Brush*					CGraphicsLibrary_GDIPlus::createTextBrushFromStyles(CGUIStyles *pStyles)
{
	// todo - delete gui pens and brushes when no longer needed

	Color colour = getGdiplusColourFromRGBAUint32(pStyles->getStyle<uint32>("text-colour"));

	SolidBrush *pBrush = new SolidBrush(colour);
	
	return pBrush;
}

Font*					CGraphicsLibrary_GDIPlus::createFontFromStyles(CGUIStyles *pStyles)
{
	uint32 uiTextSize = pStyles->getStyle<uint32>("text-size");
	string strTextFont = pStyles->getStyle<string>("text-font");
	uint32 uiTextThickness = pStyles->getStyle<uint32>("text-thickness");

	INT iStyle = FontStyleRegular;
	if (pStyles->doesStyleExist("text-style"))
	{
		iStyle = 0;

		string a = pStyles->getStyle<string>("text-style");
		string b = CStringUtility::replace(pStyles->getStyle<string>("text-style"), " ", ",");

		string c = "a";

		vector<string> vecTextStyleValues = CStringUtility::split(CStringUtility::replace(pStyles->getStyle<string>("text-style"), " ", ","), ",");
		unordered_map<string, bool> umapTextStyleValues;
		for (string& strTextStyleValue : vecTextStyleValues)
		{
			umapTextStyleValues[strTextStyleValue] = true;
		}

		if (umapTextStyleValues.count("bold"))
		{
			iStyle |= FontStyleBold;
		}
		if (umapTextStyleValues.count("italic"))
		{
			iStyle |= FontStyleItalic;
		}
		if (umapTextStyleValues.count("underline"))
		{
			iStyle |= FontStyleUnderline;
		}
		if (umapTextStyleValues.count("strikethrough"))
		{
			iStyle |= FontStyleStrikeout;
		}
	}

	FontFamily *pFontFamily = new Gdiplus::FontFamily(CStringUtility::convertStdStringToStdWString(strTextFont).c_str());
	Font *pFont = new Font(pFontFamily, uiTextSize, iStyle, Gdiplus::UnitPixel);

	return pFont;
}

// position/size calculation
CVector2i32				CGraphicsLibrary_GDIPlus::getTextPositionFromStyles(CVector2i32& vecPosition, CVector2ui32& vecSize, string& strText, CGUIStyles *pStyles)
{
	string
		strAlignX = pStyles->getTextAlignX(),
		strAlignY = pStyles->getTextAlignY();
	CVector2i32
		vecOutPosition,
		vecMinInnerSpacing = pStyles->getMinInnerSpacing(),
		vecMaxInnerSpacing = pStyles->getMaxInnerSpacing();
	CVector2ui32
		vecTextSize = getTextSize(strText, pStyles);

	// calculate x position
	if (strAlignX == "left")
	{
		vecOutPosition.m_x = vecPosition.m_x;
	}
	else if (strAlignX == "right")
	{
		vecOutPosition.m_x = (vecPosition.m_x + vecSize.m_x) - vecTextSize.m_x;
	}
	else if (strAlignX == "center")
	{
		vecOutPosition.m_x = vecPosition.m_x + ((vecSize.m_x - vecTextSize.m_x) / 2);
	}
	vecOutPosition.m_x += vecMinInnerSpacing.m_x; // inner-spacing-left
	vecOutPosition.m_x -= vecMaxInnerSpacing.m_x; // inner-spacing-right
	
	// calculate y position
	if (strAlignY == "top")
	{
		vecOutPosition.m_y = vecPosition.m_y;
	}
	else if (strAlignY == "bottom")
	{
		vecOutPosition.m_y = (vecPosition.m_y + vecSize.m_y) - vecTextSize.m_y;
	}
	else if (strAlignY == "center")
	{
		vecOutPosition.m_y = vecPosition.m_y + ((vecSize.m_y - vecTextSize.m_y) / 2);
	}
	vecOutPosition.m_y += vecMinInnerSpacing.m_y; // inner-spacing-top
	vecOutPosition.m_y -= vecMaxInnerSpacing.m_y; // inner-spacing-bottom

	return vecOutPosition;
}

// utility
Point*					CGraphicsLibrary_GDIPlus::getGdiplusPointsFromVectorPoints(vector<CVector2i32>& vecPoints)
{
	// todo - delete
	Point* pPoints = new Point[vecPoints.size()];
	uint32 i = 0;
	for (CVector2i32& vecPoint : vecPoints)
	{
		pPoints[i].X = vecPoint.m_x;
		pPoints[i].Y = vecPoint.m_y;
		i++;
	}
	return pPoints;
}

Rect					CGraphicsLibrary_GDIPlus::getGdiplusRect(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	return Rect(vecPosition.m_x, vecPosition.m_y, vecSize.m_x, vecSize.m_y);
}

Color					CGraphicsLibrary_GDIPlus::getGdiplusColourFromRGBAUint32(uint32 uiColour)
{
	if (((uiColour >> 24) & 0xFF) == 0)
	{
		uiColour |= (0xFF << 24);
	}
	return Gdiplus::Color((uiColour >> 24) & 0xFF, uiColour & 0xFF, (uiColour >> 8) & 0xFF, (uiColour >> 16) & 0xFF);
}