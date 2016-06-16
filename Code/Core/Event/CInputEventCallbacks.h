#ifndef CInputEventCallbacks_H
#define CInputEventCallbacks_H

#include "CPoint2D.h"

class CInputEventCallbacks
{
public:
	virtual void					onLeftMouseDown(CPoint2D& vecCursorPoint) {}
	virtual void					onLeftMouseUp(CPoint2D& vecCursorPoint) {}
	virtual void					onMouseMove(CPoint2D& vecCursorPoint) {}
};

#endif