#ifndef CInputEventCallbacks_H
#define CInputEventCallbacks_H

#include "CVector2i32.h"

class CInputEventCallbacks
{
public:
	virtual void					onLeftMouseDown(CVector2i32& vecCursorPoint) {}
	virtual void					onLeftMouseUp(CVector2i32& vecCursorPoint) {}
	virtual void					onMouseMove(CVector2i32& vecCursorPoint) {}
};

#endif