#include "CGDIPlusUtility.h"
#include "String/CStringUtility.h"
#include "Math/CMathUtility.h"
#include "CVector4ui32.h"

using namespace std;
using namespace Gdiplus;

HDC CGDIPlusUtility::m_hdc = nullptr;

void									CGDIPlusUtility::drawLine(CVector2ui32& vecPoint1, CVector2ui32& vecPoint2, uint32 uiLineColour)
{
	uint8
		ucRed = (uiLineColour >> 24) & 0xFF,
		ucGreen = (uiLineColour >> 16) & 0xFF,
		ucBlue = (uiLineColour >> 8) & 0xFF,
		ucAlpha = uiLineColour & 0xFF
		;

	Pen pen(Color(ucAlpha, ucRed, ucGreen, ucBlue));
	Graphics g(getHDC());
	g.DrawLine(&pen, (INT)vecPoint1.m_x, (INT)vecPoint1.m_y, (INT)vecPoint2.m_x, (INT)vecPoint2.m_y);
}

void									CGDIPlusUtility::drawText(CVector2ui32& vecMinPoint, CVector2ui32& vecSize, string& strText, uint32 uiTextColour, uint32 uiFontSize, bool bIsBold)
{
	HDC hdc = getHDC();
	HFONT hFont = CreateFont(uiFontSize, 0, 0, 0, bIsBold ? FW_BOLD : FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Verdana"));
	SelectObject(hdc, hFont);

	uint8
		ucRed = (uiTextColour >> 24) & 0xFF,
		ucGreen = (uiTextColour >> 16) & 0xFF,
		ucBlue = (uiTextColour >> 8) & 0xFF,
		ucAlpha = uiTextColour & 0xFF
		;

	SetTextColor(hdc, RGB(ucRed, ucGreen, ucBlue));
	SetBkMode(hdc, TRANSPARENT);

	RECT rect;
	rect.left = vecMinPoint.m_x;
	rect.top = vecMinPoint.m_y;
	rect.right = vecMinPoint.m_x + vecSize.m_x;
	rect.bottom = vecMinPoint.m_y + vecSize.m_y;
	wstring wStr = CStringUtility::convertStdStringToStdWString(strText);
	DrawText(hdc, wStr.c_str(), wStr.length(), &rect, DT_NOPREFIX);

	DeleteObject(hFont);
}

void									CGDIPlusUtility::drawRectangleFill(CVector2ui32& vecMinPoint, CVector2ui32& vecSize, uint32 uiFillColour)
{
	RECT rect;
	rect.left = vecMinPoint.m_x;
	rect.top = vecMinPoint.m_y;
	rect.right = rect.left + vecSize.m_x;
	rect.bottom = rect.top + vecSize.m_y;

	COLORREF uiFillColour2 = RGB(
		(uiFillColour >> 24) & 0xFF,
		(uiFillColour >> 16) & 0xFF,
		(uiFillColour >> 8) & 0xFF
		);
	HBRUSH hBrush = CreateSolidBrush(uiFillColour2);
	FillRect(getHDC(), &rect, hBrush);
	DeleteObject(hBrush);
}

void									CGDIPlusUtility::drawRectangleBorder(CVector2ui32& vecMinPoint, CVector2ui32& vecSize, uint32 uiLineColour)
{
	// todo use polygon point rendering instead of lines

	CVector2ui32 vecMaxPoint = vecMinPoint + vecSize;
	drawLine(vecMinPoint, CVector2ui32(vecMaxPoint.m_x, vecMinPoint.m_y), uiLineColour);
	drawLine(vecMinPoint, CVector2ui32(vecMinPoint.m_x, vecMaxPoint.m_y), uiLineColour);
	drawLine(vecMaxPoint, CVector2ui32(vecMinPoint.m_x, vecMaxPoint.m_y), uiLineColour);
	drawLine(vecMaxPoint, CVector2ui32(vecMaxPoint.m_x, vecMinPoint.m_y), uiLineColour);
}

void									CGDIPlusUtility::drawRectangleWithBorderRadius(CVector2ui32& vecMinPoint, CVector2ui32& vecSize, uint32 uiCornerRadius, uint32 uiFillColour, uint32 uiLineColour)
{
	// http://stackoverflow.com/questions/628261/how-to-draw-rounded-rectangle-with-variable-width-border-inside-of-specific-boun

	//int strokeOffset = Convert.ToInt32(Math.Ceiling(DrawPen.Width));
	//Bounds = Rectangle.Inflate(Bounds, -strokeOffset, -strokeOffset);

	uint8
		ucRed1 = (uiFillColour >> 24) & 0xFF,
		ucGreen1 = (uiFillColour >> 16) & 0xFF,
		ucBlue1 = (uiFillColour >> 8) & 0xFF,
		ucAlpha1 = uiFillColour & 0xFF
		;
	uint8
		ucRed2 = (uiLineColour >> 24) & 0xFF,
		ucGreen2 = (uiLineColour >> 16) & 0xFF,
		ucBlue2 = (uiLineColour >> 8) & 0xFF,
		ucAlpha2 = uiLineColour & 0xFF
		;

	//DrawPen.SetEndCap(LineCap.Round);
	//DrawPen.StartCap();

	GraphicsPath *gfxPath = new GraphicsPath(FillMode::FillModeAlternate);
	gfxPath->AddArc((INT)vecMinPoint.m_x, vecMinPoint.m_y, uiCornerRadius, uiCornerRadius, 180, 90);
	gfxPath->AddArc(vecMinPoint.m_x + vecSize.m_x - uiCornerRadius, (INT)vecMinPoint.m_y, uiCornerRadius, uiCornerRadius, 270, 90);
	gfxPath->AddArc(vecMinPoint.m_x + vecSize.m_x - uiCornerRadius, vecMinPoint.m_y + vecSize.m_y - uiCornerRadius, (INT)uiCornerRadius, uiCornerRadius, 0, 90);
	gfxPath->AddArc((INT)vecMinPoint.m_x, vecMinPoint.m_y + vecSize.m_y - uiCornerRadius, uiCornerRadius, uiCornerRadius, 90, 90);
	gfxPath->CloseAllFigures();

	Graphics g(getHDC());

	g.FillPath(new SolidBrush(Color(ucRed1, ucGreen1, ucBlue1)), gfxPath);

	Pen pen(Color(ucRed2, ucGreen2, ucBlue2));
	pen.SetWidth(5);
	pen.SetStartCap(LineCap::LineCapRound);
	pen.SetEndCap(LineCap::LineCapRound);
	g.DrawPath(&pen, gfxPath);
}

void									CGDIPlusUtility::drawRectangleFillWithGradient(CVector2ui32& vecMinPoint, CVector2ui32& vecSize, uint32 uiFillColourStart, uint32 uiFillColourStop)
{
	uint8
		ucRed1 = (uiFillColourStart >> 24) & 0xFF,
		ucGreen1 = (uiFillColourStart >> 16) & 0xFF,
		ucBlue1 = (uiFillColourStart >> 8) & 0xFF,
		ucAlpha1 = uiFillColourStart & 0xFF
		;
	uint8
		ucRed2 = (uiFillColourStop >> 24) & 0xFF,
		ucGreen2 = (uiFillColourStop >> 16) & 0xFF,
		ucBlue2 = (uiFillColourStop >> 8) & 0xFF,
		ucAlpha2 = uiFillColourStop & 0xFF
		;

	Rect rect;
	rect.X = vecMinPoint.m_x;
	rect.Y = vecMinPoint.m_y;
	rect.Width = vecSize.m_x;
	rect.Height = vecSize.m_y;

	/*
	LinearGradientBrush brush(
		Point((INT)0, (INT)0),
		Point((INT)0, (INT)vecSize.m_y),
		Color(ucAlpha1, ucRed1, ucGreen1, ucBlue1),
		Color(ucAlpha2, ucRed2, ucGreen2, ucBlue2));
	*/
	LinearGradientBrush *pBrush = new LinearGradientBrush(rect, Color(ucAlpha1, ucRed1, ucGreen1, ucBlue1), Color(ucAlpha2, ucRed2, ucGreen2, ucBlue2), LinearGradientModeVertical);

	Graphics g(getHDC());
	g.FillRectangle(pBrush, rect);

	delete pBrush;
}

void									CGDIPlusUtility::drawCircleFill(CVector2ui32& vecCenterPoint, float32 fRadius, uint32 uiFillColour)
{
	uint8
		ucRed = (uiFillColour >> 24) & 0xFF,
		ucGreen = (uiFillColour >> 16) & 0xFF,
		ucBlue = (uiFillColour >> 8) & 0xFF,
		ucAlpha = uiFillColour & 0xFF
		;

	CVector4ui32 vecRect = CMathUtility::getRectangleFromCircle(vecCenterPoint, fRadius);
	Rect rect;
	rect.X = vecRect.m_x;
	rect.Y = vecRect.m_y;
	rect.Width = fRadius * 2;
	rect.Height = fRadius * 2;

	Graphics g(getHDC());
	Color colour = Color(ucAlpha, ucRed, ucGreen, ucBlue);
	SolidBrush *pBrush = new SolidBrush(colour);
	g.FillPie(pBrush, rect, 0, 360);
	delete pBrush;
}

void									CGDIPlusUtility::drawCircleBorder(CVector2ui32& vecCenterPoint, float32 fRadius, uint32 uiLineColour)
{
	uint8
		ucRed = (uiLineColour >> 24) & 0xFF,
		ucGreen = (uiLineColour >> 16) & 0xFF,
		ucBlue = (uiLineColour >> 8) & 0xFF,
		ucAlpha = uiLineColour & 0xFF
		;

	CVector4ui32 vecRect = CMathUtility::getRectangleFromCircle(vecCenterPoint, fRadius);
	Rect rect;
	rect.X = vecRect.m_x;
	rect.Y = vecRect.m_y;
	rect.Width = fRadius * 2;
	rect.Height = fRadius * 2;

	Graphics g(getHDC());
	Color colour = Color(ucAlpha, ucRed, ucGreen, ucBlue);
	SolidBrush *pBrush = new SolidBrush(colour);
	Pen *pPen = new Pen(pBrush);
	//g.DrawPie(pPen, rect, 0, 360);
	g.DrawEllipse(pPen, rect);
	delete pPen;
	delete pBrush;
}

uint32					CGDIPlusUtility::getTextWidth(string& strText, uint32 uiFontSize)
{
	HFONT hFont = CreateFont(uiFontSize, 0, 0, 0, false ? FW_BOLD : FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Verdana"));
	SelectObject(getHDC(), hFont);

	SIZE textSize;
	GetTextExtentPoint32(getHDC(), CStringUtility::convertStdStringToStdWString(strText).c_str(), strText.length(), &textSize);

	DeleteObject(hFont);

	return textSize.cx;
}