#include "CGraphicsLibrary_GDIPlus.h"
#include "Math/CMathUtility.h"
#include "String/CStringUtility.h"
#include "GUI/Styles/CGUIStyles.h"
#include "CColour.h"

using namespace std;
using namespace Gdiplus;

CGraphicsLibrary_GDIPlus::CGraphicsLibrary_GDIPlus(void) :
	m_pGraphics(nullptr)
{
}

// draw line
void					CGraphicsLibrary_GDIPlus::drawLine(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	m_pGraphics->DrawLine(createPenFromStyles(pStyles), (INT) vecPoint1.m_x, (INT) vecPoint1.m_y, (INT) vecPoint2.m_x, (INT) vecPoint2.m_y);
}

// draw rectangle
void					CGraphicsLibrary_GDIPlus::drawRectangle(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	if (pStyles->doesHaveFill())
	{
		drawRectangleFill(vecPosition, vecSize, pStyles);
	}
	if (pStyles->doesHaveBorder())
	{
		drawRectangleBorder(vecPosition, vecSize, pStyles);
	}
}

void					CGraphicsLibrary_GDIPlus::drawRectangleBorder(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	uint32 uiBorderIntersectionRadius = (uint32) pStyles->getStyle<float32>("border-intersection-radius");
	if (uiBorderIntersectionRadius == 0)
	{
		// border-intersection-radius is not set
		CPoint2D vecMaxPoint = CPoint2D(vecPosition.m_x + vecSize.m_x, vecPosition.m_y + vecSize.m_y);
		if (pStyles->doesHaveLeftBorder())
		{
			drawLine(vecPosition, CPoint2D(vecPosition.m_x, vecMaxPoint.m_y), pStyles);
		}
		if (pStyles->doesHaveRightBorder())
		{
			drawLine(vecMaxPoint, CPoint2D(vecMaxPoint.m_x, vecPosition.m_y), pStyles);
		}
		if (pStyles->doesHaveTopBorder())
		{
			drawLine(vecPosition, CPoint2D(vecMaxPoint.m_x, vecPosition.m_y), pStyles);
		}
		if (pStyles->doesHaveBottomBorder())
		{
			drawLine(vecMaxPoint, CPoint2D(vecPosition.m_x, vecMaxPoint.m_y), pStyles);
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

void					CGraphicsLibrary_GDIPlus::drawRectangleFill(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
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
void					CGraphicsLibrary_GDIPlus::drawSquare(CPoint2D& vecPosition, uint32 uiSize, CGUIStyles *pStyles)
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

void					CGraphicsLibrary_GDIPlus::drawSquareBorder(CPoint2D& vecPosition, uint32 uiSize, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	drawRectangleBorder(vecPosition, CSize2D(uiSize, uiSize), pStyles);
}

void					CGraphicsLibrary_GDIPlus::drawSquareFill(CPoint2D& vecPosition, uint32 uiSize, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveFill())
	{
		return;
	}

	drawRectangleFill(vecPosition, CSize2D(uiSize, uiSize), pStyles);
}

// draw circle
void					CGraphicsLibrary_GDIPlus::drawCircle(CPoint2D& vecPosition, float32 fRadius, CGUIStyles *pStyles)
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

void					CGraphicsLibrary_GDIPlus::drawCircleBorder(CPoint2D& vecPosition, float32 fRadius, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	drawEllipseBorder(vecPosition, CMathUtility::get2DSizeFromCircle(vecPosition, fRadius), pStyles);
}

void					CGraphicsLibrary_GDIPlus::drawCircleFill(CPoint2D& vecPosition, float32 fRadius, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveFill())
	{
		return;
	}

	drawEllipseFill(vecPosition, CMathUtility::get2DSizeFromCircle(vecPosition, fRadius), pStyles);
}

// draw ellipse
void					CGraphicsLibrary_GDIPlus::drawEllipse(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
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

void					CGraphicsLibrary_GDIPlus::drawEllipseBorder(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	m_pGraphics->DrawEllipse(createPenFromStyles(pStyles), getGdiplusRect(vecPosition, vecSize));
}

void					CGraphicsLibrary_GDIPlus::drawEllipseFill(CPoint2D& vecPosition, CSize2D& vecSize, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveFill())
	{
		return;
	}

	m_pGraphics->FillEllipse(createBackgroundBrushFromStyles(pStyles), getGdiplusRect(vecPosition, vecSize));
}

// draw triangle
void					CGraphicsLibrary_GDIPlus::drawTriangle(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CPoint2D& vecPoint3, CGUIStyles *pStyles)
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

void					CGraphicsLibrary_GDIPlus::drawEquilateralTriangle(CPoint2D& vecPosition, float32 fSideLength, uint32 uiPointingDirection, CGUIStyles *pStyles)
{
	vector<CPoint2D> vecShapePoints = CMathUtility::getEquilateralTrianglePoints(vecPosition, fSideLength, uiPointingDirection);
	drawTriangle(vecShapePoints[0], vecShapePoints[1], vecShapePoints[2], pStyles);
}

void					CGraphicsLibrary_GDIPlus::drawEquilateralTriangle(CPoint2D& vecBottomLeftPoint, CPoint2D& vecBottomRightPoint, CGUIStyles *pStyles)
{
	vector<CPoint2D> vecShapePoints = CMathUtility::getEquilateralTrianglePoints(vecBottomLeftPoint, vecBottomRightPoint);
	drawTriangle(vecShapePoints[0], vecShapePoints[1], vecShapePoints[2], pStyles);
}

void					CGraphicsLibrary_GDIPlus::drawEquilateralTriangle(CPoint2D& vecBottomLeftPoint, float32 fSideLength, float32 fBaseAngle, CGUIStyles *pStyles)
{
	vector<CPoint2D> vecShapePoints = CMathUtility::getEquilateralTrianglePoints(vecBottomLeftPoint, fSideLength, fBaseAngle);
	drawTriangle(vecShapePoints[0], vecShapePoints[1], vecShapePoints[2], pStyles);
}

void					CGraphicsLibrary_GDIPlus::drawIsoscelesTriangle(CPoint2D& vecPosition, float32 fBaseLength, float32 fLegLength, uint32 uiPointingDirection, CGUIStyles *pStyles)
{
	vector<CPoint2D> vecShapePoints = CMathUtility::getIsoscelesTrianglePoints(vecPosition, fBaseLength, fLegLength, uiPointingDirection);
	drawTriangle(vecShapePoints[0], vecShapePoints[1], vecShapePoints[2], pStyles);
}

void					CGraphicsLibrary_GDIPlus::drawIsoscelesTriangle(CPoint2D& vecBaseCenterPoint, CPoint2D& vecTipPoint, uint32 uiBaseHalfWidth, CGUIStyles *pStyles)
{
	vector<CPoint2D> vecShapePoints = CMathUtility::getIsoscelesTrianglePoints(vecBaseCenterPoint, vecTipPoint, uiBaseHalfWidth);
	drawTriangle(vecShapePoints[0], vecShapePoints[1], vecShapePoints[2], pStyles);
}

void					CGraphicsLibrary_GDIPlus::drawIsoscelesTriangle(CPoint2D& vecBottomLeftPoint, float32 fBaseLength, float32 fTipAngle, float32 fBaseAngle, CGUIStyles *pStyles)
{
	vector<CPoint2D> vecShapePoints = CMathUtility::getIsoscelesTrianglePoints(vecBottomLeftPoint, fBaseLength, fTipAngle, fBaseAngle);
	drawTriangle(vecShapePoints[0], vecShapePoints[1], vecShapePoints[2], pStyles);
}

void					CGraphicsLibrary_GDIPlus::drawTriangleBorder(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CPoint2D& vecPoint3, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	vector<CPoint2D> vecPoints;
	vecPoints.resize(3);
	vecPoints[0] = vecPoint1;
	vecPoints[1] = vecPoint2;
	vecPoints[2] = vecPoint3;

	m_pGraphics->DrawPolygon(createPenFromStyles(pStyles), getGdiplusPointsFromVectorPoints(vecPoints), 3);
}

void					CGraphicsLibrary_GDIPlus::drawTriangleFill(CPoint2D& vecPoint1, CPoint2D& vecPoint2, CPoint2D& vecPoint3, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveFill())
	{
		return;
	}

	vector<CPoint2D> vecPoints;
	vecPoints.resize(3);
	vecPoints[0] = vecPoint1;
	vecPoints[1] = vecPoint2;
	vecPoints[2] = vecPoint3;

	m_pGraphics->FillPolygon(createBackgroundBrushFromStyles(pStyles), getGdiplusPointsFromVectorPoints(vecPoints), 3);
}

// draw polygon
void					CGraphicsLibrary_GDIPlus::drawPolygon(vector<CPoint2D>& vecPoints, CGUIStyles *pStyles)
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

void					CGraphicsLibrary_GDIPlus::drawPolygonBorder(vector<CPoint2D>& vecPoints, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveBorder())
	{
		return;
	}

	Point* pPoints = getGdiplusPointsFromVectorPoints(vecPoints);
	m_pGraphics->DrawPolygon(createPenFromStyles(pStyles), pPoints, vecPoints.size());
}

void					CGraphicsLibrary_GDIPlus::drawPolygonFill(vector<CPoint2D>& vecPoints, CGUIStyles *pStyles)
{
	if (!pStyles->doesHaveFill())
	{
		return;
	}

	Point* pPoints = getGdiplusPointsFromVectorPoints(vecPoints);
	m_pGraphics->FillPolygon(createBackgroundBrushFromStyles(pStyles), pPoints, vecPoints.size());
}

// draw text
void					CGraphicsLibrary_GDIPlus::drawText(CPoint2D& vecPosition, CSize2D& vecSize, string& strText, CGUIStyles *pStyles, CSize2D vecTextSize)
{
	CPoint2D vecPosition2 = getTextPositionFromStyles(vecPosition, vecSize, strText, vecTextSize, pStyles);

	wstring wstrText = CStringUtility::convertStdStringToStdWString(strText);
	RectF layoutRect(vecPosition2.m_x, vecPosition2.m_y, vecPosition2.m_x + vecSize.m_x, vecPosition2.m_y + vecSize.m_y);
	StringFormat stringFormat;

	m_pGraphics->DrawString(wstrText.c_str(), wstrText.length(), createFontFromStyles(pStyles), layoutRect, &stringFormat, createTextBrushFromStyles(pStyles));
}

void					CGraphicsLibrary_GDIPlus::drawText(CPoint2D& vecPosition, CSize2D& vecSize, CGUIString& gstrString, CGUIStyles *pStyles)
{
	drawText(vecPosition, vecSize, gstrString.getString(), pStyles, gstrString.getSize());
}

// text size
CSize2D					CGraphicsLibrary_GDIPlus::getTextSize(string& strText, CGUIStyles *pStyles)
{
	wstring wstrText = CStringUtility::convertStdStringToStdWString(strText);
	const RectF layoutRect(0, 0, 30000, 30000);
	RectF boundingBox(0, 0, 0, 0);
	CSize2D vecTextSize;

	m_pGraphics->MeasureString(wstrText.c_str(), wstrText.length(), createFontFromStyles(pStyles), layoutRect, &boundingBox);
	vecTextSize.m_x = boundingBox.Width;
	vecTextSize.m_y = boundingBox.Height;
	return vecTextSize;
}

// image
void					CGraphicsLibrary_GDIPlus::drawImage(CPoint2D& vecPosition, string& strImagePath, CSize2D vecSize)
{
	drawImage(vecPosition, new Image(CStringUtility::convertStdStringToStdWString(strImagePath).c_str()), vecSize);
}

void					CGraphicsLibrary_GDIPlus::drawImage(CPoint2D& vecPosition, Image *pImage, CSize2D vecSize)
{
	if (vecSize.width() == 0 && vecSize.height() == 0)
	{
		// size not specified
		m_pGraphics->DrawImage(pImage, vecPosition.x(), vecPosition.y());
	}
	else
	{
		// size specified
		m_pGraphics->DrawImage(pImage, vecPosition.x(), vecPosition.y(), vecSize.width(), vecSize.height());
	}
}

// pen/brush creation
Pen*					CGraphicsLibrary_GDIPlus::createPenFromStyles(CGUIStyles *pStyles)
{
	Color colour = getGdiplusColourFromColour(&pStyles->getStyle<CColour>("border-colour"));
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
			Color colour = getGdiplusColourFromColour(&pStyles->getStyle<CColour>("fill-colour"));
			return new SolidBrush(colour);
		}
	case 2:
		{
			PointF point1, point2;
			Color colour1, colour2;
			point1 = PointF(0, 0);
			point2 = PointF(200, 200);
			colour1 = getGdiplusColourFromColour(&pStyles->getStyle<CColour>("fill-colour-start"));
			colour2 = getGdiplusColourFromColour(&pStyles->getStyle<CColour>("fill-colour-stop"));
			return new LinearGradientBrush(point1, point2, colour1, colour2);
		}
	}
	
	return nullptr;
}

Brush*					CGraphicsLibrary_GDIPlus::createTextBrushFromStyles(CGUIStyles *pStyles)
{
	// todo - delete gui pens and brushes when no longer needed

	Color colour = getGdiplusColourFromColour(&pStyles->getStyle<CColour>("text-colour"));

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
CPoint2D				CGraphicsLibrary_GDIPlus::getTextPositionFromStyles(CPoint2D& vecPosition, CSize2D& vecSize, string& strText, CSize2D& vecTextSize, CGUIStyles *pStyles)
{
	string
		strAlignX = pStyles->getTextAlignX(),
		strAlignY = pStyles->getTextAlignY();
	CPoint2D
		vecOutPosition,
		vecMinInnerSpacing = pStyles->getMinInnerSpacing(),
		vecMaxInnerSpacing = pStyles->getMaxInnerSpacing();

	if (vecTextSize.m_x == 0 && vecTextSize.m_y == 0)
	{
		vecTextSize = getTextSize(strText, pStyles);
	}

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
Point*					CGraphicsLibrary_GDIPlus::getGdiplusPointsFromVectorPoints(vector<CPoint2D>& vecPoints)
{
	// todo - delete
	Point* pPoints = new Point[vecPoints.size()];
	uint32 i = 0;
	for (CPoint2D& vecPoint : vecPoints)
	{
		pPoints[i].X = vecPoint.m_x;
		pPoints[i].Y = vecPoint.m_y;
		i++;
	}
	return pPoints;
}

Rect					CGraphicsLibrary_GDIPlus::getGdiplusRect(CPoint2D& vecPosition, CSize2D& vecSize)
{
	return Rect(vecPosition.m_x, vecPosition.m_y, vecSize.m_x, vecSize.m_y);
}

Color					CGraphicsLibrary_GDIPlus::getGdiplusColourFromColour(CColour *pColour)
{
	return Gdiplus::Color(pColour->alpha(), pColour->red(), pColour->green(), pColour->blue());
}