#ifndef CGDIPlusUtility_H
#define CGDIPlusUtility_H

#include <Windows.h>
#include "CVector2ui32.h"
#include <gdiplus.h>
#include <string>

class CGDIPlusUtility
{
public:
	static void				drawLine(CVector2ui32& vecPoint1, CVector2ui32& vecPoint2, uint32 uiLineColour);
	static void				drawText(CVector2ui32& vecMinPoint, CVector2ui32& vecSize, std::string& strText, uint32 uiTextColour, uint32 uiFontSize, bool bIsBold);
	
	static void				drawRectangleFill(CVector2ui32& vecMinPoint, CVector2ui32& vecSize, uint32 uiFillColour);
	static void				drawRectangleBorder(CVector2ui32& vecMinPoint, CVector2ui32& vecSize, uint32 uiLineColour);
	static void				drawRectangleWithBorderRadius(CVector2ui32& vecMinPoint, CVector2ui32& vecSize, uint32 uiCornerRadius, uint32 uiFillColour, uint32 uiLineColour);
	static void				drawRectangleFillWithGradient(CVector2ui32& vecMinPoint, CVector2ui32& vecSize, uint32 uiFillColourStart, uint32 uiFillColourStop);

	static void				drawCircleFill(CVector2ui32& vecCenterPoint, float32 fRadius, uint32 uiFillColour);
	static void				drawCircleBorder(CVector2ui32& vecCenterPoint, float32 fRadius, uint32 uiLineColour);
	
	static void				drawPolygonFill();
	static void				drawPolygonBorder();

	static uint32			getTextWidth(std::string& strText, uint32 uiFontSize);

	static void				setHDC(HDC hdc) { m_hdc = hdc; }
	static HDC				getHDC(void) { return m_hdc; }

private:
	static HDC				m_hdc;
};

#endif