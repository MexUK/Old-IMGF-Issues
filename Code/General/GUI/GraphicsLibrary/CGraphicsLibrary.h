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

	virtual void					drawRectangle(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawRectangleBorder(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawRectangleFill(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawCircle(CVector2i32& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawCircleBorder(CVector2i32& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawCircleFill(CVector2i32& vecPosition, float32 fRadius, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawEllipse(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawEllipseBorder(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawEllipseFill(CVector2i32& vecPosition, CVector2ui32& vecSize, CGUIStyles *pStyles = nullptr) = 0;
	
	virtual void					drawTriangle(CVector2i32& vecPosition, uint32 uiSideLength, uint8 uiFacingDirection, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawTriangleBorder(CVector2i32& vecPosition, uint32 uiSideLength, uint8 uiFacingDirection, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawTriangleFill(CVector2i32& vecPosition, uint32 uiSideLength, uint8 uiFacingDirection, CGUIStyles *pStyles = nullptr) = 0;
	
	virtual void					drawPolygon(std::vector<CVector2i32>& vecPoints, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawPolygonBorder(std::vector<CVector2i32>& vecPoints, CGUIStyles *pStyles = nullptr) = 0;
	virtual void					drawPolygonFill(std::vector<CVector2i32>& vecPoints, CGUIStyles *pStyles = nullptr) = 0;

	virtual void					drawText(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string& strText, CGUIStyles *pStyles = nullptr) = 0;
	virtual CVector2ui32			getTextSize(std::string& strText, CGUIStyles *pStyles = nullptr) = 0;
};

#endif